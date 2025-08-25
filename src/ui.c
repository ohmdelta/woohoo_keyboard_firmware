#include "ssd1306.h"
#include "ui.h"
#include "tusb.h"

char const main_options[][9] = {
    " TIMER",
    " LED",
};

const uint8_t num_main_options = sizeof(main_options) / sizeof(main_options[0]);

void render_main_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count)
{
    static int8_t state = 0;
    state = (ccw_count - cw_count) % num_main_options;
    if (state < 0)
    {
        state += num_main_options;
    }

    render_time(buf, 120, 0, time);

    if (tud_mounted())
    {
        write_string_vertical(buf, 112, 0, "MOUNTED");
    }
    else
    {
        write_string_vertical(buf, 112, 0, "N/A CONN");
    }

    char ccw[SSD1306_NUM_PAGES];
    snprintf(ccw, SSD1306_NUM_PAGES, "CCW:%d", ccw_count);
    char cc[SSD1306_NUM_PAGES];
    snprintf(cc, SSD1306_NUM_PAGES, "CC:%d", cw_count);

    write_string_vertical(buf, 104, 0, ccw);
    write_string_vertical(buf, 96, 0, cc);

    for (uint8_t i = 0; i < num_main_options; i++)
    {
        write_string_vertical(buf, 2 + 12 * i, 0, main_options[i]);
    }

    {
        uint8_t ax = 12 * state;
        uint8_t bx = 12 + ax;

        draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
        draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

        draw_line(buf, ax, 0, bx, 0, 1);
        draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
    }
}
