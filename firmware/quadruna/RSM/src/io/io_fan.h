#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio gpio;
} BinaryFan;
#else
EMPTY_STRUCT(BinaryFan);
#endif

/**
 * Turn an LED on or off.
 * @param on Whether or not to turn LED on.
 */
void io_fan_enable(const BinaryFan *fan, bool on);