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

#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "structs.h"

#include "keyboard_config.h"
#include "firmware_timer.h"
#include "matrix.h"
#include "board.h"
#include "debounce.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define IS_RGBW false

#define KEYBOARD_BACKLIGHT_PIN 42
#define NUM_PIXELS 36

const uint64_t SWITCH_MASK = ((uint64_t)(1) << A1) | ((uint64_t)(1) << A2) | ((uint64_t)(1) << A3) | ((uint64_t)(1) << A4) | ((uint64_t)(1) << A5) | ((uint64_t)(1) << B1) | ((uint64_t)(1) << B2) | ((uint64_t)(1) << B3) | ((uint64_t)(1) << B4) | ((uint64_t)(1) << B5) | ((uint64_t)(1) << C1) | ((uint64_t)(1) << C2) | ((uint64_t)(1) << C3) | ((uint64_t)(1) << C4) | ((uint64_t)(1) << C5) | ((uint64_t)(1) << D1) | ((uint64_t)(1) << D2) | ((uint64_t)(1) << D3) | ((uint64_t)(1) << D4) | ((uint64_t)(1) << D5) | ((uint64_t)(1) << E1) | ((uint64_t)(1) << E2) | ((uint64_t)(1) << E3) | ((uint64_t)(1) << E4) | ((uint64_t)(1) << E5) | ((uint64_t)(1) << F1) | ((uint64_t)(1) << F2) | ((uint64_t)(1) << F3) | ((uint64_t)(1) << F4) | ((uint64_t)(1) << F5) | ((uint64_t)(1) << T1) | ((uint64_t)(1) << T2) | ((uint64_t)(1) << T3) | ((uint64_t)(1) << T4) | ((uint64_t)(1) << T5) | ((uint64_t)(1) << T6);

static inline void
put_pixel(PIO pio, uint sm, uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         (uint32_t)(b);
}

static inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  return ((uint32_t)(r) << 8) |
         ((uint32_t)(g) << 16) |
         ((uint32_t)(w) << 24) |
         (uint32_t)(b);
}

uint8_t buttons_queue = 0;
void isr_handler(uint gpio, uint32_t events)
{
  // Put the GPIO event(s) that just happened into event_str
  // so we can print it
  if (gpio <= T6)
  {
    buttons_queue = 'A' + gpio - A1;
  }
  else
  {
    buttons_queue = 0;
  }
}

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

#define SETUP_GPIO(x)       \
  gpio_init(x);             \
  gpio_set_dir(x, GPIO_IN); \
  gpio_pull_up(x);
  // gpio_put(x, 1);
// matrix_row_t raw_matrix[MATRIX_ROWS];
// matrix_row_t matrix[MATRIX_ROWS];

/*------------- MAIN -------------*/
int main(void)
{
  // board_init();

  stdio_init_all();
  setup_board();

  // gpio_set_irq_enabled_with_callback(
  //     BUTTON_DOWN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &isr_handler);
  // gpio_set_irq_enabled(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true);
  // gpio_set_irq_enabled(BUTTON_RIGHT, GPIO_IRQ_EDGE_FALL, true);
  // gpio_set_irq_enabled(BUTTON_LEFT, GPIO_IRQ_EDGE_FALL, true);

  matrix_init();
  debounce_init(ROWS_PER_HAND);

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb)
  {
    board_init_after_tusb();
  }

  PIO pio;
  uint sm;
  uint offset;

  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, KEYBOARD_BACKLIGHT_PIN, 1, true);
  hard_assert(success);
  ws2812_program_init(pio, sm, offset, KEYBOARD_BACKLIGHT_PIN, 800000, IS_RGBW);
  uint8_t p =0;
  while (1)
  {
    bool changed = true;
    // changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);

    matrix_scan_kb();
    tud_task(); // tinyusb device task
    led_blinking_task();
    uint64_t input_status = gpio_get_all64() & SWITCH_MASK;

    buttons_queue = 0;

    if (input_status)
      for (int i = A1; i <= T6; i++)
      {
        if (!((input_status >> i) & 1))
        {
          buttons_queue = 'A' + i - A1;
        }
      }

    for (size_t i = A1; i <= T6; i++)
    {
      uint8_t val = (((input_status >> i) & 1) + 1) & 1;
      put_pixel(pio, sm, urgb_u32((val)*p + 0b100, 0b100, 0b100));
    }
    p++;
    sleep_ms(10);

    hid_task();
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

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if (!tud_hid_ready())
    return;

  switch (report_id)
  {
  case REPORT_ID_KEYBOARD:
  {
    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;

    if (btn && !has_keyboard_key)
    {
      uint8_t keycode[6] = {0};

      if (btn <= 'A' + T6 - A1)
      {
        keycode[0] = HID_KEY_A + btn - 'A';
      }
      else
      {
        keycode[0] = HID_KEY_Z;
      }

      buttons_queue = 0;
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
      has_keyboard_key = true;
    }
    else
    {
      if (has_keyboard_key)
      {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      }
      has_keyboard_key = false;
    }

    // if (btn) {
    //   uint8_t keycode[6] = {0};
    //   keycode[0] = HID_KEY_A;

    //  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
    //  has_keyboard_key = true;
    //} else {
    //  // send empty key report if previously has key pressed
    //  if (has_keyboard_key) {
    //    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    //  }
    //  has_keyboard_key = false;
    //}
  }
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
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return; // not enough time
  start_ms += interval_ms;

  // uint32_t const btn = board_button_read();
  // uint8_t buttons_queue = 0;
  // if (!gpio_get(BUTTON_UP)) {
  //  buttons_queue = 'A';
  //} else if (!gpio_get(BUTTON_DOWN)) {
  //  buttons_queue = 'B';
  //} else if (!gpio_get(BUTTON_LEFT)) {
  //  buttons_queue = 'C';
  //} else if (!gpio_get(BUTTON_RIGHT)) {
  //  buttons_queue = 'D';
  //}
  //

  send_hid_report(REPORT_ID_KEYBOARD, buttons_queue);

  // Remote wakeup
  // if (tud_suspended() && buttons_queue) {
  //  // Wake up host if we are in suspend mode
  //  // and REMOTE_WAKEUP feature is enabled by host
  //  tud_remote_wakeup();
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

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }
      else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms)
    return;

  // Blink every interval ms
  if (board_millis() - start_ms < blink_interval_ms)
    return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
