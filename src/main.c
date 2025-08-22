/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "keyboard_config.h"
#include "usb_descriptors.h"

#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <tusb.h>
#include <class/hid/hid.h>
#include "bsp/board_api.h"

#include "structs.h"
#include "matrix_status.h"

#include "firmware_timer.h"
#include "matrix.h"
#include "board.h"
#include "debounce.h"
#include "keyboard_mapping.h"

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "keyboard_led.h"
#include "keycode_buffer.h"
#include "layer.h"

#include "ssd1306.h"

#define QUEUE_SIZE 128
#define LAYER_MOD_KEY ((uint8_t)(LAYER_MODIFIER_KEY - A1))

typedef struct
{
  bool caps_lock : 1;
  bool num_lock : 1;
  bool scroll_lock : 1;
} indicator_state_t;

indicator_state_t keyboard_indicator_state = {0, 0, 0};
keycode_buffer_t keycode_buffer = {0};
keycode_buffer_t consumer_control_buffer = {0};

typedef struct
{
  uint8_t ch;
  bool done;
} other_board_t;

typedef struct
{
  uint8_t size;
  uint8_t ch[QUEUE_SIZE];
  modifier_t modifiers;
} queue_t;


void send_consumer_control_block();
void send_keyboard_report_block();

other_board_t ch = {.ch = 0, .done = 1};
queue_t key_queue = {
    .size = 0,
    .ch = {0},
    .modifiers = {.bits = 0},
};

static void encoder_task();
static void layer_key_task();
void display_task();

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define IS_RGBW false

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum
{
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

PIO pio;
uint sm;
uint offset;

PIO pio_2;
uint sm_2;
uint offset_2;

void uart_read_task();

void led_task()
{
  run_pattern(pio, sm, NUM_KEYS);
}

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

void set_indicator_leds()
{
  const int8_t layer = get_layer();
  const uint8_t caps_lock = 0x4 * keyboard_indicator_state.caps_lock;
  const uint8_t num_lock = 0x4 * keyboard_indicator_state.num_lock;
  const uint8_t scroll_lock = 0x4 * keyboard_indicator_state.scroll_lock;

  uint8_t red_addition = 0;
  uint8_t green_addition = 0;
  uint8_t blue_addition = 0;
  switch (layer)
  {
  case LAYER_SYMBOL:
    red_addition += 0x1;
    break;
  case LAYER_CONTROL:
    blue_addition += 0x1;
    break;
  default:
    break;
  }

  put_pixel(pio_2, sm_2, white_offset_urgb_u32(caps_lock, red_addition, green_addition, blue_addition));
  put_pixel(pio_2, sm_2, white_offset_urgb_u32(num_lock, red_addition, green_addition, blue_addition));
  put_pixel(pio_2, sm_2, white_offset_urgb_u32(scroll_lock, red_addition, green_addition, blue_addition));
}

uint8_t buf[SSD1306_BUF_LEN];

struct render_area frame_area = {
  start_col : 0,
  end_col : SSD1306_WIDTH - 1,
  start_page : 0,
  end_page : SSD1306_NUM_PAGES - 1
};

#define FLAG_VALUE 123

void core1_entry()
{
  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, KEYBOARD_BACKLIGHT_PIN, 1, true);
  hard_assert(success);
  ws2812_program_init(pio, sm, offset, KEYBOARD_BACKLIGHT_PIN, 800000, IS_RGBW);

  i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
  gpio_set_function(SSD1306_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SSD1306_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SSD1306_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(SSD1306_DEFAULT_I2C_SCL_PIN);

  sleep_ms(100);
  SSD1306_init();

  calc_render_area_buflen(&frame_area);

  // zero the entire display
  memset(buf, 0, SSD1306_BUF_LEN);

  SSD1306_send_cmd(SSD1306_SET_ALL_ON); // Set all pixels on

  SSD1306_send_cmd(SSD1306_SET_ENTIRE_ON); // go back to following RAM for pixel state

  while (1)
  {
    display_task();
    led_task();
  }
  pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
/*------------- MAIN -------------*/
int main(void)
{
  // board_init();
  stdio_init_all();

  setup_board();

  matrix_init();
  debounce_init(ROWS_PER_HAND);

  multicore_launch_core1(core1_entry);
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb)
  {
    board_init_after_tusb();
  }

  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio_2, &sm_2, &offset_2, INDICATOR_LEDS_PIN, 1, true);
  hard_assert(success);
  ws2812_program_init(pio_2, sm_2, offset_2, INDICATOR_LEDS_PIN, 800000, IS_RGBW);

  // Set up our UART with a basic baud rate.
  uart_init(UART_ID, BAUD_RATE);

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
  gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

  // Set UART flow control CTS/RTS, we don't want these, so turn them off
  uart_set_hw_flow(UART_ID, false, false);

  // Set our data format
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  // Turn off FIFO's - we want to do this character by character
  uart_set_fifo_enabled(UART_ID, false);
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  // And set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART_IRQ, uart_read_task);
  irq_set_enabled(UART_IRQ, true);

  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_ID, true, false);

  // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
  for (size_t i = A1; i <= T6; i++)
  {
    put_pixel(pio, sm, urgb_u32(0x7, 0x7, 0x7));
  }

  set_indicator_leds();

  while (1)
  {
    // changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);
    matrix_task();
    tud_task(); // tinyusb device task

    hid_task();
    if (tud_mounted())
    {
      if (tud_hid_ready())
      {
        if (keycode_buffer.size > keycode_buffer.completed)
        {
          send_keyboard_report_block();
        }
        else if (consumer_control_buffer.size > consumer_control_buffer.completed)
        {
          // uint8_t* key = consumer_control_buffer.keycodes + consumer_control_buffer.completed;
          send_consumer_control_block();
        }
        else
        {
          send_keyboard_report_block();
        }
      }
    }
    else
    {
      reset_keycode_buffer(&keycode_buffer);
      reset_keycode_buffer(&consumer_control_buffer);
    }

    encoder_task();

    if (tud_suspended())
      tud_remote_wakeup();
  }

  pio_remove_program_and_unclaim_sm(&ws2812_program, pio_2, sm_2, offset_2);
}


