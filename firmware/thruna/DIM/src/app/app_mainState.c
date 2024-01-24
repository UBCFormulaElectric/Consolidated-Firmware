#include <string.h>
#include "app_mainState.h"
#include <stdlib.h>
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_globals.h"
#include "app_sevenSegDisplays.h"
#include "app_avgPower.h"
#include "io_led.h"
#include "io_switch.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888)
#define WHEEL_DIAMETER_IN (16.0f)
#define GEAR_RATIO (4.3f)
#define MOTOR_RPM_TO_KMH(kmh) \
    ((kmh) * (float)WHEEL_DIAMETER_IN * PI * INCH_TO_KM * MIN_TO_HOUR / GEAR_RATIO) // take rpm of whell to kph

static void mainStateRunOnTick100Hz(void)
{
    const bool imd_fault_latched = App_CanRx_BMS_ImdLatchedFault_Get();
    io_led_enable(globals->config->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = App_CanRx_BMS_BspdLatchedFault_Get();
    io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    const bool contactors_open = App_CanRx_BMS_AirNegative_Get() == CONTACTOR_STATE_OPEN &&
                                 App_CanRx_BMS_AirPositive_Get() == CONTACTOR_STATE_OPEN;
    io_led_enable(globals->config->shdn_led, contactors_open);

    const bool in_drive_state = App_CanRx_DCM_State_Get() == DCM_DRIVE_STATE;
    io_led_enable(globals->config->drive_led, in_drive_state);

    const bool start_switch_on = io_switch_isClosed(globals->config->start_switch);
    const bool aux_switch_on   = io_switch_isClosed(globals->config->aux_switch);

    app_avgPower_enable(aux_switch_on);

    App_CanTx_DIM_StartSwitch_Set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    App_CanTx_DIM_AuxSwitch_Set(aux_switch_on ? SWITCH_ON : SWITCH_OFF);

    const RgbLed *board_status_leds[NUM_BOARD_LEDS] = {
        [BMS_LED] = globals->config->bms_status_led, [DCM_LED] = globals->config->dcm_status_led,
        [DIM_LED] = globals->config->dim_status_led, [FSM_LED] = globals->config->fsm_status_led,
        [PDM_LED] = globals->config->pdm_status_led,
    };

    CanAlertBoard alert_board_ids[NUM_BOARD_LEDS] = {
        [BMS_LED] = BMS_ALERT_BOARD, [DCM_LED] = DCM_ALERT_BOARD, [DIM_LED] = DIM_ALERT_BOARD,
        [FSM_LED] = FSM_ALERT_BOARD, [PDM_LED] = PDM_ALERT_BOARD,
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        const RgbLed *board_status_led = board_status_leds[i];

        if (App_CanAlerts_BoardHasFault(alert_board_ids[i]))
        {
            // Turn red.
            io_rgbLed_enable(board_status_led, true, false, false);
        }
        else if (App_CanAlerts_BoardHasWarning(alert_board_ids[i]))
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

    app_heartbeatMonitor_checkIn(globals->heartbeat_monitor);

    app_heartbeatMonitor_tick(globals->heartbeat_monitor);
    app_heartbeatMonitor_broadcastFaults(globals->heartbeat_monitor);

    bool missing_hb = false;

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        missing_hb |= !globals->heartbeat_monitor->status[board];
    }

    const float avg_rpm =
        ((float)abs(App_CanRx_INVL_MotorSpeed_Get()) + (float)abs(App_CanRx_INVR_MotorSpeed_Get())) / 2;
    const float speed_kph = MOTOR_RPM_TO_KMH(avg_rpm);

    const float instant_power =
        App_CanRx_BMS_TractiveSystemVoltage_Get() * App_CanRx_BMS_TractiveSystemCurrent_Get() / 1000.0f; // instant kW

    const float min_cell_voltage = App_CanRx_BMS_MinCellVoltage_Get();

    if (missing_hb)
    {
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, SSEG_HB_NOT_RECEIVED_ERR);
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, SSEG_HB_NOT_RECEIVED_ERR);
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, SSEG_HB_NOT_RECEIVED_ERR);
    }
    else
    {
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, speed_kph);
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, min_cell_voltage);
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, instant_power);
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
