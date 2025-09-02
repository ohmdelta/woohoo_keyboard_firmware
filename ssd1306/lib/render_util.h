#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include "ssd1306_i2c.h"
#include <stdint.h>

static inline void render_time(uint8_t *buf, int16_t x, int16_t y, uint64_t time_us)
{
    char text[] = "00:00:00";
    uint64_t ms = time_us / 1000;
    uint64_t s = ms / 1000;
    uint8_t _s = s % 60;

    uint64_t mins = s / 60;
    uint8_t _mins = mins % 60;

    uint64_t hours = mins / 60;
    uint8_t _hours = hours % 100;

    text[0] = (_hours / 10 % 10) + '0';
    text[1] = (_hours % 10) + '0';

    text[3] = (_mins / 10 % 10) + '0';
    text[4] = (_mins % 10) + '0';

    text[6] = (_s / 10 % 10) + '0';
    text[7] = (_s % 10) + '0';

    write_string_vertical(buf, x, y, text);
}


#endif