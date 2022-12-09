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

static void App_SendAndReceiveHeartbeat(
    struct BmsCanTxInterface *can_tx,
    struct BmsCanRxInterface *can_rx,
    struct HeartbeatMonitor * hb_monitor)
{
    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (App_CanRx_FSM_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, FSM_HEARTBEAT_ONE_HOT);
        App_CanRx_FSM_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    if (App_CanRx_DCM_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, DCM_HEARTBEAT_ONE_HOT);
        App_CanRx_DCM_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    if (App_CanRx_PDM_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, PDM_HEARTBEAT_ONE_HOT);
        App_CanRx_PDM_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }
}

static void App_CheckCellVoltageRange(struct BmsCanTxInterface *can_tx, struct Accumulator *accumulator)
{
    // Get the min and max cell voltages and check to see if the voltages are in range
    uint8_t     min_segment           = 0U;
    uint8_t     min_loc               = 0U;
    uint8_t     max_segment           = 0U;
    uint8_t     max_loc               = 0U;
    const float curr_min_cell_voltage = App_Accumulator_GetMinVoltage(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_voltage = App_Accumulator_GetMaxVoltage(accumulator, &max_segment, &max_loc);

    App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE(can_tx, curr_min_cell_voltage);
    App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE(can_tx, curr_max_cell_voltage);
    App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_SEGMENT(can_tx, min_segment);
    App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_IDX(can_tx, min_loc);
    App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_SEGMENT(can_tx, max_segment);
    App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_IDX(can_tx, max_loc);
}

static void App_CheckCellTemperatureRange(
    struct BmsCanTxInterface *can_tx,
    struct Accumulator *      accumulator,
    struct StateMachine *     state_machine)
{
    // Get the min and max cell temperature and check to see if the temperatures
    // are in range
    uint8_t     min_segment        = 0U;
    uint8_t     min_loc            = 0U;
    uint8_t     max_segment        = 0U;
    uint8_t     max_loc            = 0U;
    const float curr_min_cell_temp = App_Accumulator_GetMinCellTempDegC(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_temp = App_Accumulator_GetMaxCellTempDegC(accumulator, &max_segment, &max_loc);

    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMPERATURE(can_tx, curr_min_cell_temp);
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMPERATURE(can_tx, curr_max_cell_temp);
    App_CanTx_SetPeriodicSignal_MIN_TEMP_SEGMENT(can_tx, min_segment);
    App_CanTx_SetPeriodicSignal_MIN_TEMP_IDX(can_tx, min_loc);
    App_CanTx_SetPeriodicSignal_MAX_TEMP_SEGMENT(can_tx, max_segment);
    App_CanTx_SetPeriodicSignal_MAX_TEMP_IDX(can_tx, max_loc);
}

static void
    App_AdvertisePackPower(struct BmsCanTxInterface *can_tx, struct Accumulator *accumulator, struct TractiveSystem *ts)
{
    uint8_t segment = 0;
    UNUSED(segment);

    const float max_cell_temp = App_Accumulator_GetMaxCellTempDegC(accumulator, &segment, &segment);
    const float available_power =
        min(App_SharedProcessing_LinearDerating(
                max_cell_temp, MAX_POWER_LIMIT_W, CELL_ROLL_OFF_TEMP_DEGC, CELL_FULLY_DERATED_TEMP),
            MAX_POWER_LIMIT_W);

    App_CanTx_SetPeriodicSignal_AVAILABLE_POWER(can_tx, available_power);
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx           = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence = App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *          charger          = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(can_tx, charger_is_connected);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct BmsCanRxInterface *can_rx      = App_BmsWorld_GetCanRx(world);
    struct Imd *              imd         = App_BmsWorld_GetImd(world);
    struct OkStatus *         bms_ok      = App_BmsWorld_GetBmsOkStatus(world);
    struct OkStatus *         imd_ok      = App_BmsWorld_GetImdOkStatus(world);
    struct OkStatus *         bspd_ok     = App_BmsWorld_GetBspdOkStatus(world);
    struct Airs *             airs        = App_BmsWorld_GetAirs(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct HeartbeatMonitor * hb_monitor  = App_BmsWorld_GetHeartbeatMonitor(world);
    struct TractiveSystem *   ts          = App_BmsWorld_GetTractiveSystem(world);

    bool           status                = true;
    static uint8_t acc_meas_settle_count = 0U;
    App_SendAndReceiveHeartbeat(can_tx, can_rx, hb_monitor);

    App_Accumulator_RunOnTick100Hz(accumulator);
    App_CheckCellVoltageRange(can_tx, accumulator);
    App_CheckCellTemperatureRange(can_tx, accumulator, state_machine);

    const bool acc_fault = App_Accumulator_CheckFaults(can_tx, accumulator, ts);
    const bool ts_fault  = App_TractveSystem_CheckFaults(can_tx, ts);

    App_CanTx_SetPeriodicSignal_PACK_VOLTAGE(can_tx, App_Accumulator_GetPackVoltage(accumulator));
    App_CanTx_SetPeriodicSignal_TS_VOLTAGE(can_tx, App_TractiveSystem_GetVoltage(ts));
    App_CanTx_SetPeriodicSignal_TS_CURRENT(can_tx, App_TractiveSystem_GetCurrent(ts));
    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(can_tx, App_Airs_IsAirNegativeClosed(airs));
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(can_tx, App_Airs_IsAirPositiveClosed(airs));
    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    App_AdvertisePackPower(can_tx, accumulator, ts);

    App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, App_OkStatus_IsEnabled(bms_ok));
    App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, App_OkStatus_IsEnabled(imd_ok));
    App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, App_OkStatus_IsEnabled(bspd_ok));

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

    return status;
}
