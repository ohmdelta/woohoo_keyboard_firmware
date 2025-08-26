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

void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, bool on);
void draw_solid_rectangle(uint8_t *buf, int x0, int y0, int x1, int y1, bool on);

#endif
