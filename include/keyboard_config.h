#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

// TEST
#define BUTTON_UP (0)
#define BUTTON_DOWN (3)
#define BUTTON_LEFT (2)
#define BUTTON_RIGHT (1)
//

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

#define MATRIX_MASK ( \
    (1 << A1) |       \
    (1 << A2) |       \
    (1 << A3) |       \
    (1 << A4) |       \
    (1 << A5) |       \
    (1 << B1) |       \
    (1 << B2) |       \
    (1 << B3) |       \
    (1 << B4) |       \
    (1 << B5) |       \
    (1 << C1) |       \
    (1 << C2) |       \
    (1 << C3) |       \
    (1 << C4) |       \
    (1 << C5) |       \
    (1 << D1) |       \
    (1 << D2) |       \
    (1 << D3) |       \
    (1 << D4) |       \
    (1 << D5) |       \
    (1 << E1) |       \
    (1 << E2) |       \
    (1 << E3) |       \
    (1 << E4) |       \
    (1 << E5) |       \
    (1 << F1) |       \
    (1 << F2) |       \
    (1 << F3) |       \
    (1 << F4) |       \
    (1 << F5) |       \
    (1 << T1) |       \
    (1 << T2) |       \
    (1 << T3) |       \
    (1 << T4) |       \
    (1 << T5) |       \
    (1 << T6))

#endif
