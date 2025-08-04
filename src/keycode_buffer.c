#include "keycode_buffer.h"
#include <string.h>
#include <tusb.h>

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

bool add_keycodes(keycode_buffer_t *keycode_buffer, const uint8_t * keycode)
{
    while ((keycode_buffer->size < KEYCODE_BUFFER_LENGTH) && (*keycode))
    {
        keycode_buffer->keycodes[keycode_buffer->size++] = *(keycode++);
    }
    return true;
}

bool valid(keycode_buffer_t *keycode_buffer)
{
    return (keycode_buffer->size <= KEYCODE_BUFFER_LENGTH) && (keycode_buffer->completed <= keycode_buffer->size);
}

void update_modifier(modifier_t *modifier, uint8_t key)
{
  switch (key)
  {
  case HID_KEY_CONTROL_LEFT:
    modifier->modifiers.left_ctrl = 1;
    break;
  case HID_KEY_SHIFT_LEFT:
    modifier->modifiers.left_shift = 1;
    break;
  case HID_KEY_ALT_LEFT:
    modifier->modifiers.left_alt = 1;
    break;
  case HID_KEY_GUI_LEFT:
    modifier->modifiers.left_gui = 1;
    break;
  case HID_KEY_CONTROL_RIGHT:
    modifier->modifiers.right_ctrl = 1;
    break;
  case HID_KEY_SHIFT_RIGHT:
    modifier->modifiers.right_shift = 1;
    break;
  case HID_KEY_ALT_RIGHT:
    modifier->modifiers.right_alt = 1;
    break;
  case HID_KEY_GUI_RIGHT:
    modifier->modifiers.right_gui = 1;
    break;

  default:
    break;
  }
}
