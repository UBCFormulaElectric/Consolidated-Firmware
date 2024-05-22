#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_rgbLed.h"
#include "io_led.h"

typedef struct
{
    const BinaryLed *const imd_led;
    const BinaryLed *const bspd_led;
    const BinaryLed *const ams_led;
    const BinaryLed *const start_led;
    const BinaryLed *const regen_led;
    const BinaryLed *const torquevec_led;
    const RgbLed *const    shdn_led;
    const RgbLed *const    bms_status_led;
    const RgbLed *const    fsm_status_led;
    const RgbLed *const    vc_status_led;
    const RgbLed *const    aux_status_led;
    const RgbLed *const    crit_status_led;
    const RgbLed *const    rsm_status_led;
} Leds;

void io_led_init(const Leds *in_leds);
#endif

void io_led_imd_set(bool val);
void io_led_bspd_set(bool val);
void io_led_ams_set(bool val);
void io_led_start_set(bool val);
void io_led_regen_set(bool val);
void io_led_torquevec_set(bool val);

typedef enum
{
    BOARD_LED_STATUS_OK,
    BOARD_LED_STATUS_WARNING,
    BOARD_LED_STATUS_FAULT,
    BOARD_LED_STATUS_NOT_IMPLEMENTED,
    BOARD_LED_STATUS_MISSING_HEARTBEAT
} BoardLEDStatus;

void io_led_shutdown_set(BoardLEDStatus status);
void io_led_bms_status_set(BoardLEDStatus status);
void io_led_fsm_status_set(BoardLEDStatus status);
void io_led_vc_status_set(BoardLEDStatus status);
void io_led_aux_status_set(BoardLEDStatus status);
void io_led_crit_status_set(BoardLEDStatus status);
void io_led_rsm_status_set(BoardLEDStatus status);
