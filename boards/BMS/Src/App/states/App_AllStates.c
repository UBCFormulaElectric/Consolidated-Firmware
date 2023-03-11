#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_Accumulator.h"
#include "App_SharedMacros.h"
#include "App_SharedProcessing.h"

#define MAX_POWER_LIMIT_W (78e3f)
#define CELL_ROLL_OFF_TEMP_DEGC (40.0f)
#define CELL_FULLY_DERATED_TEMP (60.0f)

// Num of cycles for voltage and cell temperature values to settle
#define NUM_CYCLES_TO_SETTLE (3U)

static uint8_t acc_meas_settle_count = 0U;

static void App_SendAndReceiveHeartbeat(struct HeartbeatMonitor *hb_monitor)
{
    App_CanTx_BMS_Vitals_Heartbeat_Set(true);

    if (App_CanRx_FSM_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, FSM_HEARTBEAT_ONE_HOT);
        App_CanRx_FSM_Vitals_Heartbeat_Update(false);
    }

    if (App_CanRx_DCM_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, DCM_HEARTBEAT_ONE_HOT);
        App_CanRx_DCM_Vitals_Heartbeat_Update(false);
    }

    if (App_CanRx_PDM_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, PDM_HEARTBEAT_ONE_HOT);
        App_CanRx_PDM_Vitals_Heartbeat_Update(false);
    }
}

static void App_CheckCellVoltageRange(struct Accumulator *accumulator)
{
    // Get the min and max cell voltages and check to see if the voltages are in range
    uint8_t     min_segment           = 0U;
    uint8_t     min_loc               = 0U;
    uint8_t     max_segment           = 0U;
    uint8_t     max_loc               = 0U;
    const float curr_min_cell_voltage = App_Accumulator_GetMinVoltage(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_voltage = App_Accumulator_GetMaxVoltage(accumulator, &max_segment, &max_loc);

    App_CanTx_BMS_CellVoltages_MinCellVoltage_Set(curr_min_cell_voltage);
    App_CanTx_BMS_CellVoltages_MaxCellVoltage_Set(curr_max_cell_voltage);
    App_CanTx_BMS_CellStats_MinCellVoltageSegment_Set(min_segment);
    App_CanTx_BMS_CellStats_MaxCellVoltageSegment_Set(max_segment);
    App_CanTx_BMS_CellStats_MinCellVoltageIdx_Set(min_loc);
    App_CanTx_BMS_CellStats_MaxCellVoltageIdx_Set(max_loc);
}

static void App_CheckCellTemperatureRange(struct Accumulator *accumulator, struct StateMachine *state_machine)
{
    // Get the min and max cell temperature and check to see if the temperatures
    // are in range
    uint8_t     min_segment        = 0U;
    uint8_t     min_loc            = 0U;
    uint8_t     max_segment        = 0U;
    uint8_t     max_loc            = 0U;
    const float curr_min_cell_temp = App_Accumulator_GetMinCellTempDegC(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_temp = App_Accumulator_GetMaxCellTempDegC(accumulator, &max_segment, &max_loc);

    App_CanTx_BMS_CellTemperatures_MinCellTemperature_Set(curr_min_cell_temp);
    App_CanTx_BMS_CellTemperatures_MaxCellTemperature_Set(curr_max_cell_temp);
    App_CanTx_BMS_CellStats_MinTempSegment_Set(min_segment);
    App_CanTx_BMS_CellStats_MaxTempSegment_Set(max_segment);
    App_CanTx_BMS_CellStats_MinTempIdx_Set(min_loc);
    App_CanTx_BMS_CellStats_MaxTempIdx_Set(max_loc);
}

static void App_AdvertisePackPower(struct Accumulator *accumulator, struct TractiveSystem *ts)
{
    uint8_t segment = 0;
    UNUSED(segment);

    const float max_cell_temp = App_Accumulator_GetMaxCellTempDegC(accumulator, &segment, &segment);
    const float available_power =
        MIN(App_SharedProcessing_LinearDerating(
                max_cell_temp, MAX_POWER_LIMIT_W, CELL_ROLL_OFF_TEMP_DEGC, CELL_FULLY_DERATED_TEMP),
            MAX_POWER_LIMIT_W);

    App_CanTx_BMS_AvailablePower_AvailablePower_Set(available_power);
}

void App_AllStates_Init()
{
    // Reset accumulator settle count
    acc_meas_settle_count = 0U;
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *       charger          = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_BMS_Charger_IsConnected_Set(charger_is_connected);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *        world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Imd *             imd         = App_BmsWorld_GetImd(world);
    struct OkStatus *        bms_ok      = App_BmsWorld_GetBmsOkStatus(world);
    struct OkStatus *        imd_ok      = App_BmsWorld_GetImdOkStatus(world);
    struct OkStatus *        bspd_ok     = App_BmsWorld_GetBspdOkStatus(world);
    struct Airs *            airs        = App_BmsWorld_GetAirs(world);
    struct Accumulator *     accumulator = App_BmsWorld_GetAccumulator(world);
    struct HeartbeatMonitor *hb_monitor  = App_BmsWorld_GetHeartbeatMonitor(world);
    struct TractiveSystem *  ts          = App_BmsWorld_GetTractiveSystem(world);
    struct Charger *         charger     = App_BmsWorld_GetCharger(world);

    bool status = true;
    App_SendAndReceiveHeartbeat(hb_monitor);

    App_Accumulator_RunOnTick100Hz(accumulator);
    App_CheckCellVoltageRange(accumulator);
    App_CheckCellTemperatureRange(accumulator, state_machine);

    const bool acc_fault = App_Accumulator_CheckFaults(accumulator, ts);
    const bool ts_fault  = App_TractveSystem_CheckFaults(ts);

    App_CanTx_BMS_PackVoltage_PackVoltage_Set(App_Accumulator_GetAccumulatorVoltage(accumulator));
    App_CanTx_BMS_TractiveSystem_TsVoltage_Set(App_TractiveSystem_GetVoltage(ts));
    App_CanTx_BMS_TractiveSystem_TsCurrent_Set(App_TractiveSystem_GetCurrent(ts));
    App_CanTx_BMS_Contactors_AirNegative_Set(
        App_Airs_IsAirNegativeClosed(airs) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    App_CanTx_BMS_Contactors_AirPositive_Set(
        App_Airs_IsAirPositiveClosed(airs) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    App_SetPeriodicCanSignals_Imd(imd);

    App_AdvertisePackPower(accumulator, ts);

    App_CanTx_BMS_OkStatuses_BmsOk_Set(App_OkStatus_IsEnabled(bms_ok));
    App_CanTx_BMS_OkStatuses_ImdOk_Set(App_OkStatus_IsEnabled(imd_ok));
    App_CanTx_BMS_OkStatuses_BspdOk_Set(App_OkStatus_IsEnabled(bspd_ok));

    // Wait for cell voltage and temperature measurements to settle. We expect to read back valid values from the
    // monitoring chips within 3 cycles
    if (acc_meas_settle_count < NUM_CYCLES_TO_SETTLE)
    {
        acc_meas_settle_count++;
    }
    else if (acc_fault || ts_fault)
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
    // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
    if (App_Charger_IsConnected(charger))
    {
        if (App_Charger_HasFaulted(charger))
        {
            status = false;
            App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
    }
    else if (App_SharedStateMachine_GetCurrentState(state_machine) == App_GetChargeState())
    {
        status = false;
        App_CanTx_BMS_Faults_ChargerDisconnectedInChargeState_Set(true);
        App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
