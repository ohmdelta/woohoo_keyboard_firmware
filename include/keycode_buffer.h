#ifndef KEYCODE_BUFFER_H
#define KEYCODE_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define KEYCODE_BUFFER_LENGTH 126

typedef struct 
{
    uint8_t size;
    uint8_t completed;
    uint8_t keycodes[KEYCODE_BUFFER_LENGTH];
} keycode_buffer_t;

void reset_keycode_buffer(keycode_buffer_t* keycode_buffer);

bool add_keycode(keycode_buffer_t* keycode_buffer, uint8_t keycode);
bool valid(keycode_buffer_t* keycode_buffer);

#endif