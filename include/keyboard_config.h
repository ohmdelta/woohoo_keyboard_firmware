#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

#include <stdint.h>

#define A1 4
#define A2 5
#define A3 6
#define A4 7
#define A5 8

#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13

#define C1 14
#define C2 15
#define C3 16
#define C4 17
#define C5 18

#define D1 19
#define D2 20
#define D3 21
#define D4 22
#define D5 23

#define E1 24
#define E2 25
#define E3 26
#define E4 27
#define E5 28

#define F1 29
#define F2 30
#define F3 31
#define F4 32
#define F5 33

// THUMB CLUSTER
#define T1 34
#define T2 35
#define T3 36
#define T4 37
#define T5 38
#define T6 39

#define NUM_KEYS ((5 * 6) + 6)
#define MATRIX_COLS 6
#define MATRIX_ROWS 5

#define ROWS_PER_HAND (MATRIX_ROWS)

#define SWITCH_MASK (         \
    (((uint64_t)(1)) << A1) | \
    (((uint64_t)(1)) << A2) | \
    (((uint64_t)(1)) << A3) | \
    (((uint64_t)(1)) << A4) | \
    (((uint64_t)(1)) << A5) | \
    (((uint64_t)(1)) << B1) | \
    (((uint64_t)(1)) << B2) | \
    (((uint64_t)(1)) << B3) | \
    (((uint64_t)(1)) << B4) | \
    (((uint64_t)(1)) << B5) | \
    (((uint64_t)(1)) << C1) | \
    (((uint64_t)(1)) << C2) | \
    (((uint64_t)(1)) << C3) | \
    (((uint64_t)(1)) << C4) | \
    (((uint64_t)(1)) << C5) | \
    (((uint64_t)(1)) << D1) | \
    (((uint64_t)(1)) << D2) | \
    (((uint64_t)(1)) << D3) | \
    (((uint64_t)(1)) << D4) | \
    (((uint64_t)(1)) << D5) | \
    (((uint64_t)(1)) << E1) | \
    (((uint64_t)(1)) << E2) | \
    (((uint64_t)(1)) << E3) | \
    (((uint64_t)(1)) << E4) | \
    (((uint64_t)(1)) << E5) | \
    (((uint64_t)(1)) << F1) | \
    (((uint64_t)(1)) << F2) | \
    (((uint64_t)(1)) << F3) | \
    (((uint64_t)(1)) << F4) | \
    (((uint64_t)(1)) << F5) | \
    (((uint64_t)(1)) << T1) | \
    (((uint64_t)(1)) << T2) | \
    (((uint64_t)(1)) << T3) | \
    (((uint64_t)(1)) << T4) | \
    (((uint64_t)(1)) << T5) | \
    (((uint64_t)(1)) << T6))

#define KEYBOARD_BACKLIGHT_PIN 42
#define INDICATOR_LEDS_PIN 1

#define LEFT 0
#define RIGHT 1

#ifndef KEYBOARD_SIDE
#define KEYBOARD_SIDE 0
#endif

#if KEYBOARD_SIDE == LEFT
#define UART_TX_PIN 2
#define UART_RX_PIN 3
#else
#define UART_TX_PIN 3
#define UART_RX_PIN 2
#endif

// UART CONFIG
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define PICO_UART_ENABLE_CRLF_SUPPORT 0

#define INITIAL_TAPHOLD_TIMEOUT 270000
#define TAPHOLD_TIMEOUT  9000

#define ENCODER_BUTTON 45
#define ENCODER_A 46
#define ENCODER_B 47

#define KEYBOARD_LAYERS 3
#define LAYER_MODIFIER_KEY F5

#define SCREEN_SDA 2
#define SCREEN_SCL 3

#define SSD1306_I2C_CLK             1000

// #define PICO_DEFAULT_I2C 1
#define SSD1306_DEFAULT_I2C_SDA_PIN 40
#define SSD1306_DEFAULT_I2C_SCL_PIN 41

#endif
