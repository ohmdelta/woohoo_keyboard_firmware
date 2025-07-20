#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "keyboard_config.h"
#include "structs.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Debounce raw matrix events according to the choosen debounce
 * algorithm.
 *
 * @param raw The current key state
 * @param cooked The debounced key state
 * @param num_rows Number of rows to debounce
 * @param changed True if raw has changed since the last call
 * @return true Cooked has new keychanges after debouncing
 * @return false Cooked is the same as before
 */
bool debounce(uint64_t *raw, uint64_t *cooked,
              bool changed);

void debounce_init(uint8_t num_rows);

void debounce_free(void);

#endif