void uart_read_task()
{
  while (uart_is_readable(UART_ID))
  {
    uint8_t c = uart_getc(UART_ID);
    if ((c == LAYER_DOWN_CODE) || (c == LAYER_UP_CODE))
    {
      if (c == LAYER_DOWN_CODE)
      {
        update_layer(LAYER_DOWN);
      }
      else
      {
        update_layer(LAYER_UP);
      }
      set_indicator_leds();
    }
    else
    {
      if (!((c >= HID_KEY_CONTROL_LEFT) && (c <= HID_KEY_GUI_RIGHT)) && (c != HID_KEY_NONE))
      {
        key_queue.ch[key_queue.size] = c;
        key_queue.size++;
      }
      else
      {
        update_modifier(&key_queue.modifiers, c);
      }
    }
  }
}

void display_task()
{
  const fast_timer_t interval_us = 10000;
  static fast_timer_t start_us = 0;
  const fast_timer_t current_time = timer_read_fast();

  if (current_time - start_us < interval_us)
    return; // not enough time

  memset(buf, 0, SSD1306_BUF_LEN);

  start_us = current_time;

  char text[] = "00:00:00";
  fast_timer_t ms = current_time / 1000;
  fast_timer_t s = ms / 1000;
  uint8_t _s = s % 60;

  fast_timer_t mins = s / 60;
  uint8_t _mins = mins % 60;

  fast_timer_t hours = mins / 60;
  uint8_t _hours = hours % 100;

  text[0] = (_hours / 10 % 10) + '0';
  text[1] = (_hours % 10) + '0';

  text[3] = (_mins / 10 % 10) + '0';
  text[4] = (_mins % 10) + '0';

  text[6] = (_s / 10 % 10) + '0';
  text[7] = (_s % 10) + '0';

  write_string_vertical(buf, 120, 0, text);

  if (tud_mounted())
  {
    write_string_vertical(buf, 112, 0, "MOUNTED");
  }
  else
    write_string_vertical(buf, 112, 0, "N/A CONN");

  render(buf, &frame_area);
}
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = BLINK_MOUNTED; }

// Invoked when device is unmounted
void tud_umount_cb(void) { blink_interval_ms = BLINK_NOT_MOUNTED; }

