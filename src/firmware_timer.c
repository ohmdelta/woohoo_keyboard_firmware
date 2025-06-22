#include "firmware_timer.h"
#include "hardware/timer.h"

fast_timer_t inline timer_read_fast() {
    return time_us_32();
}
