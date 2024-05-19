#pragma once

#include <stdbool.h>
#include "app_heartbeatMonitor.h"
#include "io_led.h"
#include "app_timer.h"

typedef struct
{
    uint8_t a;
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
