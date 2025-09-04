#include "ssd1306.h"
#include "ui/timer.h"
#include "ui/common.h"
#include <stdint.h>

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

typedef struct
{
    uint8_t hours : 7;
    uint8_t minutes : 6;
    uint8_t seconds : 6;
} timer_selector_t;

bool selected = false;

uint64_t offset_time = 0;

timer_selector_t time_to_selector(uint64_t time_us)
{
    uint64_t ms = time_us / 1000;
    uint64_t s = ms / 1000;
    uint8_t _s = s % 60;

    uint64_t mins = s / 60;
    uint8_t _mins = mins % 60;

    uint64_t hours = mins / 60;
    uint8_t _hours = hours % 100;

    return (timer_selector_t){
        .hours = _hours,
        .minutes = _mins,
        .seconds = _s,
    };
}

uint64_t selector_to_time(timer_selector_t timer_selector_vals)
{
    return ((uint64_t)(timer_selector_vals.seconds) + (60 * (uint64_t)timer_selector_vals.minutes) + (60 * 60 * (uint64_t)timer_selector_vals.hours)) * 1000000;
}

inline int8_t compute_offset(uint8_t val, int8_t offset, uint8_t bound)
{
    int8_t h = val + offset;
    h %= bound;
    if (h < 0)
    {
        h += bound;
    }
    return h;
}

uint64_t timer_end = 0;
bool timer_running = false;

void handle_timer_select_screen(main_page_state_t *page_state, ui_command_t *state)
{
    if (!selected)
    {
        handle_state(&(page_state->ui_page), state, TIMER_BACK + 1);
    }

    page_state->ccw_count += state->ccw_count;
    page_state->cw_count += state->cw_count;

    if (state->encoder_pressed)
    {
        switch (page_state->ui_page.state)
        {
            case TIMER_SELECT_HOUR:
            case TIMER_SELECT_MIN:
            case TIMER_SELECT_SEC:
                selected = !selected;
                break;
            case TIMER_START:
                {
                    if (!timer_running)
                    {
                        timer_end = state->time + offset_time;
                    }
                    timer_running = true;
                    break;
                }
            case TIMER_PAUSE:
                {
                    if (timer_running)
                    {
                        offset_time = timer_end - state->time;
                    }
                    timer_running = false;
                    break;
                }
            case TIMER_BACK:
                switch_state(&(page_state->ui_page), MAIN_PAGE);
                break;
            default:
                break;
        }
    }

    if (selected)
    {
        timer_selector_t selector = time_to_selector(offset_time);
        int8_t offset = (state->cw_count - state->ccw_count);

        switch (page_state->ui_page.state)
        {
            case TIMER_SELECT_HOUR:
                selector.hours = compute_offset(selector.hours, offset, 100);
                break;
            case TIMER_SELECT_MIN:
                selector.minutes = compute_offset(selector.minutes, offset, 60);
                break;
            case TIMER_SELECT_SEC:
                selector.seconds = compute_offset(selector.seconds, offset, 60);
                break;
            default:
                break;
        }

        offset_time = selector_to_time(selector);
    }
}

inline uint64_t pos_time(uint64_t time, uint64_t offset_time)
{
    if (time < offset_time) {
        return 0;
    }
    else {
        return time - offset_time;
    }

}

void render_timer_select_screen(uint8_t *buf, main_page_state_t *page_state)
{
    if (selected)
    {
        const char text[] = "hh:mm:ss";
        write_string_vertical(buf, 94, 0, text);
    }

    if (timer_running)
    {
        render_time(buf, 80, 0, pos_time(timer_end , page_state->ui_page.time));
    }
    else {
        render_time(buf, 80, 0, offset_time);
    }

    for (uint8_t i = 0; i < NUM_TIMER_SELECT_OPTIONS; i++)
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
