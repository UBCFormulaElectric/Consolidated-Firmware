#include <string.h>
#include "states/App_DriveState.h"
#include <stdlib.h>
#include "App_CanAlerts.h"
#include "App_SharedMacros.h"
#include "app_globals.h"
#include "io_led.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888)

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_DIM_Vitals_State_Set(DIM_STATE_DRIVE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_DimWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *        world              = App_SharedStateMachine_GetWorld(state_machine);
    struct SevenSegDisplays *seven_seg_displays = App_DimWorld_GetSevenSegDisplays(world);
    struct HeartbeatMonitor *heartbeat_monitor  = App_DimWorld_GetHeartbeatMonitor(world);
    struct BinarySwitch *    start_switch       = App_DimWorld_GetStartSwitch(world);
    struct BinarySwitch *    aux_switch         = App_DimWorld_GetAuxSwitch(world);
    struct AvgPowerCalc *    avg_power_calc     = App_DimWorld_GetAvgPowerCalc(world);

    App_CanTx_DIM_Vitals_Heartbeat_Set(true);

    const bool imd_fault_latched = App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Get();
    io_led_enable(globals->config->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Get();
    io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    const bool contactors_open = App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_OPEN &&
                                 App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_OPEN;
    io_led_enable(globals->config->shdn_led, contactors_open);

    const bool in_drive_state = App_CanRx_DCM_Vitals_CurrentState_Get() == DCM_DRIVE_STATE;
    io_led_enable(globals->config->drive_led, in_drive_state);

    const bool start_switch_on = App_BinarySwitch_IsTurnedOn(start_switch);
    const bool aux_switch_on   = App_BinarySwitch_IsTurnedOn(aux_switch);

    App_AvgPowerCalc_Enable(avg_power_calc, aux_switch_on);

    App_CanTx_DIM_Switches_StartSwitch_Set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    App_CanTx_DIM_Switches_AuxSwitch_Set(aux_switch_on ? SWITCH_ON : SWITCH_OFF);

    struct RgbLed *board_status_leds[NUM_BOARD_LEDS] = { [BMS_LED] = App_DimWorld_GetBmsStatusLed(world),
                                                         [DCM_LED] = App_DimWorld_GetDcmStatusLed(world),
                                                         [DIM_LED] = App_DimWorld_GetDimStatusLed(world),
                                                         [FSM_LED] = App_DimWorld_GetFsmStatusLed(world),
                                                         [PDM_LED] = App_DimWorld_GetPdmStatusLed(world) };

    CanAlertBoard alert_board_ids[NUM_BOARD_LEDS] = {
        [BMS_LED] = BMS_ALERT_BOARD, [DCM_LED] = DCM_ALERT_BOARD, [DIM_LED] = DIM_ALERT_BOARD,
        [FSM_LED] = FSM_ALERT_BOARD, [PDM_LED] = PDM_ALERT_BOARD,
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        struct RgbLed *board_status_led = board_status_leds[i];

        if (App_CanAlerts_BoardHasFault(alert_board_ids[i]))
        {
            App_SharedRgbLed_TurnRed(board_status_led);
        }
        else if (App_CanAlerts_BoardHasWarning(alert_board_ids[i]))
        {
            App_SharedRgbLed_TurnBlue(board_status_led);
        }
        else
        {
            App_SharedRgbLed_TurnGreen(board_status_led);
        }
    }

    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
    App_CanAlerts_SetFault(DIM_FAULT_MISSING_HEARTBEAT, missing_hb);

    const float avg_rpm = ((float)abs(App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get()) +
                           (float)abs(App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get())) /
                          2;
    const float speed_kph = MOTOR_RPM_TO_KMH(avg_rpm);
    //    float gate_temp = App_CanRx_INVR_Temperatures1_GateDriverBoardTemperature_Get();
    //    float min_cell_voltage = App_CanRx_BMS_CellVoltages_MinCellVoltage_Get();

    const float instant_power = App_CanRx_BMS_TractiveSystem_TsVoltage_Get() *
                                App_CanRx_BMS_TractiveSystem_TsCurrent_Get() / 1000.0f; // instant kW

    const float min_cell_voltage = App_CanRx_BMS_CellVoltages_MinCellVoltage_Get();

    if (missing_hb)
    {
        App_SevenSegDisplays_SetGroupL(seven_seg_displays, SSEG_HB_NOT_RECEIVED_ERR);
        App_SevenSegDisplays_SetGroupM(seven_seg_displays, SSEG_HB_NOT_RECEIVED_ERR);
        App_SevenSegDisplays_SetGroupR(seven_seg_displays, SSEG_HB_NOT_RECEIVED_ERR);
    }
    else
    {
        App_SevenSegDisplays_SetGroupL(seven_seg_displays, speed_kph);
        App_SevenSegDisplays_SetGroupM(seven_seg_displays, instant_power);
        App_SevenSegDisplays_SetGroupR(seven_seg_displays, min_cell_voltage);
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
