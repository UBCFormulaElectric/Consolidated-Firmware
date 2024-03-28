#pragma once

#include <stdbool.h>
#include "app_heartbeatMonitor.h"
#include "io_led.h"
#include "io_buzzer.h"
#include "app_timer.h"

#define BUZZER_ON_DURATION_MS 2000

typedef struct
{
    const BinaryLed *const brake_light;
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
