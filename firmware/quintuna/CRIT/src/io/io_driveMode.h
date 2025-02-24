#pragma once

#include <stdint.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *const launch_control;
    const Gpio *const torque_vectoring;
    const Gpio *const regen;
} DriveMode;
#else
#include "app_utils.h"
EMPTY_STRUCT(DriveMode);
#endif

void io_driveMode_init(const DriveMode *config);
/**
 * Readings drive modes from the pins.
 */
uint16_t io_driveMode_readPins(void);
