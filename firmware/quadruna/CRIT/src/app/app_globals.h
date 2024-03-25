#pragma once

#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_led.h"

typedef struct
{
    const BinaryLed *const imd_led;
    const BinaryLed *const bspd_led;
    const BinaryLed *const ams_led;
    const BinaryLed *const start_led;
    const BinaryLed *const regen_led;
    const BinaryLed *const torquevec_led;
    const BinaryLed *const shdn_led;
    const Switch *const    start_switch;
    const Switch *const    regen_switch;
    const Switch *const    torquevec_switch;
    const RgbLed *const    bms_status_led;
    const RgbLed *const    fsm_status_led;
    const RgbLed *const    vc_status_led;
    const RgbLed *const    aux_status_led;
    const RgbLed *const    crit_status_led;
    const RgbLed *const    rsm_status_led;
    const Gpio *const      shdn_sen_pin;
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
