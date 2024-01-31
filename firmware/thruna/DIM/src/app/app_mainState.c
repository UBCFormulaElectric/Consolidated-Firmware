#include <string.h>
#include "app_mainState.h"
#include <stdlib.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_globals.h"
#include "app_sevenSegDisplays.h"
#include "app_avgPower.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_time.h"

static void mainStateRunOnTick100Hz(void)
{
    uint8_t *fault_array   = globals->fault_code_array;
    uint8_t *warning_array = globals->warning_code_array;
    uint8_t  element_num   = globals->element_num;
    uint32_t current_time  = io_time_getCurrentMs();
    uint32_t previous_time = globals->previous_time;

    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_enable(globals->config->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    const bool contactors_open = app_canRx_BMS_AirNegative_get() == CONTACTOR_STATE_OPEN &&
                                 app_canRx_BMS_AirPositive_get() == CONTACTOR_STATE_OPEN;
    io_led_enable(globals->config->shdn_led, contactors_open);

    const bool in_drive_state = app_canRx_DCM_State_get() == DCM_DRIVE_STATE;
    io_led_enable(globals->config->drive_led, in_drive_state);

    const bool start_switch_on = io_switch_isClosed(globals->config->start_switch);
    const bool aux_switch_on   = io_switch_isClosed(globals->config->aux_switch);

    app_avgPower_enable(aux_switch_on);

    app_canTx_DIM_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    app_canTx_DIM_AuxSwitch_set(aux_switch_on ? SWITCH_ON : SWITCH_OFF);

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

        if (app_canAlerts_BoardHasFault(alert_board_ids[i]))
        {
            // Turn red.
            globals->fault_element_num += app_canAlerts_FaultCode(alert_board_ids[i], fault_array);
            io_rgbLed_enable(board_status_led, true, false, false);
        }
        else if (app_canAlerts_BoardHasWarning(alert_board_ids[i]))
        {
            // Turn blue.
            globals->warning_element_num += app_canAlerts_WarningCode(alert_board_ids[i], warning_array);
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

    uint8_t fault_num   = globals->fault_element_num;
    uint8_t warning_num = globals->warning_element_num;

    const float avg_rpm =
        ((float)abs(app_canRx_INVL_MotorSpeed_get()) + (float)abs(app_canRx_INVR_MotorSpeed_get())) / 2;
    const float speed_kph = MOTOR_RPM_TO_KMH(avg_rpm);

    const float instant_power =
        app_canRx_BMS_TractiveSystemVoltage_get() * app_canRx_BMS_TractiveSystemCurrent_get() / 1000.0f; // instant kW

    const float min_cell_voltage = app_canRx_BMS_MinCellVoltage_get();
    uint32_t    time_difference  = current_time - previous_time;

    if (globals->fault_in_car)
    {
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, (float)fault_array[element_num]);
        globals->element_num++;
    }

    else if (time_difference > 2000 && time_difference < 2500)
    {
        if (fault_num > 0)
        {
            app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, (float)fault_array[element_num]);
            globals->element_num  = 0;
            globals->fault_in_car = true;
        }

        else if (element_num < warning_num)
        {
            app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, (float)fault_array[element_num]);
        }

        else
        {
            app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, speed_kph);
            globals->element_num = 0;
        }
        globals->previous_time = previous_time;
    }

    else
    {
        app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, speed_kph);
    }
    app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, min_cell_voltage);
    app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, instant_power);
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
