#ifndef UI_COMMON_H
#define UI_COMMON_H

#include <stdint.h>

void render_back_button(uint8_t *buf);

void handle_state(ui_page_state_t *page_state, ui_command_t *state, uint8_t num_options);

#endif
