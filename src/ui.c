#include "ssd1306.h"
#include "ui.h"
#include "keyboard_led.h"
#include "tusb.h"

// LED SCREEN
void render_led_screen(uint8_t *buf, ui_page_state_t *page_state);
void handle_led_screen(ui_page_state_t* page_state, ui_command_t* state);

void handle_led_brightness(ui_page_state_t *page_state, ui_command_t *state);
void render_led_brightness(uint8_t *buf, ui_page_state_t *page_state);

void handle_led_pattern_screen(ui_page_state_t* page_state, ui_command_t* state);
void render_led_pattern_screen(uint8_t *buf, ui_page_state_t *page_state);

const option_t main_options[] = {
    {" LED", LED_PAGE},
    {" TIMER", TIMER_PAGE},
};

const uint8_t num_main_options = sizeof(main_options) / sizeof(main_options[0]);

void render_ui(uint8_t *buf, ui_command_t *state)
{
    static main_page_state_t page_state = {
        .ui_page = {
            .page = MAIN_PAGE,
            .state = 0,
            .time = 0,
        },
        .ccw_count = 0,
        .cw_count = 0,
    };

    page_state.ui_page.time = state->time;

    switch (page_state.ui_page.page)
    {
    case MAIN_PAGE:
        handle_main_screen(&page_state, state);
        render_main_screen(buf, &page_state);
        break;
    case LED_PAGE:
        handle_led_screen((ui_page_state_t *)&page_state, state);
        render_led_screen(buf, (ui_page_state_t *)&page_state);
        break;
    case LED_PATTERN_PAGE:
        handle_led_pattern_screen((ui_page_state_t *)&page_state, state);
        render_led_pattern_screen(buf, (ui_page_state_t *)&page_state);
        break;
    case TIMER_PAGE:
        break;
    case LED_BRIGHTNESS_PAGE:
        handle_led_brightness((ui_page_state_t *)&page_state, state);
        render_led_brightness(buf, (ui_page_state_t *)&page_state);
        break;

    default:
        break;
    }
}

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

void handle_state(ui_page_state_t *page_state, ui_command_t *state, uint8_t num_options)
{
    int8_t ui_state = page_state->state + (state->ccw_count - state->cw_count);
    ui_state %= num_options;
    if (ui_state < 0)
    {
        ui_state += num_options;
    }

    page_state->state = ui_state;
}

void handle_main_screen(main_page_state_t *page_state, ui_command_t *state)
{
    handle_state((ui_page_state_t *)(page_state), state, num_main_options);

    page_state->ccw_count += state->ccw_count;
    page_state->cw_count += state->cw_count;

    if (state->encoder_pressed)
    {
        switch (page_state->ui_page.state + 1)
        {
        case LED_PAGE:
            page_state->ui_page.page = LED_PAGE;
            break;

        default:
            break;
        }
    }
}

enum LED_SCREEN_OPTIONS{
    LED_SCREEN_PATTERN = 0,
    LED_PAGE_BRIGHTNESS ,
};

char const led_screen_options[][9] = {
    [LED_SCREEN_PATTERN] = "PATTERN",
    [LED_PAGE_BRIGHTNESS] = "BRIGHT",
};
#define NUM_LED_SCREEN_OPTIONS (count_of(led_screen_options))

void render_led_screen(uint8_t *buf, ui_page_state_t *page_state)
{
    for (uint8_t i = 0; i < NUM_LED_SCREEN_OPTIONS; i++)
    {
        write_string_vertical(buf, 128 - 12 * (i + 1), 0, led_screen_options[i]);
    }

    {
        uint8_t state = page_state->state;
        uint8_t ax = 114 - 12 * state;
        uint8_t bx = 12 + ax;

        draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
        draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

        draw_line(buf, ax, 0, bx, 0, 1);
        draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
    }
}

void handle_led_screen(ui_page_state_t* page_state, ui_command_t* state)
{
    handle_state((ui_page_state_t *)(page_state), state, num_main_options);

    if (state->encoder_pressed)
    {
        switch (page_state->state)
        {
        case LED_SCREEN_PATTERN:
            page_state->page = LED_PATTERN_PAGE;
            break;
        case LED_PAGE_BRIGHTNESS:
            page_state->page = LED_BRIGHTNESS_PAGE;
            break;
        default:
            break;
        }
    }

}

char const led_options[][9] = {
    "NEUTRAL",
    "RIPPLE",
    "SNAKES",
    "RANDOM",
    "SPARKLE",
    "GREYS",
    "DULL",
};

const uint8_t num_led_options = sizeof(led_options) / sizeof(led_options[0]);

void render_led_pattern_screen(uint8_t *buf, ui_page_state_t *page_state)
{
    for (uint8_t i = 0; i < num_led_options; i++)
    {
        write_string_vertical(buf, 128 - 12 * (i + 1), 0, led_options[i]);
    }

    {
        uint8_t state = page_state->state;
        uint8_t ax = 114 - 12 * state;
        uint8_t bx = 12 + ax;

        draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
        draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

        draw_line(buf, ax, 0, bx, 0, 1);
        draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);
    }
}

void handle_led_pattern_screen(ui_page_state_t *page_state, ui_command_t *state)
{
    handle_state((ui_page_state_t *)(page_state), state, num_led_options);

    if (state->encoder_pressed)
    {
        led_set_pattern(page_state->state);
        page_state->page = MAIN_PAGE;
    }
}

void handle_led_brightness(ui_page_state_t *page_state, ui_command_t *state)
{
    brightness_update(state->ccw_count - state->cw_count);

    if (state->encoder_pressed)
    {
        page_state->page = LED_PAGE;
    }
}

void render_led_brightness(uint8_t *buf, ui_page_state_t *page_state)
{
    write_string_vertical(buf, 92, 0, "LED");

    uint8_t state = page_state->state;
    uint8_t ax = 64 - 12 * state;
    uint8_t bx = 12 + ax;

    draw_line(buf, ax, 0, ax, SSD1306_HEIGHT - 1, 1);
    draw_line(buf, bx, 0, bx, SSD1306_HEIGHT - 1, 1);

    draw_line(buf, ax, 0, bx, 0, 1);
    draw_line(buf, ax, SSD1306_HEIGHT - 1, bx, SSD1306_HEIGHT - 1, 1);

    uint8_t brightness = get_led_brightness();
    draw_solid_rectangle(buf, ax, 0, bx, brightness ? ((brightness * 8) - 1) : 0, 1);
}
