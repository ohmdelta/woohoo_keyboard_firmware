#include "ssd1306.h"
#include "ui/timer.h"
#include "ui/common.h"

enum timer_screen_states
{
    TIMER_SELECT_HOUR = 0,
    TIMER_SELECT_MIN,
    TIMER_SELECT_SEC,
    TIMER_START,
    TIMER_PAUSE,
    TIMER_BACK,
};

char const timer_select_options[][9] = {
    " START",
    " PAUSE",
};
#define NUM_TIMER_SELECT_OPTIONS (count_of(timer_select_options))

bool selected = false;

void handle_timer_select_screen(main_page_state_t *page_state, ui_command_t *state)
{
    handle_state(page_state->ui_page, state, TIMER_BACK + 1);
    static uint64_t current_timer_count = 0;
    current_timer_count = MIN(current_timer_count, page_state->timer_val - page_state->ui_page.time);

    page_state->ccw_count += state->ccw_count;
    page_state->cw_count += state->cw_count;

    if (state->encoder_pressed)
    {
        switch (page_state->ui_page.state)
        {
        case TIMER_SELECT_HOUR:
        case TIMER_SELECT_MIN:
        case TIMER_SELECT_SEC:
            selected = selected ? 0 : 1;
            break;
        case TIMER_START:
            break;
        case TIMER_PAUSE:
            break;
        case TIMER_BACK:
            switch_state(&(page_state->ui_page), MAIN_PAGE);
            break;
        default:
            break;
        }
    }
}

void render_timer_select_screen(uint8_t *buf, main_page_state_t *page_state)
{
    write_string_vertical(buf, 80, 0, "00:00:00");
    for (uint8_t i = 0; i < 2; i++)
    {
        write_string_vertical(buf, 64 - 12 * (i + 1), 0, timer_select_options[i]);
    }

    render_back_button(buf);

    {
        uint8_t state = page_state->ui_page.state;
        uint8_t ax = 0;
        uint8_t bx = 12;

        if (state <= TIMER_SELECT_SEC)
        {
            ax = 78;
            bx = 12 + ax;

            uint8_t ay = 0;
            uint8_t by;
            switch (state)
            {
            case TIMER_SELECT_HOUR:
                by = ay + 8 * 3 - 5;
                break;
            case TIMER_SELECT_MIN:
                ay = 8 * 3 - 5;
                by = SSD1306_HEIGHT - (8 * 3 - 3);
                break;
            case TIMER_SELECT_SEC:
                ay = SSD1306_HEIGHT - (8 * 3 - 3);
                by = SSD1306_HEIGHT - 1;
                break;
            default:
                break;
            }
            draw_rectangle(buf, ax, ay, bx, by, 1);
        }
        else if ((state > TIMER_SELECT_SEC) && (state != TIMER_BACK))
        {
            ax = 50 - 12 * (state - TIMER_SELECT_SEC - 1);
            bx = 12 + ax;
            draw_divider_box(buf, ax, bx, 1);
        }
        else {
            ax = 0;
            bx = 12;
            draw_divider_box(buf, ax, bx, 1);
        }
    }
}