// Invoked when usb bus is suspended
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void encoder_task()
{
  if (encoder_has_action())
  {
    if (encoder_a)
    {
      // uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
      // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
      // send_hid_report_mod(REPORT_ID_KEYBOARD, 0, HID_KEY_MINUS);
      // tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      brightness_update(1);
      encoder_a = false;
    }
    if (encoder_b)
    {
      // uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
      // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
      // send_hid_report_mod(REPORT_ID_KEYBOARD, 0, HID_KEY_MINUS);
      // tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      brightness_update(-1);
      encoder_b = false;
    }
    // if (encoder_b)
    // {
    //   // uint8_t volume_up[2] = {0x00, 0xE9};
    //   // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, volume_up, 2);
    //   send_hid_report_mod(REPORT_ID_KEYBOARD, 0, HID_KEY_MINUS);
    //   tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    //   encoder_b = false;
    //   has_consumer_key = true;
    // }
    if (encoder_button)
    {
      // uint16_t volume_mute = HID_USAGE_CONSUMER_MUTE;
      // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_mute, 2);
      // send_hid_report_mod(REPORT_ID_KEYBOARD, 0, HID_KEY_MUTE);
      // tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      led_cycle_pattern();
      // uint8_t keycode[6] = {HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F};
      // tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
      // tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      encoder_button = false;
    }
  }
}

bool is_modifier(uint8_t code)
{
  (void)code;
  return 0;
}

void layer_key_task()
{
  const fast_timer_t current_time = timer_read_fast();
  if (
      (matrix_bank_status[LAYER_MOD_KEY].is_pressed) &&
      (!matrix_bank_status[LAYER_MOD_KEY].last_state) &&
      (current_time > (matrix_bank_status[LAYER_MOD_KEY].last_handled_time + TAPHOLD_TIMEOUT)))
  {
#if KEYBOARD_SIDE == LEFT
    update_layer(LAYER_UP);
    uart_putc(UART_ID, LAYER_UP_CODE);
#else
    update_layer(LAYER_DOWN);
    uart_putc(UART_ID, LAYER_DOWN_CODE);
#endif
    set_indicator_leds();
    matrix_bank_status[LAYER_MOD_KEY].last_handled_time = current_time;
    matrix_bank_status[LAYER_MOD_KEY].last_state = true;
  }
}

void send_modifier_uart(modifier_t *modifier)
{
  if (modifier->modifiers.left_ctrl)
    uart_putc(UART_ID, HID_KEY_CONTROL_LEFT);
  if (modifier->modifiers.left_shift)
    uart_putc(UART_ID, HID_KEY_SHIFT_LEFT);
  if (modifier->modifiers.left_alt)
    uart_putc(UART_ID, HID_KEY_ALT_LEFT);
  if (modifier->modifiers.left_gui)
    uart_putc(UART_ID, HID_KEY_GUI_LEFT);
  if (modifier->modifiers.right_ctrl)
    uart_putc(UART_ID, HID_KEY_CONTROL_RIGHT);
  if (modifier->modifiers.right_shift)
    uart_putc(UART_ID, HID_KEY_SHIFT_RIGHT);
  if (modifier->modifiers.right_alt)
    uart_putc(UART_ID, HID_KEY_ALT_RIGHT);
  if (modifier->modifiers.right_gui)
    uart_putc(UART_ID, HID_KEY_GUI_RIGHT);
}

