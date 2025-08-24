#ifndef UI_H
#define UI_H

#include <stdint.h>

#include "ssd1306.h"

void render_main_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count);

#endif