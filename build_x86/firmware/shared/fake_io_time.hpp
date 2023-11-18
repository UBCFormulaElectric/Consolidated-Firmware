#pragma once

#include <stdint.h>

extern "C"
{
#include "io_time.h"
}

// 
// Fakes for: io_time_getCurrentMs
//

/**
 * Reset the fake for io_time_getCurrentMs.
 */
void fake_io_time_getCurrentMs_reset(void);

/**
 * Set the value to be returned when io_time_getCurrentMs is invoked.
 */
void fake_io_time_getCurrentMs_returns(uint32_t return_value);

/**
 * Return the number of times io_time_getCurrentMs has been invoked.
 */
uint32_t fake_io_time_getCurrentMs_callCount(void);

