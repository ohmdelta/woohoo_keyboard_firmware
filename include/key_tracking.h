#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

#include "structs.h"
#include <stdint.h>

#ifndef KEY_INFO_SIZE
#define KEY_INFO_SIZE 124
#endif

typedef struct
{
    uint8_t count;
    fast_timer_t time;
} key_count_time_t;
typedef struct 
{
    uint8_t len;
    uint8_t _size;
    key_count_time_t timings[KEY_INFO_SIZE];
} key_info_t;

void add_key(key_info_t* key_info, fast_timer_t time);
void add_key_count(key_info_t* key_info, fast_timer_t time, uint8_t count);
void refresh_usage(key_info_t* key_info, fast_timer_t current_time);

/*
 * Counter number of keys pressed between start and end time.
 * */
unsigned int key_window_count(key_info_t* key_info, fast_timer_t start_time, fast_timer_t end_time);

#endif
