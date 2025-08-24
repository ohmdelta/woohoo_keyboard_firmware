#include "util.h"

uint8_t reverse(uint8_t x)
{
    x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
    x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
    x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
    return x;
}

void buf_swap(uint8_t* a, uint8_t* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
    *a = reverse(*a);
    *b = reverse(*b);
}
