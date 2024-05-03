#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *const shdn_sen_pin;
} ShutdownSensor;
#else
EMPTY_STRUCT(ShutdownSensor);
#endif

/**
 * Readings pins.
 */
bool io_shutdownSensor_readPin(const ShutdownSensor *gpio);
