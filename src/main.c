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

#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"
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
#include "layer.h"

#define QUEUE_SIZE 128
#define LAYER_MOD_KEY ((uint8_t)(LAYER_MODIFIER_KEY - A1))

typedef struct
{
  bool caps_lock : 1;
  bool num_lock : 1;
  bool scroll_lock : 1;
} indicator_state_t;

indicator_state_t keyboard_indicator_state = {0, 0, 0};

typedef struct
{
  uint8_t ch;
  bool done;
} other_board_t;

typedef struct
{
  uint8_t size;
  uint8_t ch[QUEUE_SIZE];
} queue_t;

other_board_t ch = {.ch = 0, .done = 1};
queue_t key_queue = {0, 0};

static void encoder_task();

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
  int8_t layer = get_layer();

  uint8_t caps_lock = 0b100 * keyboard_indicator_state.caps_lock;
  uint8_t num_lock = 0b100 * keyboard_indicator_state.num_lock;
  uint8_t scroll_lock = 0b100 * keyboard_indicator_state.scroll_lock;

  uint8_t red_addition = 0;
  uint8_t green_addition = 0;
  uint8_t blue_addition = 0;
  if (layer == 1) {
    red_addition += 0b01;
  }
  else if (layer == 2) {
    blue_addition += 0b1;
  }

  put_pixel(pio_2, sm_2, urgb_u32(red_addition + caps_lock, green_addition + caps_lock, blue_addition + caps_lock >> 1));
  put_pixel(pio_2, sm_2, urgb_u32(red_addition + num_lock, green_addition + num_lock, blue_addition + num_lock >> 1));
  put_pixel(pio_2, sm_2, urgb_u32(red_addition + scroll_lock, green_addition + scroll_lock, blue_addition + scroll_lock >> 1));
}

/*------------- MAIN -------------*/
int main(void)
{
  // board_init();
  stdio_init_all();
  setup_board();

  matrix_init();
  debounce_init(ROWS_PER_HAND);

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb)
  {
    board_init_after_tusb();
  }

  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, KEYBOARD_BACKLIGHT_PIN, 1, true);
  hard_assert(success);
  ws2812_program_init(pio, sm, offset, KEYBOARD_BACKLIGHT_PIN, 800000, IS_RGBW);

  success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio_2, &sm_2, &offset_2, INDICATOR_LEDS_PIN, 1, true);
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

  // Set up a RX interrupt
  // We need to set up the handler first
  // Select correct interrupt for the UART we are using
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  for (size_t i = A1; i <= T6; i++)
  {
    put_pixel(pio, sm, urgb_u32(0b100 + 0b11, 0b100 + 0b11, 0b100 + 0b11));
  }

  set_indicator_leds();

  uint8_t p = 0;
  while (!tud_hid_ready()) // WAIT;

    while (1)
    {
      // changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);
      bool changed = matrix_task();
      tud_task(); // tinyusb device task

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
          key_queue.ch[key_queue.size++] = c;
        }
      }

      hid_task();
      led_task();
      encoder_task();

      if (tud_suspended())
        tud_remote_wakeup();
    }

  pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
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
static void send_hid_report_mod(uint8_t report_id, uint8_t modifier, uint32_t btn);

static void encoder_task()
{
  // skip if hid is not ready yet
  // if (!tud_hid_ready())
  //   return;

  static bool has_consumer_key = false;
  if (encoder_has_action())
  {
    // if (encoder_a)
    // {
    //   // uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    //   // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
    //   send_hid_report_mod(REPORT_ID_KEYBOARD, 0, HID_KEY_MINUS);
    //   tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    //   encoder_a = false;
    //   has_consumer_key = true;
    // }
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
      encoder_button = false;
      has_consumer_key = true;
    }
    // uint16_t empty_key = 0;
    // tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
  }
  // else
  // {
  //   if (has_consumer_key)
  //   {
  //     uint16_t empty_key = 0;
  //     if (has_consumer_key)
  //   }
  //   has_consumer_key = true;
  // }
}

