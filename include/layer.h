#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>
#include <stdbool.h>

#include "keyboard_config.h"

#define LAYER_UP (1)
#define LAYER_DOWN (-1)

void update_layer(int8_t dir);
int8_t get_layer();

#endif
