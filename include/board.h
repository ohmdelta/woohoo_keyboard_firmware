#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

void setup_board(void);

extern bool encoder_a;
extern bool encoder_b;
extern bool encoder_button;

bool encoder_has_action();

#endif