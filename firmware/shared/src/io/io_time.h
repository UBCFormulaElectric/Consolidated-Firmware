#pragma once

#include <stdint.h>

/**
 * Get the time passed since startup, in milliseconds.
 * @return Time in ms.
 */
uint32_t io_time_getCurrentMs(void);

/**
 * Delays the program for a given amount of time.
 * @param ms time to delay in milliseconds.
 */
void io_time_delay(uint32_t ms);