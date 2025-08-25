#include "ssd1306.h"
#include "ui.h"
#include "tusb.h"

const option_t main_options[] = {
    {" LED", LED},
    {" TIMER", TIMER},
};

const uint8_t num_main_options = sizeof(main_options) / sizeof(main_options[0]);

void render_main_screen(uint8_t *buf, main_page_state_t *main_page_state)
{
    int8_t state = main_page_state->ui_page.state;

    render_time(buf, 120, 0, main_page_state->ui_page.time);

    if (tud_mounted())
    {
        write_string_vertical(buf, 112, 0, "MOUNTED");
    }
    else
    {
        write_string_vertical(buf, 112, 0, "N/A CONN");
    }

    char ccw[SSD1306_NUM_PAGES];
    snprintf(ccw, SSD1306_NUM_PAGES, "CCW:%d", main_page_state->ccw_count);
    char cc[SSD1306_NUM_PAGES];
    snprintf(cc, SSD1306_NUM_PAGES, "CC:%d", main_page_state->cw_count);

    write_string_vertical(buf, 104, 0, ccw);
    write_string_vertical(buf, 96, 0, cc);

    draw_line(buf, 95, 0, 95, SSD1306_HEIGHT - 1, 1);
    for (uint8_t i = 0; i < num_main_options; i++)
    {
        write_string_vertical(buf, 76 - 12 * i, 0, main_options[i].tag);
    }

    {
        uint8_t ax = 88 - 2 - 12 * state;
        uint8_t bx = ax - 12;

        draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
        draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

        draw_line(buf, ax, 0, bx, 0, 1);
        draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
    }
}

void handle_main_screen(main_page_state_t *page_state, ui_command_t *state)
{
    page_state->ccw_count += state->ccw_count;
    page_state->cw_count += state->cw_count;

    int8_t ui_state = page_state->ui_page.state + (state->ccw_count - state->cw_count);
    ui_state %= num_main_options;
    if (ui_state < 0)
    {
        ui_state += num_main_options;
    }

    page_state->ui_page.state = ui_state;

    if (state->encoder_pressed)
    {
        switch (page_state->ui_page.state)
        {
        case LED:
            page_state->ui_page.page = LED;
            break;

        default:
            break;
        }
    }
}

char const led_options[][9] = {
    "SNAKES",
    "RANDOM",
    "SPARKLE",
    "GREYS",
    "DULL",
    "RIPPLE",
    "NEUTRAL",
};

const uint8_t num_led_options = sizeof(led_options) / sizeof(led_options[0]);

void render_led_screen(uint8_t *buf, uint64_t time, uint8_t ccw_count, uint8_t cw_count)
{
    (void)time;
    for (uint8_t i = 0; i < num_led_options; i++)
    {
        write_string_vertical(buf, 128 - 12 * (i + 1), 0, led_options[i]);
    }

    {
        uint8_t state = (ccw_count - cw_count) % num_led_options;
        uint8_t ax = 12 * state;
        uint8_t bx = 12 + ax;

        draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
        draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

        draw_line(buf, ax, 0, bx, 0, 1);
        draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
    }
}