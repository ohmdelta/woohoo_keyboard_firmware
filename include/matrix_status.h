#ifndef MATRIX_STATUS_H
#define MATRIX_STATUS_H

#include <stdbool.h>

#include "structs.h"
#include "keyboard_config.h"

typedef struct 
{
    bool is_pressed : 1;
    bool last_state : 1;
    fast_timer_t last_update_time;
    fast_timer_t last_handled_time;
} matrix_status;

#endif