#include "key_tracking.h"
#include "util.h"

#include <stdint.h>
#include <string.h>

void refresh_usage(key_info_t* key_info, fast_timer_t current_time)
{
    uint8_t i = 0;
    for (; i < min(KEY_INFO_SIZE, key_info->len); i++) {
        if(key_info->timings[i].time < current_time)
        {
            break;
        }
    }
    memcpy(key_info->timings + i,key_info->timings, key_info->len - i);
    key_info->len -= i;
}

void add_key(key_info_t* key_info, fast_timer_t time) {
    add_key_count(key_info, time, 1);
}

void add_key_count(key_info_t* key_info, fast_timer_t time, uint8_t count){
    if (key_info->len < KEY_INFO_SIZE)
    {
        key_info->timings[key_info->len].time = time;
        key_info->timings[key_info->len].count = count;
        key_info->len++;
    }
}

unsigned int key_window_count(key_info_t* key_info, fast_timer_t start_time, fast_timer_t end_time) {
    uint8_t i = 0;
    unsigned int count = 0;
    for (; i < key_info->len; i++) {
        if ((key_info->timings[i].time < end_time) && (key_info->timings[i].time > start_time))
        {
            count++;
        }
    }
    return count;
}
