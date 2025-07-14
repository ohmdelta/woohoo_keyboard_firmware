#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "hardware/gpio.h"

#include "matrix.h"
#include "firmware_timer.h"
#include "structs.h"
#include "debounce.h"

#include "matrix_status.h"


uint64_t raw_matrix = 0;
uint64_t matrix = 0;

matrix_status matrix_bank_status[NUM_KEYS] = {0};

/**
 * @brief Generates a tick event at a maximum rate of 1KHz that drives the
 * internal QMK state machine.
 */
static inline void generate_tick_event(void) {
    static fast_timer_t last_tick = 0;
    const fast_timer_t  now       = timer_read_fast();
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
  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    matrix_bank_status[i].last_state = 0;
    matrix_bank_status[i].is_pressed = 0;
    matrix_bank_status[i].last_handled_time = 0;
    matrix_bank_status[i].last_update_time = 0;
  }
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

uint64_t matrix_previous = 0;
bool matrix_task(void)
{
  changes = 0;
  // if (!matrix_can_read()) {
  //     generate_tick_event();
  //     return false;
  // }

  matrix_scan();
  const fast_timer_t now = timer_read_fast();

  // const bool process_keypress = should_process_keypress();

  const uint64_t current = key_status;
  changes = (current ^ matrix_previous);

  // uint8_t button = 0;
  if (changes)
  {
    for (uint8_t i = A1; i <= T6; i++)
    {
      matrix_bank_status[i - A1].last_state = matrix_bank_status[i - A1].is_pressed;
      matrix_bank_status[i - A1].is_pressed = ((current >> (i)) & 1);
      matrix_bank_status[i - A1].last_update_time = now;
    }
  }

  matrix_previous = current;

  return changes;
}