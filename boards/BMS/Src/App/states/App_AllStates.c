#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_Accumulator.h"
#include "App_SharedMacros.h"

#define MIN_CELL_VOLTAGE (3.0f)
#define MAX_CELL_VOLTAGE (4.2f)
#define DEFAULT_MIN_CELL_TEMP_DEGC (0.0f)
#define DEFAULT_MAX_CELL_TEMP_DEGC (60.0f)

// Num of cycles for voltage and cell temperature values to settle
#define NUM_CYCLES_TO_SETTLE (1U)

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
    struct Imd *              imd         = App_BmsWorld_GetImd(world);
    struct OkStatus *         bms_ok      = App_BmsWorld_GetBmsOkStatus(world);
    struct OkStatus *         imd_ok      = App_BmsWorld_GetImdOkStatus(world);
    struct OkStatus *         bspd_ok     = App_BmsWorld_GetBspdOkStatus(world);
    struct Airs *             airs        = App_BmsWorld_GetAirs(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);

    bool           status                = true;
    static uint8_t settling_time_counter = 0U;

    App_Accumulator_RunOnTick100Hz(accumulator);

    uint8_t min_segment = 0U;
    uint8_t min_loc     = 0U;
    uint8_t max_segment = 0U;
    uint8_t max_loc     = 0U;

    // Get the min and max cell voltages and check to see if the voltages are in range
    const float curr_min_cell_voltage      = App_Accumulator_GetMinVoltage(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_voltage      = App_Accumulator_GetMaxVoltage(accumulator, &max_segment, &max_loc);
    const bool  is_min_cell_v_out_of_range = !IS_IN_RANGE(MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE, curr_min_cell_voltage);
    const bool  is_max_cell_v_out_of_range = !IS_IN_RANGE(MIN_CELL_VOLTAGE, MAX_CELL_VOLTAGE, curr_max_cell_voltage);
    App_SharedErrorTable_SetError(
        error_table, BMS_AIR_SHUTDOWN_MAX_CELL_VOLTAGE_OUT_OF_RANGE, is_max_cell_v_out_of_range);
    App_SharedErrorTable_SetError(
        error_table, BMS_AIR_SHUTDOWN_MIN_CELL_VOLTAGE_OUT_OF_RANGE, is_min_cell_v_out_of_range);
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_HAS_PEC_ERROR, is_max_cell_v_out_of_range);

    App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE(can_tx, curr_min_cell_voltage);
    App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE(can_tx, curr_max_cell_voltage);
    App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE(can_tx, is_min_cell_v_out_of_range);
    App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE(can_tx, is_max_cell_v_out_of_range);
    App_CanTx_SetPeriodicSignal_MIN_CELL_SEGMENT(can_tx, min_segment);
    App_CanTx_SetPeriodicSignal_MIN_CELL_IDX(can_tx, min_loc);
    App_CanTx_SetPeriodicSignal_MAX_CELL_SEGMENT(can_tx, max_segment);
    App_CanTx_SetPeriodicSignal_MAX_CELL_IDX(can_tx, max_loc);

    // Get the min and max cell temperature and check to see if the temperatures
    // are in range
    // TODO: broadcast values via CAN
    const float curr_min_cell_temp = App_Accumulator_GetMinCellTempDegC(accumulator, &min_segment, &min_loc);
    const float curr_max_cell_temp = App_Accumulator_GetMaxCellTempDegC(accumulator, &max_segment, &max_loc);
    const bool  is_min_cell_out_of_range =
        !IS_IN_RANGE(DEFAULT_MIN_CELL_TEMP_DEGC, DEFAULT_MAX_CELL_TEMP_DEGC, curr_min_cell_temp);
    const bool is_max_cell_out_of_range =
        !IS_IN_RANGE(DEFAULT_MIN_CELL_TEMP_DEGC, DEFAULT_MAX_CELL_TEMP_DEGC, curr_max_cell_temp);

    // Check if cell temperatures are out of bounds. If out of bounds, transition to the fault state
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_MIN_CELL_TEMP_OUT_OF_RANGE, is_min_cell_out_of_range);
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_MAX_CELL_TEMP_OUT_OF_RANGE, is_max_cell_out_of_range);
    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE(can_tx, is_min_cell_out_of_range);
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE(can_tx, is_max_cell_out_of_range);
    App_CanTx_SetPeriodicSignal_MIN_CELL_TEMPERATURE(can_tx, curr_min_cell_temp);
    App_CanTx_SetPeriodicSignal_MAX_CELL_TEMPERATURE(can_tx, curr_max_cell_temp);

    const bool has_acc_comms_error = App_Accumulator_HasCommunicationError(accumulator);
    App_CanTx_SetPeriodicSignal_HAS_PEC_ERROR(can_tx, has_acc_comms_error);
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_HAS_PEC_ERROR, has_acc_comms_error);

    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)));
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirPositive(airs)));

    App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, App_OkStatus_IsEnabled(bms_ok));
    App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, App_OkStatus_IsEnabled(imd_ok));
    App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, App_OkStatus_IsEnabled(bspd_ok));

    if (settling_time_counter < NUM_CYCLES_TO_SETTLE)
    {
        settling_time_counter++;
    }
    else if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
