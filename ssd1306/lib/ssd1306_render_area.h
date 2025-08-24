#ifndef SSD1306_RENDER_AREA_h
#define SSD1306_RENDER_AREA_h

#include <stdint.h>

struct render_area
{
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
};

void calc_render_area_buflen(struct render_area *area);

#endif