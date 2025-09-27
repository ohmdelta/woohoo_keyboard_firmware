#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdatomic.h>

void setup_board(void);
void setup_encoder();

extern bool encoder_button;

extern atomic_uint count_clockwise;
extern atomic_uint count_anti_clockwise;

bool encoder_has_action();

#endif
