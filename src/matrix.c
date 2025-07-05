#include "keyboard_config.h"
#include "firmware_timer.h"
#include "matrix.h"
#include "structs.h"
#include "debounce.h"
#include <stdlib.h>
#include <stdbool.h>

#include "hardware/gpio.h"
#include "matrix_status.h"

#include <limits.h>

uint64_t raw_matrix = 0;
uint64_t matrix = 0;

/**
 * @brief Generates a tick event at a maximum rate of 1KHz that drives the
 * internal QMK state machine.
 */
static inline void generate_tick_event(void) {
    static uint32_t last_tick = 0;
    const uint32_t  now       = timer_read_fast();
    if (TIMER_DIFF_32(now, last_tick) != 0) {
        // action_exec(MAKE_TICK_EVENT);
        last_tick = now;
    }
}

void matrix_scan_kb(void)
{
}

void matrix_init_kb() {}

void matrix_init(void)
{
  /* Column output pins */
  matrix_init_kb();
}

uint64_t key_status = 0;

uint8_t matrix_scan(void)
{
  bool changed = false;
  key_status = ~gpio_get_all64() & SWITCH_MASK;

  // debounce(matrix_debouncing, matrix, matrix_rows(), changed);
  // changed = debounce(raw_matrix, matrix, changed);
  matrix_scan_kb();

  return (uint8_t)changed;
}

uint64_t changes = 0;

uint64_t get_keys(void)
{
  return key_status;
}

matrix_status_t matrix_bank_status[NUM_KEYS] = {0};

uint64_t matrix_previous = 0;
bool matrix_task(void)
{
  changes = 0;
  // if (!matrix_can_read()) {
  //     generate_tick_event();
  //     return false;
  // }

  matrix_scan();
  const uint32_t now = timer_read_fast();

  // const bool process_keypress = should_process_keypress();

  const uint64_t current = key_status;
  changes = (current ^ matrix_previous) & SWITCH_MASK;

  // uint8_t button = 0;
  if (changes) {
    for (int i = 0; i< NUM_KEYS; i++)
    {
      matrix_bank_status[i].is_pressed = (changes >> (i + A1)) & 1;
      matrix_bank_status[i].last_update_time = now;
    }
      // for(int i=0; i<64; i++)
      // {
      //   if ((changes >> i) & 1)
      //   {
      //     // action_exec(MAKE_KEYEVENT(row, col, key_pressed));
      //     button = i;
      //   }
      // }

      // switch_events(row, col, key_pressed);
    matrix_previous = current;
  }

  return changes;
}