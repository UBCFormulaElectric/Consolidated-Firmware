#include "states/driveState.h"
#include <string.h>
#include <stdlib.h>
#include "App_CanAlerts.h"
#include "led.h"
#include "globals.h"
#include "decimalDisplays.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888)

// clang-format off
RgbLed *board_status_leds[NUM_BOARD_LEDS] = { 
    [BMS_LED] = globals->bms_status_led,
    [DCM_LED] = globals->dcm_status_led,
    [DIM_LED] = globals->dim_status_led,
    [FSM_LED] = globals->fsm_status_led,
    [PDM_LED] = globals->pdm_status_led 
};

CanAlertBoard alert_board_ids[NUM_BOARD_LEDS] = {
    [BMS_LED] = BMS_ALERT_BOARD, 
    [DCM_LED] = DCM_ALERT_BOARD, 
    [DIM_LED] = DIM_ALERT_BOARD,
    [FSM_LED] = FSM_ALERT_BOARD, 
    [PDM_LED] = PDM_ALERT_BOARD,
};
// clang-format on

typedef enum
{
    DCM_LED,
    DIM_LED,
    FSM_LED,
    PDM_LED,
    BMS_LED,
    NUM_BOARD_LEDS,
} BoardLeds;

static void driveState_runOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_DIM_Vitals_State_Set(DIM_STATE_DRIVE);
}

static void driveState_tick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void driveState_tick100Hz(struct StateMachine *const state_machine)
{
    // Handle heartbeats (just checks BMS).
    App_CanTx_DIM_Vitals_Heartbeat_Set(true);

    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
    App_CanAlerts_SetFault(DIM_FAULT_MISSING_HEARTBEAT, missing_hb);

    // Enable LED if IMD fault.
    const bool imd_fault = !App_CanRx_BMS_OkStatuses_ImdOk_Get();
    led_enable(globals->imd_led, imd_fault);

    // Enable LED if IMD fault.
    const bool bspd_fault = !App_CanRx_BMS_OkStatuses_BspdOk_Get();
    led_enable(globals->bspd_led, bspd_fault);

    // Enable SHDN LED if contactors open.
    const bool contactors_open = App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_OPEN &&
                                 App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_OPEN;
    led_enable(globals->shdn_led, contactors_open);

    // Enable DCM LED if contactors open.
    const bool dcm_in_drive_state = App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_OPEN &&
                                    App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_OPEN;
    led_enable(globals->drive_led, dcm_in_drive_state);

    // Broadcast start switch statuses.
    const bool start_switch_on = switch_isEnabled(globals->start_switch);
    const bool aux_switch_on   = switch_isEnabled(globals->);
    App_CanTx_DIM_Switches_StartSwitch_Set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    App_CanTx_DIM_Switches_AuxSwitch_Set(aux_switch_on ? SWITCH_ON : SWITCH_OFF);

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        struct RgbLed *board_status_led = board_status_leds[i];

        if (App_CanAlerts_BoardHasFault(alert_board_ids[i]))
        {
            // If board faulted, turn red.
            rgbLed_enable(board_status_led, true, false, false);
        }
        else if (App_CanAlerts_BoardHasWarning(alert_board_ids[i]))
        {
            // If board set a warning, turn blue.
            rgbLed_enable(board_status_led, false, false, true);
        }
        else
        {
            // If board OK, turn green.
            rgbLed_enable(board_status_led, false, true, false);
        }
    }

    // Calculate values to display on seven seg banks.
    const float left_motor_speed = (float)abs(App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()
    const float right_motor_speed = (float)abs(App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()
    float avg_rpm = (left_motor_speed + right_motor_speed) / 2;
    float speed_kph = MOTOR_RPM_TO_KMH(avg_rpm);
    float instant_power_kW =
        App_CanRx_BMS_TractiveSystem_TsVoltage_Get() * App_CanRx_BMS_TractiveSystem_TsCurrent_Get() / 1000.0f;

    // Reset average power calculation aux switch flicked.
    float avg_power;
    if (aux_switch_on)
    {
        avg_power = avgPower_update(instant_power_kW);
    }
    else
    {
        avgPower_reset();
        avg_power = 0.0f;
    }

    if (missing_hb)
    {
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_L, SSEG_HB_NOT_RECEIVED_ERR);
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_M, SSEG_HB_NOT_RECEIVED_ERR);
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_R, SSEG_HB_NOT_RECEIVED_ERR);
    }
    else
    {
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_L, speed_kph);
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_M, instant_power_kW);
        decimalDisplays_displayNumber(DECIMAL_DISPLAY_BANK_R, avg_power);
    }
}

static void driveState_runOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *driveState_getState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveState_runOnEntry,
        .run_on_tick_1Hz   = driveState_tick1Hz,
        .run_on_tick_100Hz = driveState_tick100Hz,
        .run_on_exit       = driveState_runOnExit,
    };

    return &drive_state;
}
