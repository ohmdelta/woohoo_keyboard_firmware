#include "layer.h"

volatile int8_t layer = 0;

void update_layer(int8_t dir)
{
    layer += dir;
    layer %= KEYBOARD_LAYERS;
    if (layer < 0)
    {
        layer += KEYBOARD_LAYERS;
    }
}

int8_t get_layer()
{
    return layer;
}