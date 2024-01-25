#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio gpio;
} BinaryLed;
#else
EMPTY_STRUCT(BinaryLed);
#endif

/**
 * Turn an LED on or off.
 * @param on Whether or not to turn LED on.
 */
void io_led_enable(const BinaryLed *led, bool on);
