#pragma once

#include "led.h"
#include "switch.h"
#include "rgbLed.h"

typedef struct
{
    Led *   imd_led;
    Led *   bspd_led;
    Led *   shdn_led;
    Led *   drive_led;
    Switch *start_switch;
    Switch *aux_switch;
    RgbLed *bms_status_led;
    RgbLed *dcm_status_led;
    RgbLed *dim_status_led;
    RgbLed *fsm_status_led;
    RgbLed *pdm_status_led;
} Globals;

extern Globals *const globals;

void globals_init(
    Led *   imd_led,
    Led *   bspd_led,
    Led *   shdn_led,
    Led *   drive_led,
    Switch *start_switch,
    Switch *aux_switch,
    RgbLed *bms_status_led,
    RgbLed *dcm_status_led,
    RgbLed *dim_status_led,
    RgbLed *fsm_status_led,
    RgbLed *pdm_status_led);