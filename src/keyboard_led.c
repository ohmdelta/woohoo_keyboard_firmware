#include <stdio.h>
#include <stdlib.h>

#include "firmware_timer.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include "keyboard_led.h"

void pattern_snakes(PIO pio, uint sm, uint len, uint t);
void pattern_random(PIO pio, uint sm, uint len, uint t);
void pattern_sparkle(PIO pio, uint sm, uint len, uint t);
void pattern_greys(PIO pio, uint sm, uint len, uint t);
void pattern_dull(PIO pio, uint sm, uint len, uint t);

const struct
{
    pattern pat;
    const char *name;
} pattern_table[] = {
    {pattern_snakes, "Snakes!"},
    {pattern_random, "Random data"},
    {pattern_sparkle, "Sparkles"},
    {pattern_greys, "Greys"},
    {pattern_dull, "Dull"},
};


static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

inline uint32_t urgbw_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           ((uint32_t)(w) << 24) |
           (uint32_t)(b);
}

uint8_t curr_pattern = 0;

void led_cycle_pattern()
{
    curr_pattern = (curr_pattern + 1) % count_of(pattern_table);
}

void run_pattern(PIO pio, uint sm, uint len)
{
    static uint32_t last_time = 0;
    uint32_t current_time = timer_read_fast();

    static int t = 0;
    if (current_time > last_time + LED_TIMEOUT)
    {
        pattern_table[curr_pattern].pat(pio, sm, len, t);
        last_time = current_time;
        t++;
    }
}

void pattern_snakes(PIO pio, uint sm, uint len, uint t)
{
    for (uint i = 0; i < len; ++i)
    {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(pio, sm, urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(pio, sm, urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(pio, sm, urgb_u32(0, 0, 0xff));
        else
            put_pixel(pio, sm, 0);
    }
}

void pattern_random(PIO pio, uint sm, uint len, uint t)
{
    if (t % 8)
        return;
    for (uint i = 0; i < len; ++i)
        put_pixel(pio, sm, rand());
}

void pattern_sparkle(PIO pio, uint sm, uint len, uint t)
{
    if (t % 8)
        return;
    for (uint i = 0; i < len; ++i)
        put_pixel(pio, sm, rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(PIO pio, uint sm, uint len, uint t)
{
    uint max = 100; // let's not draw too much current!
    t %= max;
    for (uint i = 0; i < len; ++i)
    {
        put_pixel(pio, sm, t * 0x10101);
        if (++t >= max)
            t = 0;
    }
}

void pattern_dull(PIO pio, uint sm, uint len, uint t){
    for (uint i = 0; i < len; ++i)
    {
        put_pixel(pio, sm, urgb_u32(0b10, 0b10, 0b10));
    }
}