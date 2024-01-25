#pragma once

#include "App_SharedHeartbeatMonitor.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_rgbLed.h"

typedef struct
{
    const BinaryLed *const imd_led;
    const BinaryLed *const bspd_led;
    const BinaryLed *const shdn_led;
    const BinaryLed *const drive_led;
    const Switch *const    start_switch;
    const Switch *const    aux_switch;
    const RgbLed *const    bms_status_led;
    const RgbLed *const    dcm_status_led;
    const RgbLed *const    fsm_status_led;
    const RgbLed *const    pdm_status_led;
    const RgbLed *const    dim_status_led;
} GlobalsConfig;

typedef struct
{
    const GlobalsConfig     *config;
    struct HeartbeatMonitor *heartbeat_monitor;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init(const GlobalsConfig *config);
