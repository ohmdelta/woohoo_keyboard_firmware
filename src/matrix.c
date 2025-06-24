#include "keyboard_config.h"
#include "structs.h"
#include "debounce.h"
#include <stdlib.h>
#include <stdbool.h>

matrix_row_t raw_matrix[MATRIX_ROWS];
matrix_row_t matrix[MATRIX_ROWS];

void matrix_scan_kb(void)
{
}

void matrix_init_kb() {}

void matrix_init(void)
{
  /* Column output pins */
  for (uint8_t i = 0; i < MATRIX_ROWS; i++)
  {
    matrix[i] = 0;
    raw_matrix[i] = 0;
  }

  matrix_init_kb();
}

uint8_t matrix_scan(void)
{
  bool changed = false;
  for (uint8_t col = 0; col < MATRIX_COLS; col++)
  {
    // // select_row(col);
    // wait_us(30);
    // matrix_row_t rows = read_cols();
    // for (uint8_t row = 0; row < matrix_rows(); row++)
    // {
    //   bool prev_bit = raw_matrix[row] & ((matrix_row_t)1 << col);
    //   bool curr_bit = rows & (1 << row);
    //   if ((changed |= prev_bit != curr_bit))
    //   {
    //     raw_matrix[row] ^= (matrix_row_t)1 << col;
    //   }
    // }
  }

  // debounce(matrix_debouncing, matrix, matrix_rows(), changed);
  changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);
  matrix_scan_kb();

  return (uint8_t)changed;
}

void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row)
{
  // Start with a clear matrix row
  matrix_row_t current_row_value = 0;

  // matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
  // for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1)
  // {
  //   pin_t pin = direct_pins[current_row][col_index];
  //   current_row_value |= readMatrixPin(pin) ? 0 : row_shifter;
  // }

  // Update the matrix
  current_matrix[current_row] = current_row_value;
}

static bool matrix_task(void)
{
  bool matrix_changed = false;
  // if (!matrix_can_read()) {
  //     generate_tick_event();
  //     return false;
  // }

  // static matrix_row_t matrix_previous[MATRIX_ROWS];

  // matrix_scan();
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