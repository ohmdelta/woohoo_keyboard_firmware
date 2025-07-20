#include "firmware_timer.h"
#include "hardware/timer.h"

inline fast_timer_t timer_read_fast()
{
    return (time_us_64());
}
