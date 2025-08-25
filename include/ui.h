#ifndef UI_H
#define UI_H

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
    MAIN = 0,
    LED,
    TIMER,
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
    uint8_t ccw_count;
    uint8_t cw_count;
} main_page_state_t;


typedef struct
{
    char tag[9];
    enum UIPage ui_page;
} option_t;

// MAIN SCREEN
void render_main_screen(uint8_t *buf, main_page_state_t *main_page_state);
void handle_main_screen(main_page_state_t* page_state, ui_command_t* state);

// LED SCREEN
void render_led_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count);

#endif