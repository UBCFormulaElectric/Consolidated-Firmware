#pragma once

#include <stdbool.h>
#include "io_led.h"

typedef struct
{
    const BinaryLed *const brake_light;
    const Gpio *const      acc_fan;
    const Gpio *const      rad_fan;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *config;

} Globals;

extern Globals *const globals;

void app_globals_init();