#ifndef SSD1306_UTIL_H
#define SSD1306_UTIL_H

#include <stdint.h>

uint8_t reverse(uint8_t x);

void buf_swap(uint8_t *a, uint8_t *b);

#endif