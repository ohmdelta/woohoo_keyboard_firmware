#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>
#include <stdbool.h>

#include "keyboard_config.h"

#define LAYER_DOWN_CODE ((uint8_t)(254))
#define LAYER_UP_CODE ((uint8_t)(255))

#define LAYER_UP ((int8_t)(1))
#define LAYER_DOWN ((int8_t)(-1))

enum
{
    LAYER_NORMAL = 0,
    LAYER_SYMBOL = 1,
    LAYER_CONTROL = 2,
};

void update_layer(int8_t dir);
int8_t get_layer();

#endif
