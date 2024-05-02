#pragma once

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
    const RgbLed *const    bms_status_led;
    const RgbLed *const    fsm_status_led;
    const RgbLed *const    vc_status_led;
    const RgbLed *const    aux_status_led;
    const RgbLed *const    crit_status_led;
    const RgbLed *const    rsm_status_led;
} Leds;

typedef enum
{
    BMS_LED,
    FSM_LED,
    VC_LED,
    AUX_LED,
    CRIT_LED,
    RSM_LED,
    NUM_BOARD_LEDS,
} BoardLeds;

void app_led_init(const Leds *in_leds);

void app_leds_update(void);
