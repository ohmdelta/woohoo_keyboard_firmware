#ifndef KEYCODE_BUFFER_H
#define KEYCODE_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define KEYCODE_BUFFER_LENGTH 125

typedef union
{
    struct
    {
        bool left_ctrl : 1;   ///< left control
        bool left_shift : 1;  ///< left shift
        bool left_alt : 1;    ///< left alt
        bool left_gui : 1;    ///< left window
        bool right_ctrl : 1;  ///< right control
        bool right_shift : 1; ///< right shift
        bool right_alt : 1;   ///< right alt
        bool right_gui : 1;   ///< right window
    } modifiers;
    uint8_t bits;
} modifier_t;

typedef struct
{
    uint8_t size;
    uint8_t completed;
    bool null_sent;
    modifier_t modifier;
    uint8_t keycodes[KEYCODE_BUFFER_LENGTH];
} keycode_buffer_t;

void reset_keycode_buffer(keycode_buffer_t *keycode_buffer);

bool add_keycode(keycode_buffer_t *keycode_buffer, uint8_t keycode);
bool add_keycodes(keycode_buffer_t *keycode_buffer, const uint8_t* keycode);
bool valid(keycode_buffer_t *keycode_buffer);

void update_modifier(modifier_t *modifier, uint8_t key);

#endif
