#pragma once

#include <stdbool.h>
#include "io_led.h"
#include "io_fan.h"

typedef struct
{
    const BinaryLed *brake_light;
    const BinaryFan *acc_fan;
    const BinaryFan *rad_fan;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *config;

} Globals;

extern Globals *const globals;

void app_globals_init();