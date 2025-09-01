#ifndef UI_H
#define UI_H

// MAIN SCREEN
#include "ui_structs.h"

void render_ui(uint8_t *buf, ui_command_t *state);

void render_main_screen(uint8_t *buf, main_page_state_t *main_page_state);
void handle_main_screen(main_page_state_t* page_state, ui_command_t* state);

#endif
