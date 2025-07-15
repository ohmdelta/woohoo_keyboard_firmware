#ifndef LED_H
#define LED_H

#include "matrix.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_TIMEOUT 10000

void led_cycle_pattern();

void brightness_update(int8_t val);

void run_pattern(PIO pio, uint sm, uint len);

inline uint32_t white_offset_urgb_u32(uint8_t w, uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r + w) << 8) |
           ((uint32_t)(g + w) << 16) |
           (uint32_t)(b + w);
}

inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

typedef void (*pattern)(PIO pio, uint sm, uint len, uint t);
#endif