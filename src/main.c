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
#include <stdint.h>
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
#include "keyboard_mapping.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define IS_RGBW false

#define KEYBOARD_BACKLIGHT_PIN 42

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

  PIO pio;
  uint sm;
  uint offset;

  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, KEYBOARD_BACKLIGHT_PIN, 1, true);
  hard_assert(success);
  ws2812_program_init(pio, sm, offset, KEYBOARD_BACKLIGHT_PIN, 800000, IS_RGBW);

  for (size_t i = A1; i <= T6; i++)
  {
    uint8_t val = 0;
    put_pixel(pio, sm, urgb_u32(0b100 * val, 0b100 * val, 0b100 * val));
  }

  uint8_t p =0;
  while (1)
  {
    // changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);

    bool changed = matrix_task();
    uint64_t key = get_keys() & SWITCH_MASK;
    buttons_queue = 0;
    tud_task(); // tinyusb device task

    if(changed)
    {
      for (int i = A1; i <= T6; i++)
      {
        buttons_queue = (key >> i) & 1 ? i : buttons_queue;
      }

      for (size_t i = A1; i <= T6; i++)
      {
        // uint8_t val = (((buttons_queue >> i) & 1) + 1) & 1;
        uint8_t val = 1 ;
        if (!key) val = 0;
        put_pixel(pio, sm, urgb_u32(0b100 * (val + 4 * (buttons_queue == i)), 0b100 * val, 0b100 * val));
      }
    }
    // p++;
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

      if (btn != 0)
      {
        keycode[0] = keymaps_layers[0][btn - A1];
      }
      else
      {
        keycode[0] = HID_KEY_NONE;
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
