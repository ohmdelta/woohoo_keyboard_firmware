#include "ssd1306_i2c.h"

void calc_render_area_buflen(struct render_area *area)
{
    // calculate how long the flattened buffer will be for a render area
    area->buflen = (area->end_col - area->start_col + 1) * (area->end_page - area->start_page + 1);
}

#define SWAP(a, b) \
    {              \
        *a ^= *b;  \
        *b ^= *a;  \
        *a ^= *b;  \
    }

static void swap(uint8_t *a, uint8_t *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void draw_horizontal_divider(uint8_t *buf, int x0, bool on)
{
    for (uint8_t y=0; y < SSD1306_HEIGHT / 8; y++)
    {
        buf[y * SSD1306_WIDTH + x0] |= 0xff;
    }
}

void draw_horizontal_line(uint8_t *buf, int x0, int y0, int y1, bool on)
{
    if (y0 > y1)
    {
        SWAP(&y0, &y1);
    }
    y0 = MAX(0, y0);
    y1 = MIN(y1, SSD1306_HEIGHT - 1);

    for (uint8_t y = y0; y < y1; y++)
    {
        set_pixel(buf, x0, y, on);
    }
}

// Basic Bresenhams.
void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, bool on)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (true)
    {
        set_pixel(buf, x0, y0, on);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_solid_rectangle(uint8_t *buf, int x0, int y0, int x1, int y1, bool on)
{
    int dx = abs(x1 - x0);
    x0 = MIN(x0, x1);
    y0 = MIN(y0, y1);
    int dy = abs(y1 - y0);

    for (uint8_t x = 0; x < dx; x++)
    {
        for (uint8_t y = 0; y < dy; y++)
        {
            set_pixel(buf, x0 + x, y0 + y, on);
        }
    }
}

void draw_divider_box(uint8_t *buf, int ax, int bx, bool on)
{
    draw_horizontal_divider(buf, ax, 1);
    draw_horizontal_divider(buf, bx, 1);

    draw_line(buf, ax, 0, bx, 0, 1);
    draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
}
