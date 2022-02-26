#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"

// static void App_AllStatesCanTxPeriodic100Hz(struct BmsWorld *world)
//{
//    struct BmsCanTxInterface *can_tx  = App_BmsWorld_GetCanTx(world);
//    struct Imd *              imd     = App_BmsWorld_GetImd(world);
//    struct OkStatus *         bms_ok  = App_BmsWorld_GetBmsOkStatus(world);
//    struct OkStatus *         imd_ok  = App_BmsWorld_GetImdOkStatus(world);
//    struct OkStatus *         bspd_ok = App_BmsWorld_GetBspdOkStatus(world);
//    struct Airs *             airs    = App_BmsWorld_GetAirs(world);
//
//    App_SetPeriodicCanSignals_Imd(can_tx, imd);
//
//    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(
//        can_tx,
//        App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)));
//    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(
//        can_tx,
//        App_SharedBinaryStatus_IsActive(App_Airs_GetAirPositive(airs)));
//
//    App_CanTx_SetPeriodicSignal_BMS_OK(can_tx,
//    App_OkStatus_IsEnabled(bms_ok));
//    App_CanTx_SetPeriodicSignal_IMD_OK(can_tx,
//    App_OkStatus_IsEnabled(imd_ok)); App_CanTx_SetPeriodicSignal_BSPD_OK(
//        can_tx, App_OkStatus_IsEnabled(bspd_ok));
//}

#include "Io_LTC6813.h"
#include "Io_CellVoltages.h"
#include "Io_CellTemperatures.h"
#include "Io_DieTemperatures.h"
#include "App_Accumulator.h"

enum CellMonitorState
{
    GET_CELL_VOLTAGE = 0U,
    GET_CELL_TEMP,
    GET_DIE_TEMP,
};

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(
        can_tx, App_Charger_IsConnected(charger));
}

// TODO: Make this nicer
static inline uint8_t temp_scale_for_can(float temp)
{
    return (uint8_t)(temp * (127U / 63.5f));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);

    UNUSED(accumulator);
    UNUSED(error_table);

    static enum CellMonitorState action = GET_CELL_VOLTAGE;

    if (action == GET_CELL_VOLTAGE)
    {
        // Start voltage conversion
        App_Accumulator_ReadCellVoltages(accumulator);

        Io_LTC6813_WriteConfigurationRegisters();

        App_Accumulator_AllStates100Hz(accumulator, can_tx, error_table);

        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
            can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_0));
        // App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(
        //    can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_1));
        // App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE(
        //    can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_2));
        // App_CanTx_SetPeriodicSignal_SEGMENT_3_VOLTAGE(
        //    can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_3));

        Io_CellVoltages_StartCellTemperatureConversion();
        action = GET_CELL_TEMP;
    }
    else if (action == GET_CELL_TEMP)
    {
        // Send command to start temperature conversions
        Io_LTC6813_PollAdcConversions();
        Io_CellTemperatures_GetCellTemperatureDegC();

        App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_DEGC(
            can_tx,
            temp_scale_for_can(Io_CellTemperatures_GetMaxCellTempDegC()));
        App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_DEGC(
            can_tx,
            temp_scale_for_can(Io_CellTemperatures_GetMinCellTempDegC()));
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_TEMP_DEGC(
            can_tx,
            temp_scale_for_can(Io_CellTemperatures_GetAverageCellTempDegC()));

        // Get the min max segment locations
        uint8_t segment    = 0U;
        uint8_t thermistor = 0U;
        Io_CellTemperatures_GetMinCellLocation(&segment, &thermistor);
        App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_SEGMENT(can_tx, segment);
        App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_THERM(can_tx, thermistor);

        Io_CellTemperatures_GetMaxCellLocation(&segment, &thermistor);
        App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_SEGMENT(can_tx, segment);
        App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_THERM(can_tx, thermistor);

        // Send command to get die temperatures
        Io_DieTemperatures_StartDieTempConversion();
        action = GET_DIE_TEMP;
    }
    else if (action == GET_DIE_TEMP)
    {
        Io_DieTemperatures_ReadTemp();

        // Start cell voltage conversions
        App_Accumulator_StartCellVoltageConversion(accumulator);

        // TODO: broadcast max die temp
        App_CanTx_SetPeriodicSignal_SEGMENT_5_VOLTAGE(
            can_tx, Io_DieTemperatures_GetMaxDieTemp());

        action = GET_CELL_VOLTAGE;
    }
}

#ifdef NDEBUG
if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
{
    App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
}
#endif
