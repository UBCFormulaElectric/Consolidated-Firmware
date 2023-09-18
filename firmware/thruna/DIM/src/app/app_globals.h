#pragma once

#include "io_led.h"

typedef struct
{
    const BinaryLed *const imd_led;
    const BinaryLed *const bspd_led;
    const BinaryLed *const shdn_led;
    const BinaryLed *const drive_led;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *config;
} Globals;

extern Globals *const globals;

void app_globals_init(const GlobalsConfig *config);
