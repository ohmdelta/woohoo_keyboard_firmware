#ifndef TIMER_H
#define TIMER_H

#include "structs.h"
#include "hardware/timer.h"

fast_timer_t timer_read_fast();

#define TIMER_DIFF_32(a, b) (uint32_t)((a) - (b))
#define TIMER_DIFF_FAST(a, b) TIMER_DIFF_32(a, b)

uint32_t timer_elapsed32(uint32_t last) {
    return TIMER_DIFF_32(time_us_32(), last);
}

fast_timer_t inline timer_read_fast(void) {
    return time_us_32();
}

#endif