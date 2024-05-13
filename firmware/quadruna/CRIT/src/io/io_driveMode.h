#pragma once

#include <stdbool.h>
#include "app_utils.h"
#include <stdint.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *const n_drive_mode_0_pin;
    const Gpio *const n_drive_mode_1_pin;
    const Gpio *const n_drive_mode_2_pin;
    const Gpio *const n_drive_mode_3_pin;
} DriveMode;
#else
EMPTY_STRUCT(DriveMode);
#endif

void io_driveMode_init(const DriveMode *config);
/**
 * Readings drive modes from the pins.
 */
uint16_t io_driveMode_readPins(void);
