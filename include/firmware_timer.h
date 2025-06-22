#ifndef FIRMWARE_TIMER_H
#define FIRMWARE_TIMER_H

#include "structs.h"

fast_timer_t timer_read_fast();

#define TIMER_DIFF_32(a, b) (uint32_t)((a) - (b))
#define TIMER_DIFF_FAST(a, b) TIMER_DIFF_32(a, b)

// uint32_t timer_elapsed32(uint32_t last) {
//     return TIMER_DIFF_32(time_us_32(), last);
// }


#endif