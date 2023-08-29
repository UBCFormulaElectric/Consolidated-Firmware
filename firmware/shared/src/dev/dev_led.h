#pragma once

#include <stdbool.h>

#if defined(TARGET_EMBEDDED)
#include "hw_gpio.h"
#endif

typedef struct
{
#ifdef TARGET_EMBEDDED
    const Gpio gpio;
#endif
} BinaryLed;

/**
 * Turn an LED on or off.
 * @param on Whether or not to turn LED on.
 */
void dev_led_enable(const BinaryLed *led, bool on);
