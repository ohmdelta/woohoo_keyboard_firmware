#include "ssd1306.h"
#include "ui/common.h"

void render_back_button(uint8_t *buf)
{
    draw_horizontal_divider(buf, 12, 1);
    write_string_vertical(buf, 2, 0, " BACK");
}

void handle_state(ui_page_state_t *page_state, ui_command_t *state, uint8_t num_options)
{
    int8_t ui_state = page_state->state + (state->cw_count - state->ccw_count);
    ui_state %= num_options;
    if (ui_state < 0)
    {
        ui_state += num_options;
    }

    page_state->state = ui_state;
}
