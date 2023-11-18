#pragma once

#include <stdint.h>

extern "C"
{
#include "io_rgbLed.h"
}

// 
// Fakes for: io_rgbLed_enable
//

/**
 * Reset the fake for io_rgbLed_enable.
 */
void fake_io_rgbLed_enable_reset(void);

/**
 * Return the number of times io_rgbLed_enable has been invoked.
 */
uint32_t fake_io_rgbLed_enable_callCount(void);

/**
 * Return the number of times io_rgbLed_enable has been invoked with a specific set of arguments.
 */
uint32_t fake_io_rgbLed_enable_callCountForArgs(
    const RgbLed* arg0, 
    bool arg1, 
    bool arg2, 
    bool arg3
);

// 
// Fakes for: io_rgbLed_disable
//

/**
 * Reset the fake for io_rgbLed_disable.
 */
void fake_io_rgbLed_disable_reset(void);

/**
 * Return the number of times io_rgbLed_disable has been invoked.
 */
uint32_t fake_io_rgbLed_disable_callCount(void);

/**
 * Return the number of times io_rgbLed_disable has been invoked with a specific set of arguments.
 */
uint32_t fake_io_rgbLed_disable_callCountForArgs(
    const RgbLed* arg0
);

