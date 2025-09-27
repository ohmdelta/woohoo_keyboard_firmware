#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t matrix_row_t;
typedef uint64_t fast_timer_t;

typedef struct
{
    bool caps_lock : 1;
    bool num_lock : 1;
    bool scroll_lock : 1;
} indicator_state_t;

typedef struct
{
    uint8_t ch;
    bool done;
} other_board_t;

#endif // !DEBUG STRUCTS_H
