#include <string.h>
#include "app_mainState.h"
#include <stdlib.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_globals.h"
#include "app_driveMode.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_shutdownSensor.h"
#include "tasks.h"
#include "app_heartbeatMonitor.h"

static void mainStateRunOnTick100Hz(void)
{
    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_enable(globals->config->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    const bool ams_fault_latched = app_canRx_BMS_BmsLatchedFault_get();
    io_led_enable(globals->config->ams_led, ams_fault_latched);

    const bool shutdown_sensor = io_shutdownSensor_readPin(globals->config->shdn_sen);
    io_led_enable(globals->config->shdn_led, shutdown_sensor);

    const bool start_switch_on = io_switch_isClosed(globals->config->start_switch);
    app_canTx_CRIT_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool start_led_on = app_canRx_VC_State_get() == VC_DRIVE_STATE;
    io_led_enable(globals->config->start_led, start_led_on);

    const bool regen_switch_on = io_switch_isClosed(globals->config->regen_switch);
    app_canTx_CRIT_RegenSwitch_set(regen_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool regen_light_on = app_canRx_VC_RegenEnabled_get();
    io_led_enable(globals->config->regen_led, regen_light_on);

    const bool torquevec_switch_on = io_switch_isClosed(globals->config->torquevec_switch);
    app_canTx_CRIT_TorqueVecSwitch_set(torquevec_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    io_led_enable(globals->config->torquevec_led, torquevec_light_on);

    app_driveMode_broadcast();

    typedef struct
    {
        const RgbLed *led;
        CanAlertBoard jsoncan_board_enum;
    } LedCAN;

    const LedCAN boards[NUM_BOARD_LEDS] = {
        { .led = globals->config->bms_status_led, .jsoncan_board_enum = BMS_ALERT_BOARD },
        { .led = globals->config->fsm_status_led, .jsoncan_board_enum = FSM_ALERT_BOARD },
        { .led = globals->config->vc_status_led, .jsoncan_board_enum = VC_ALERT_BOARD },
        { .led = globals->config->aux_status_led },
        { .led = globals->config->crit_status_led, .jsoncan_board_enum = CRIT_ALERT_BOARD },
        { .led = globals->config->rsm_status_led },
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        const RgbLed *board_status_led = boards[i].led;
        CanAlertBoard alert_board_ids  = boards[i].jsoncan_board_enum;

        if (app_canAlerts_BoardHasFault(alert_board_ids))
        {
            // Turn red.
            io_rgbLed_enable(board_status_led, true, false, false);
        }
        else if (app_canAlerts_BoardHasWarning(alert_board_ids))
        {
            // Turn blue.
            io_rgbLed_enable(board_status_led, false, false, true);
        }
        else
        {
            // Turn green.
            io_rgbLed_enable(board_status_led, false, true, false);
        }
    }

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();
}

static void mainStateRunOnEntry(void) {}
static void mainStateRunOnTick1Hz(void) {}
static void mainStateRunOnExit(void) {}

const State *app_mainState_get(void)
{
    static const State main_state = {
        .name              = "MAIN",
        .run_on_entry      = mainStateRunOnEntry,
        .run_on_tick_1Hz   = mainStateRunOnTick1Hz,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = mainStateRunOnExit,
    };

    return &main_state;
}
