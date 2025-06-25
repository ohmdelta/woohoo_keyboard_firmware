#include "keyboard_config.h"
#include "matrix.h"
#include "structs.h"
#include "debounce.h"
#include <stdlib.h>
#include <stdbool.h>

#include "hardware/gpio.h"

uint64_t raw_matrix = 0;
uint64_t matrix = 0;

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
  key_status = gpio_get_all64() & MATRIX_MASK;

  // debounce(matrix_debouncing, matrix, matrix_rows(), changed);
  changed = debounce(raw_matrix, matrix, changed);
  matrix_scan_kb();

  return (uint8_t)changed;
}

bool matrix_task(void)
{
  bool matrix_changed = false;
  // if (!matrix_can_read()) {
  //     generate_tick_event();
  //     return false;
  // }

  // static matrix_row_t matrix_previous[MATRIX_ROWS];

  matrix_scan();
  // bool matrix_changed = false;
  // for (uint8_t row = 0; row < MATRIX_ROWS && !matrix_changed; row++) {
  //     matrix_changed |= matrix_previous[row] ^ matrix_get_row(row);
  // }

  // matrix_scan_perf_task();

  // // Short-circuit the complete matrix processing if it is not necessary
  // if (!matrix_changed) {
  //     generate_tick_event();
  //     return matrix_changed;
  // }

  // if (debug_config.matrix) {
  //     matrix_print();
  // }

  // const bool process_keypress = should_process_keypress();

  // for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
  //     const matrix_row_t current_row = matrix_get_row(row);
  //     const matrix_row_t row_changes = current_row ^ matrix_previous[row];

  //     if (!row_changes || has_ghost_in_row(row, current_row)) {
  //         continue;
  //     }

  //     matrix_row_t col_mask = 1;
  //     for (uint8_t col = 0; col < MATRIX_COLS; col++, col_mask <<= 1) {
  //         if (row_changes & col_mask) {
  //             const bool key_pressed = current_row & col_mask;

  //             if (process_keypress) {
  //                 action_exec(MAKE_KEYEVENT(row, col, key_pressed));
  //             }

  //             switch_events(row, col, key_pressed);
  //         }
  //     }

  //     matrix_previous[row] = current_row;
  // }

  return matrix_changed;
}