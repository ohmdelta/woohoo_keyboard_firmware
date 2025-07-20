/*
Copyright 2017 Alex Ong<the.onga@gmail.com>
Copyright 2020 Andrei Purdea<andrei@purdea.ro>
Copyright 2021 Simon Arlott
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Basic symmetric per-key algorithm. Uses an 8-bit counter per key.
When no state changes have occured for DEBOUNCE milliseconds, we push the state.
*/

#include "debounce.h"
#include "firmware_timer.h"
#include <stdlib.h>

// #define UINT8_MAX 255

#ifndef DEBOUNCE
#define DEBOUNCE 5
#endif

// Maximum debounce: 255ms
#if DEBOUNCE > UINT8_MAX
#undef DEBOUNCE
#define DEBOUNCE UINT8_MAX
#endif

#define ROW_SHIFTER ((matrix_row_t)1)

typedef uint8_t debounce_counter_t;

#if DEBOUNCE > 0
static debounce_counter_t *debounce_counters;
static fast_timer_t last_time;
static bool counters_need_update;
static bool cooked_changed;

#define DEBOUNCE_ELAPSED 0

static void update_debounce_counters_and_transfer_if_expired(
    uint64_t *raw, uint64_t* cooked,
    uint8_t elapsed_time);
static void start_debounce_counters(uint64_t raw, uint64_t cooked);

// we use num_rows rather than MATRIX_ROWS to support split keyboards
void debounce_init(uint8_t num_rows)
{
  debounce_counters = (debounce_counter_t *)calloc(num_rows * MATRIX_COLS,
                                                   sizeof(debounce_counter_t));
}

void debounce_free(void)
{
  free(debounce_counters);
  debounce_counters = NULL;
}

bool debounce(uint64_t* raw, uint64_t* cooked,
              bool changed)
{
  bool updated_last = false;
  cooked_changed = false;

  if (counters_need_update)
  {
    fast_timer_t now = timer_read_fast();
    fast_timer_t elapsed_time = TIMER_DIFF_FAST(now, last_time);

    last_time = now;
    updated_last = true;
    if (elapsed_time > UINT8_MAX)
    {
      elapsed_time = UINT8_MAX;
    }

    if (elapsed_time > 0)
    {
      update_debounce_counters_and_transfer_if_expired(raw, cooked,
                                                       elapsed_time);
    }
  }

  if (changed)
  {
    if (!updated_last)
    {
      last_time = timer_read_fast();
    }

    start_debounce_counters(*raw, *cooked);
  }

  return cooked_changed;
}

static void update_debounce_counters_and_transfer_if_expired(uint64_t *raw, uint64_t *cooked, uint8_t elapsed_time)
{
  counters_need_update = false;
  debounce_counter_t *debounce_pointer = debounce_counters;
  if (*debounce_pointer != DEBOUNCE_ELAPSED)
  {
    if (*debounce_pointer <= elapsed_time)
    {
      *debounce_pointer = DEBOUNCE_ELAPSED;
      uint64_t cooked_next = *cooked | *raw;
      cooked_changed |= *cooked ^ cooked_next;
      *cooked = cooked_next;
    }
    else
    {
      *debounce_pointer -= elapsed_time;
      counters_need_update = true;
    }
  }
    debounce_pointer++;
}

static void start_debounce_counters(uint64_t raw, uint64_t cooked)
{
  debounce_counter_t *debounce_pointer = debounce_counters;
  matrix_row_t delta = raw ^ cooked;
  for (uint8_t col = 0; col < MATRIX_COLS; col++)
  {
    if (delta & (ROW_SHIFTER << col))
    {
      if (*debounce_pointer == DEBOUNCE_ELAPSED)
      {
        *debounce_pointer = DEBOUNCE;
        counters_need_update = true;
      }
    }
    else
    {
      *debounce_pointer = DEBOUNCE_ELAPSED;
    }
    debounce_pointer++;
  }
}

#else
#include "none.c"
#endif
