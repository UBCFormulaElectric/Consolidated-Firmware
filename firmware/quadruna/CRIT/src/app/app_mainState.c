#include <string.h>
#include "app_mainState.h"
#include <stdlib.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_globals.h"
#include "io_led.h"
#include "io_switch.h"

static void mainStateRunOnTick100Hz(void)
{
    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_enable(globals->config->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    // TODO: AMS fault
    // const bool ams_fault_latched =
    // io_led_enable(globals->config->ams_led, ams_fault_latched);

    const bool contactors_open = app_canRx_BMS_AirNegative_get() == CONTACTOR_STATE_OPEN &&
                                 app_canRx_BMS_AirPositive_get() == CONTACTOR_STATE_OPEN;
    io_led_enable(globals->config->shdn_led, contactors_open);

    const bool start_switch_on = io_switch_isClosed(globals->config->start_switch);
    app_canTx_CRIT_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->start_led, start_switch_on);

    const bool regen_switch_on = io_switch_isClosed(globals->config->regen_switch);
    app_canTx_CRIT_RegenSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->regen_led, regen_switch_on);

    const bool torquevec_switch_on = io_switch_isClosed(globals->config->torquevec_switch);
    app_canTx_CRIT_TorqueVecSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->torquevec_led, torquevec_switch_on);

    const RgbLed *board_status_leds[NUM_BOARD_LEDS] = {
        [BMS_LED] = globals->config->bms_status_led,   [FSM_LED] = globals->config->fsm_status_led,
        [VC_LED] = globals->config->vc_status_led,     [AUX_LED] = globals->config->aux_status_led,
        [CRIT_LED] = globals->config->crit_status_led, [RSM_LED] = globals->config->rsm_status_led
    };

    CanAlertBoard alert_board_ids[NUM_BOARD_LEDS] = {
        [BMS_LED] = BMS_ALERT_BOARD,
        [FSM_LED] = FSM_ALERT_BOARD,
        [VC_LED]  = VC_ALERT_BOARD,
        //[AUX_LED] = AUX_ALERT_BOARD
        [CRIT_LED] = CRIT_ALERT_BOARD,
        //[RSM_LED] = PDM_ALERT_BOARD
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        const RgbLed *board_status_led = board_status_leds[i];

        if (app_canAlerts_BoardHasFault(alert_board_ids[i]))
        {
            // Turn red.
            io_rgbLed_enable(board_status_led, true, false, false);
        }
        else if (app_canAlerts_BoardHasWarning(alert_board_ids[i]))
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
}
const State *app_mainState_get(void)
{
    static const State main_state = {
        .name              = "MAIN",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &main_state;
}