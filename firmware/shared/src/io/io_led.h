#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *gpio;
} BinaryLed;
#else
#include "app_utils.h"
EMPTY_STRUCT(BinaryLed);
#endif

/**
 * Turn an LED on or off.
 * @param led led to modify
 * @param on Whether or not to turn LED on.
 */
void io_led_enable(const BinaryLed *led, bool on);
