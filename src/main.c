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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define A1 (4)
#define A2 (5)
#define A3 (6)
#define A4 (7)
#define A5 (8)

#define B1 (9)
#define B2 (10)
#define B3 (11)
#define B4 (12)
#define B5 (13)

#define C1 (14)
#define C2 (15)
#define C3 (16)
#define C4 (17)
#define C5 (18)
#define D1 (19)
#define D2 (20)
#define D3 (21)
#define D4 (22)
#define D5 (23)

#define E1 (24)
#define E2 (25)
#define E3 (26)
#define E4 (27)
#define E5 (28)
#define F1 (29)
#define F2 (30)
#define F3 (31)
#define F4 (32)
#define F5 (33)

#define T1 (34)
#define T2 (35)
#define T3 (36)
#define T4 (37)
#define T5 (38)
#define T6 (39)

uint8_t buttons_queue = 0;
void isr_handler(uint gpio, uint32_t events) {
  // Put the GPIO event(s) that just happened into event_str
  // so we can print it
  if (gpio <= T6) {
    buttons_queue = 'A' + gpio - A1;
  }
  else{
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
  gpio_pull_up(x);\
  gpio_put(x, 1);

/*------------- MAIN -------------*/
int main(void)
{
  // board_init();

  stdio_init_all();
  SETUP_GPIO(A1);
  SETUP_GPIO(A2);
  SETUP_GPIO(A3);
  SETUP_GPIO(A4);
  SETUP_GPIO(A5);
  SETUP_GPIO(B1)
  SETUP_GPIO(B2)
  SETUP_GPIO(B3)
  SETUP_GPIO(B4)
  SETUP_GPIO(B5)
  SETUP_GPIO(C1)
  SETUP_GPIO(C2)
  SETUP_GPIO(C3)
  SETUP_GPIO(C4)
  SETUP_GPIO(C5)
  SETUP_GPIO(D1)
  SETUP_GPIO(D2)
  SETUP_GPIO(D3)
  SETUP_GPIO(D4)
  SETUP_GPIO(D5)
  SETUP_GPIO(E1)
  SETUP_GPIO(E2)
  SETUP_GPIO(E3)
  SETUP_GPIO(E4)
  SETUP_GPIO(E5)
  SETUP_GPIO(F1)
  SETUP_GPIO(F2)
  SETUP_GPIO(F3)
  SETUP_GPIO(F4)
  SETUP_GPIO(F5)
  SETUP_GPIO(T1)
  SETUP_GPIO(T2)
  SETUP_GPIO(T3)
  SETUP_GPIO(T4)
  SETUP_GPIO(T5)
  SETUP_GPIO(T6)

  gpio_set_irq_enabled_with_callback(
      A1, GPIO_IRQ_EDGE_RISE, true, &isr_handler);
  gpio_set_irq_enabled(A2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(A3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(A4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(A5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(B1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(B2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(B3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(B4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(B5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(C1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(C2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(C3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(C4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(C5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(D1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(D2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(D3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(D4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(D5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(E1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(E2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(E3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(E4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(E5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(F1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(F2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(F3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(F4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(F5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T1, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T2, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T3, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T4, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T5, GPIO_IRQ_LEVEL_LOW, true);
  gpio_set_irq_enabled(T6, GPIO_IRQ_LEVEL_LOW, true);

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb)
  {
    board_init_after_tusb();
  }

  while (1)
  {
    tud_task(); // tinyusb device task
    led_blinking_task();

    hid_task();
  }
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

      if (btn <= 'A' + T6 - A1) {
        keycode[0] = HID_KEY_A + btn - 'A';
      }
      else{
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
