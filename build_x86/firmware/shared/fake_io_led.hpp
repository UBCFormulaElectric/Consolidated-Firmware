#pragma once

#include <stdint.h>

extern "C"
{
#include "io_led.h"
}

// 
// Fakes for: io_led_enable
//

/**
 * Reset the fake for io_led_enable.
 */
void fake_io_led_enable_reset(void);

/**
 * Return the number of times io_led_enable has been invoked.
 */
uint32_t fake_io_led_enable_callCount(void);

/**
 * Return the number of times io_led_enable has been invoked with a specific set of arguments.
 */
uint32_t fake_io_led_enable_callCountForArgs(
    const BinaryLed* arg0, 
    bool arg1
);

