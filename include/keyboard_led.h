#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_TIMEOUT 20000

void led_cycle_pattern();

void run_pattern(PIO pio, uint sm, uint len);

inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

typedef void (*pattern)(PIO pio, uint sm, uint len, uint t);
#endif