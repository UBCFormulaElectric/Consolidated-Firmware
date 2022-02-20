#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_Accumulator.h"
#include "Io_CellVoltages.h"

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
#include "Io_CellTemperatures.h"
#include "Io_DieTemperatures.h"
#include "Io_CellBalancing.h"

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
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);

    UNUSED(accumulator);
    UNUSED(error_table);

    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);

    static uint8_t counter = 0U;

    if (counter % 3 == 0U)
    {
        // Start voltage conversion
        App_Accumulator_ReadCellVoltages(accumulator);
        App_Accumulator_AllStates100Hz(accumulator, can_tx, error_table);

        // uint8_t min_cell_segment = 0U;
        // uint8_t min_cell_index   = 0U;
        // Io_CellVoltages_GetMinCellLocation(&min_cell_segment,
        //&min_cell_index);
        // Io_LTC6813_SetConfigurationRegister(
        //   CONFIG_REG_A, min_cell_segment, min_cell_index);
        // Io_LTC6813_SetConfigurationRegister(
        //   CONFIG_REG_B, min_cell_segment, min_cell_index);

        Io_CellBalancing_ConfigureDccBits();

        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
            can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_0));
        App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(
            can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_1));
        // App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE(
        //    can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_2));
        // App_CanTx_SetPeriodicSignal_SEGMENT_3_VOLTAGE(
        //    can_tx, Io_CellVoltages_GetSegmentVoltage(ACCUMULATOR_SEGMENT_3));

        const uint16_t ADSTAT = (0x468 + (MD << 7) + CHST);
        Io_LTC6813_SendCommand(ADSTAT);
    }
    else if (counter % 3 == 1U)
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
        const uint16_t ADAX = 0x460 + (MD << 7) + CHG;
        Io_LTC6813_SendCommand(ADAX);
    }
    else if (counter % 3 == 2U)
    {
        Io_LTC6813_PollAdcConversions();
        Io_DieTemperatures_ReadTemp();
        App_Accumulator_StartCellVoltageConversion(accumulator);
    }

    counter++;
}

#ifdef NDEBUG
if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
{
    App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
}
#endif
