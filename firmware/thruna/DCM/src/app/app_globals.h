#pragma once

#include "App_SharedHeartbeatMonitor.h"
#include "io_led.h"
#include "io_buzzer.h"

typedef struct
{
    const BinaryLed *const brake_light;
    const Buzzer *const    buzzer;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig *    config;
    struct HeartbeatMonitor *heartbeat_monitor;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init(const GlobalsConfig *config);
