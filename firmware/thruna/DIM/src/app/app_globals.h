#pragma once

#include "app_heartbeatMonitor.h"
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
    HeartbeatMonitor *heartbeat_monitor;
    uint32_t                 previous_time;
    uint32_t                 *fault_code_array;
    uint32_t                 *warning_code_array;
    uint32_t                 *total_faults;
    uint8_t                  fault_element_num;
    uint8_t                  warning_element_num;
    uint8_t                  element_num;
    uint8_t                  total_element_num;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init(const GlobalsConfig *config);
