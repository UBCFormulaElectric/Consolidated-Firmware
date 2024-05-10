#pragma once

#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_led.h"
#include "io_driveMode.h"

typedef struct
{
    const DriveMode *const drive_mode;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *config;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init(const GlobalsConfig *config);
