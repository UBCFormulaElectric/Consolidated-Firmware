#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio n_drive_mode_0_pin;
    const Gpio n_drive_mode_1_pin;
    const Gpio n_drive_mode_2_pin;
    const Gpio n_drive_mode_3_pin;
} DriveMode;
#else
EMPTY_STRUCT(DriveMode);
#endif
