#include "ui.h"
#include "tusb.h"


void render_main_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count)
{
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
}
