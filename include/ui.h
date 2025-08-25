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

typedef struct
{
    uint8_t state;
    uint64_t time;
} ui_page_state_t;

void render_main_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count);

#endif