static inline void add_keycodes_n_composite(const key_layer_config_t *key)
{
  switch (key->key_type)
  {
  case REPORT_ID_KEYBOARD:
    add_keycodes_n(&keycode_buffer, key->keys, key->size);
    uart_puts(UART_ID, (const char *)key);
    break;
  case REPORT_ID_CONSUMER_CONTROL:
    add_keycode_layer(&consumer_control_buffer, key);
    break;

  default:
    break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc
// ..) tud_hid_report_complete_cb() is used to send the next report after
// previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const fast_timer_t interval_ms = 10000;
  static fast_timer_t start_ms = 0;
  const fast_timer_t current_time = timer_read_fast();

  if (current_time - start_ms < interval_ms)
    return; // not enough time

  start_ms = current_time;

  layer_key_task();
  shift_reset_keycode_buffer(&keycode_buffer);
  shift_reset_keycode_buffer(&consumer_control_buffer);

  for (int i = 0; i < NUM_KEYS; i++)
  {
    if ((matrix_bank_status[i].is_pressed))
    {
      update_modifier(&keycode_buffer.modifier, keymaps_layers[0][i].keys[0]);
    }
  }
  send_modifier_uart(&keycode_buffer.modifier);

  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    matrix_status *status = &matrix_bank_status[i];
    if ((status->is_pressed))
    {
      const key_layer_config_t key_layer_config = keymaps_layers[get_layer()][i];
      const uint8_t *key = key_layer_config.keys;
      switch (status->held)
      {
      case NO_TOUCH:
        if (!((*key >= HID_KEY_CONTROL_LEFT) && (*key <= HID_KEY_GUI_RIGHT)) && (*key != HID_KEY_NONE))
        {
          add_keycodes_n_composite(&key_layer_config);
        }
        status->last_handled_time = current_time;
        status->held = FIRST_TOUCH;
        break;
      case FIRST_TOUCH:
        if ((current_time > (status->last_handled_time + INITIAL_TAPHOLD_TIMEOUT)))
        {
          if (!((*key >= HID_KEY_CONTROL_LEFT) && (*key <= HID_KEY_GUI_RIGHT)) && (*key != HID_KEY_NONE))
          {
            add_keycodes_n_composite(&key_layer_config);
          }
          status->last_handled_time = current_time;
          status->held = CONTINUOUS_TOUCH;
        }
        break;
      case CONTINUOUS_TOUCH:
        if ((current_time > (status->last_handled_time + TAPHOLD_TIMEOUT)))
        {
          if (!((*key >= HID_KEY_CONTROL_LEFT) && (*key <= HID_KEY_GUI_RIGHT)) && (*key != HID_KEY_NONE))
          {
            add_keycodes_n_composite(&key_layer_config);
          }
          status->last_handled_time = current_time;
          status->held = CONTINUOUS_TOUCH;
        }
        break;
      default:
        break;
      }
    }
  }

  key_queue.ch[key_queue.size + 1] = HID_KEY_NONE;
  add_keycodes(&keycode_buffer, key_queue.ch);
  keycode_buffer.modifier.bits |= key_queue.modifiers.bits;
  key_queue.modifiers.bits = 0;
  key_queue.size = 0;
  memset(key_queue.ch, 0, QUEUE_SIZE);
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report,
                                uint16_t len)
{
  (void)instance;
  (void)len;

  uint8_t next_report_id = report[0];
  switch (next_report_id)
  {
  case REPORT_ID_KEYBOARD:
  {
    if (!keycode_buffer.null_sent)
    {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, keycode_buffer.modifier.bits, NULL);
      keycode_buffer.null_sent = true;
    }
    else
    {
      if (keycode_buffer.size > keycode_buffer.completed)
      {
        send_keyboard_report_block();
      }
    }
  }
  break;
  case REPORT_ID_CONSUMER_CONTROL:
  {
    if (!consumer_control_buffer.null_sent)
    {
      uint16_t empty_key = 0;
      tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
      consumer_control_buffer.null_sent = true;
    }
    else if (consumer_control_buffer.size > consumer_control_buffer.completed)
    {
      send_consumer_control_block();
    }
  }
  break;

  default:
    break;
  }
}

void send_consumer_control_block()
{
  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, consumer_control_buffer.keycodes + consumer_control_buffer.completed, 2);
  consumer_control_buffer.completed += 2;
  consumer_control_buffer.null_sent = false;
}

void send_keyboard_report_block()
{
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, keycode_buffer.modifier.bits, keycode_buffer.keycodes + keycode_buffer.completed);
  keycode_buffer.completed += 6;
  keycode_buffer.null_sent = false;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen)
{
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize)
{
  (void)instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if (bufsize < 1)
        return;

      uint8_t const kbd_leds = buffer[0];
      keyboard_indicator_state.caps_lock = kbd_leds & KEYBOARD_LED_CAPSLOCK;
      keyboard_indicator_state.num_lock = kbd_leds & KEYBOARD_LED_NUMLOCK;
      keyboard_indicator_state.scroll_lock = kbd_leds & KEYBOARD_LED_SCROLLLOCK;
      set_indicator_leds();
    }
  }
}
