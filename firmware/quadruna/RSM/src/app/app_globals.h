#pragma once

#include <stdbool.h>
#include "io_led.h"

typedef struct
{
    const BinaryLed *const brake_light;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *config;

} Globals;

extern Globals *const globals;

void app_globals_init(const GlobalsConfig *config);
