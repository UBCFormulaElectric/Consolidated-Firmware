#pragma once

#include <stdint.h>

/**
 * Get the time passed since startup, in milliseconds.
 * @return Time in ms.
 */
uint32_t hw_time_getCurrentMs(void);
