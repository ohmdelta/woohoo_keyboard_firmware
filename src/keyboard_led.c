#include "keyboard_led.h"

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#include "firmware_timer.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"


void pattern_snakes(PIO pio, uint sm, uint len, uint t);
void pattern_random(PIO pio, uint sm, uint len, uint t);
void pattern_sparkle(PIO pio, uint sm, uint len, uint t);
void pattern_greys(PIO pio, uint sm, uint len, uint t);
void pattern_dull(PIO pio, uint sm, uint len, uint t);
void pattern_ripple(PIO pio, uint sm, uint len, uint t);
void pattern_neutral(PIO pio, uint sm, uint len, uint t);

const struct
{
    pattern pat;
    const char *name;
} pattern_table[] = {
    {pattern_neutral, "Neutral"},
    {pattern_ripple, "Ripple"},
    {pattern_snakes, "Snakes"},
    {pattern_random, "Random data"},
    {pattern_sparkle, "Sparkles"},
    {pattern_greys, "Greys"},
    {pattern_dull, "Dull"},
};

uint8_t brightess = 7;

void brightness_update(int8_t val) {
    int8_t v = brightess + val;
    if (v < 0) {
        v = 0;
    }
    if (v > 8) {
        v = 8;
    }
    brightess = v;
}

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

void led_set_pattern(uint8_t ind)
{
    curr_pattern = ind % count_of(pattern_table);
}

void run_pattern(PIO pio, uint sm, uint len)
{
    static fast_timer_t last_time = 0;
    fast_timer_t current_time = timer_read_fast();

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
            put_pixel(pio, sm, urgb_u32(0x1 << brightess, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(pio, sm, urgb_u32(0, 0x1 << brightess, 0));
        else if (x >= 30 && x < 40)
            put_pixel(pio, sm, urgb_u32(0, 0, 0x1 << brightess));
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
    (void)t;
    for (uint i = 0; i < len; ++i)
    {
        put_pixel(pio, sm, urgb_u32(0x2, 0x2, 0x2));
    }
}

#define KERNEL_SIZE 5
#define RIPPLE_STEPS 3
const uint8_t ripple_kernel[KERNEL_SIZE][KERNEL_SIZE] = {
    {0, 1, 2, 1, 0},
    {1, 3, 7, 3, 1},
    {2, 7, 11, 7, 2},
    {1, 3, 7, 3, 1},
    {0, 1, 2, 1, 0}};

const uint8_t translation_kernel[KERNEL_SIZE][KERNEL_SIZE] = {
    {-12, -7, -2, 3, 8},
    {-11, -6, -1, 4, 9},
    {-10, -5, 0, 5, 10},
    {-9, -4, 1, 6, 11},
    {-8, -3, 2, 7, 12}};

inline int8_t min(int8_t a, int8_t b) {
    return a < b ? a : b;
}

inline int8_t max(int8_t a, int8_t b) {
    return a > b ? a : b;
}

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_led_t;

void pattern_ripple(PIO pio, uint sm, uint len, uint t)
{
    (void)len;
    (void)t;
    static fast_timer_t last_time = 0;
    fast_timer_t current_time = timer_read_fast();

    if (current_time < (last_time + LED_TIMEOUT * 10))
        return;

    last_time = current_time;
    static uint8_t matrix_led_state[NUM_KEYS] = {0};

    rgb_led_t current_matrix_led_state[NUM_KEYS] = {0};

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        if (matrix_bank_status[i].is_pressed && (matrix_led_state[i] == 0))
        {
            matrix_led_state[i] = RIPPLE_STEPS;
        }
        else if (matrix_led_state[i] > 0)
        {
            matrix_led_state[i]--;
        }

        int8_t x = i / MATRIX_ROWS;
        int8_t y = i % MATRIX_ROWS;
        if (y >= MATRIX_COLS)
            continue;

        if (matrix_led_state[i] == 3)
        {
            current_matrix_led_state[i].r += 0x3;
        }
        else if (matrix_led_state[i] == 2)
        {
            for (int8_t a = -1; a < 2; a++)
            {
                for (int8_t b = -1; b < 2; b++)
                {
                    if ((a == 0) && (b == 0))
                        continue;

                    int8_t a_ = x + a;
                    int8_t b_ = y + b;
                    if ((a_ < 0) || (a_ >= MATRIX_COLS) || (b_ < 0) || (b_ >= MATRIX_ROWS))
                    {
                        continue;
                    }
                    int8_t offset = translation_kernel[b + 2][a + 2];
                    current_matrix_led_state[i + offset].g += ripple_kernel[b + 2][a + 2];
                }
            }
        }
        else if (matrix_led_state[i] == 1)
        {
            for (int8_t a = -2; a < 3; a++)
            {
                for (int8_t b = -2; b < 3; b++)
                {
                    if ((a >= -1) && (a <= 1) && (b >= -1) && (b <= 1))
                        continue;

                    int8_t a_ = x + a;
                    int8_t b_ = y + b;
                    if ((a_ < 0) || (a_ >= MATRIX_COLS) || (b_ < 0) || (b_ >= MATRIX_ROWS))
                    {
                        continue;
                    }
                    int8_t offset = translation_kernel[b + 2][a + 2];
                    current_matrix_led_state[i + offset].b += ripple_kernel[b + 2][a + 2];
                }
            }
        }
    }

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        put_pixel(pio, sm, urgb_u32(current_matrix_led_state[i].r, current_matrix_led_state[i].g, current_matrix_led_state[i].b));
    }
}

void pattern_neutral(PIO pio, uint sm, uint len, uint t)
{
    (void)len;
    (void)t;

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        if (matrix_bank_status[i].is_pressed)
        {
            put_pixel(pio, sm, white_offset_urgb_u32(0xff, 0x0, 0x0, 0x0));
        }
        else
        {
            put_pixel(pio, sm, urgb_u32(0x0, 0x1, 0x4));
        }
    }
}
