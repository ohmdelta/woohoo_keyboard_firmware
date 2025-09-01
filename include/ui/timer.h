#ifndef UI_TIMER_H
#define UI_TIMER_H

#include "ui_structs.h"

void handle_timer_select_screen(main_page_state_t *page_state, ui_command_t *state);
void render_timer_select_screen(uint8_t *buf, main_page_state_t *page_state);

#endif
