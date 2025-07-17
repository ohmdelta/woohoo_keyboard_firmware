#include "keycode_buffer.h"
#include <string.h>

void reset_keycode_buffer(keycode_buffer_t *keycode_buffer)
{
    memset(keycode_buffer, 0, sizeof(keycode_buffer_t));
}

bool add_keycode(keycode_buffer_t *keycode_buffer, uint8_t keycode)
{
    if (keycode_buffer->size < KEYCODE_BUFFER_LENGTH)
    {
        keycode_buffer->keycodes[keycode_buffer->size++] = keycode;
        return true;
    }
    return false;
}

bool valid(keycode_buffer_t *keycode_buffer)
{
    return (keycode_buffer->size <= KEYCODE_BUFFER_LENGTH) && (keycode_buffer->completed <= keycode_buffer->size);
}