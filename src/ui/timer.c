#include "ssd1306.h"
#include "ui/timer.h"
#include "ui/common.h"

enum timer_screen_states
{
    TIMER_SELECT = 0,
    TIMER_START,
    TIMER_PAUSE,
    TIMER_BACK,
};

char const timer_select_options[][9] = {
    [TIMER_START] = " START",
    [TIMER_PAUSE] = " PAUSE",
};
#define NUM_TIMER_SELECT_OPTIONS (count_of(timer_select_options))

void handle_timer_select_screen(main_page_state_t *page_state, ui_command_t *state)
{
    handle_state((ui_page_state_t *)(page_state), state, TIMER_BACK + 1);
    static uint64_t current_timer_count = 0;
    current_timer_count = MIN(current_timer_count, page_state->timer_val - page_state->ui_page.time);

    page_state->ccw_count += state->ccw_count;
    page_state->cw_count += state->cw_count;

    if (state->encoder_pressed)
    {
        switch (page_state->ui_page.state)
        {
        case TIMER_SELECT:
            break;
        case TIMER_START:
            break;
        case TIMER_PAUSE:
            break;
        case TIMER_BACK:
            page_state->ui_page.page = MAIN_PAGE;
            break;
        default:
            break;
        }
    }
}

void render_timer_select_screen(uint8_t *buf, main_page_state_t *page_state)
{
    write_string_vertical(buf, 80, 0, "00:00:00");
    for (uint8_t i = TIMER_START; i < NUM_TIMER_SELECT_OPTIONS; i++)
    {
        write_string_vertical(buf, 64 - 12 * (i + 1), 0, timer_select_options[i]);
    }

    render_back_button(buf);

    {
        uint8_t state = page_state->ui_page.state;
        uint8_t ax = 0;
        if (state == TIMER_SELECT)
        {
            ax = 78;
        }
        else if (state != TIMER_BACK)
        {
            ax = 50 - 12 * state;
        }
        uint8_t bx = 12 + ax;

        draw_divider_box(buf, ax, bx, 1);
    }
}