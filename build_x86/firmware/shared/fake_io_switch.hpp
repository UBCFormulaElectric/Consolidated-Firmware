#pragma once

#include <stdint.h>

extern "C"
{
#include "io_switch.h"
}

// 
// Fakes for: io_switch_isClosed
//

/**
 * Reset the fake for io_switch_isClosed.
 */
void fake_io_switch_isClosed_reset(void);

/**
 * Set the value to be returned by io_switch_isClosed when invoked with a specific set of arguments.
 */
void fake_io_switch_isClosed_returnsForArgs(
    const Switch* arg0,
    bool return_value
);

/**
 * Return the number of times io_switch_isClosed has been invoked.
 */
uint32_t fake_io_switch_isClosed_callCount(void);

/**
 * Return the number of times io_switch_isClosed has been invoked with a specific set of arguments.
 */
uint32_t fake_io_switch_isClosed_callCountForArgs(
    const Switch* arg0
);