static void send_hid_report_mod(uint8_t report_id, uint8_t modifier, uint32_t btn)
{
  // skip if hid is not ready yet
  if (!tud_hid_ready())
    return;

  // use to avoid send multiple consecutive zero report for keyboard
  // if (btn)
  // {
  uint8_t keycode[6] = {0};

  keycode[0] = btn;

  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode);
}

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if (!tud_hid_ready())
    return;

  uint8_t keycode[6] = {0};
  keycode[0] = btn;
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
}

static uint8_t const keycode2ascii[128][2] = {HID_KEYCODE_TO_ASCII};

bool is_modifier(uint8_t code)
{
  return 0;
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc
// ..) tud_hid_report_complete_cb() is used to send the next report after
// previous one is complete
uint8_t previous_btn = 0xff;
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return; // not enough time

  start_ms += interval_ms;

  const uint32_t current_time = timer_read_fast();
  bool registered = false;

  bool shift_pressed = false;
  bool ctrl_pressed = false;
  bool gui_pressed = false;
  bool alt_pressed = false;

  for (int i = 0; i < NUM_KEYS; i++)
  {
    if ((matrix_bank_status[i].is_pressed))
    {
      switch (keymaps_layers[0][i])
      {
      case HID_KEY_SHIFT_LEFT:
      case HID_KEY_SHIFT_RIGHT:
        shift_pressed = true;
        break;
      case HID_KEY_CONTROL_LEFT:
      case HID_KEY_CONTROL_RIGHT:
        ctrl_pressed = true;
        break;
      case HID_KEY_GUI_LEFT:
      case HID_KEY_GUI_RIGHT:
        gui_pressed = true;
        break;
      case HID_KEY_ALT_LEFT:
      case HID_KEY_ALT_RIGHT:
        alt_pressed = true;
        break;
      default:
        break;
      }
    }
  }

  if ((matrix_bank_status[LAYER_MOD_KEY].is_pressed) && (current_time > (matrix_bank_status[LAYER_MOD_KEY].last_handled_time + 2 * TAPHOLD_TIMEOUT)))
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
  }

  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    if ((matrix_bank_status[i].is_pressed))
    {
      if ((current_time > (matrix_bank_status[i].last_handled_time + TAPHOLD_TIMEOUT)))
      {
        const uint8_t key = keymaps_layers[get_layer()][i];
        if (
            (tud_ready()) && !((key >= HID_KEY_CONTROL_LEFT) && (key <= HID_KEY_GUI_RIGHT)))
        {
          send_hid_report_mod(REPORT_ID_KEYBOARD, ((shift_pressed * KEYBOARD_MODIFIER_LEFTSHIFT) | (ctrl_pressed * KEYBOARD_MODIFIER_LEFTCTRL) | (alt_pressed * KEYBOARD_MODIFIER_LEFTALT) | (gui_pressed * KEYBOARD_MODIFIER_LEFTGUI)), key);
          tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
        uart_putc(UART_ID, key);
        matrix_bank_status[i].last_handled_time = current_time;
        // previous_btn = buttons_queue;
        registered = true;
      }
    }
    uint8_t val = matrix_bank_status[i].is_pressed;
    // put_pixel(pio, sm, urgb_u32(0b100 * (val * 4) + 0b11, 0b100 * val + 0b11, 0b100 * val + 0b11));
  }

  for (uint8_t i = 0; i < key_queue.size; i++)
  {
    const uint8_t key = key_queue.ch[i];
    if (!((key >= HID_KEY_CONTROL_LEFT) && (key <= HID_KEY_GUI_RIGHT)))
      send_hid_report_mod(REPORT_ID_KEYBOARD, shift_pressed ? (1 << 1) : 0, key);

    registered = true;
  }
  key_queue.size = 0;

  // if (registered){
  if (tud_hid_ready())
  {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
  }

  // Remote wakeup
  //} else {
  //  // Send the 1st of report chain, the rest will be sent by
  //  // tud_hid_report_complete_cb()
  //  send_hid_report(REPORT_ID_KEYBOARD, buttons_queue);
  //}
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report,
                                uint16_t len)
{
  (void)instance;
  (void)len;

  uint8_t next_report_id = report[0] + 1u;

  if (next_report_id < REPORT_ID_COUNT)
  {
    // send_hid_report(next_report_id, board_button_read());
  }
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
