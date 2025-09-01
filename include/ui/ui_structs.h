#ifndef UI_STRUCTS_H
#define UI_STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool encoder_pressed;
    uint8_t ccw_count;
    uint8_t cw_count;
    uint64_t time;
} ui_command_t;

enum UIPage
{
    MAIN_PAGE = 0,
    LED_PAGE,
    TIMER_SELECT_PAGE,
    LED_BRIGHTNESS_PAGE,
    LED_PATTERN_PAGE,
};

typedef struct
{
    enum UIPage page;
    uint8_t state;
    uint64_t time;
} ui_page_state_t;

typedef struct
{
    ui_page_state_t ui_page;
    uint64_t timer_val;
    uint8_t ccw_count;
    uint8_t cw_count;
} main_page_state_t;

typedef struct
{
    char tag[9];
    enum UIPage ui_page;
} option_t;

#endif
