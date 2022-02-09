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

#include "Io_CellTemperatures.h"
#include "Io_DieTemperatures.h"

extern uint32_t cell_temperatures[1][8];
extern float    internal_die_temp[1];

#include "Io_LTC6813.h"
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

    // Send command to start temperature conversions
    Io_LTC6813_PollAdcConversions();
    Io_CellTemperatures_GetCellTemperatureDegC();
    App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
            can_tx, (float)(cell_temperatures[0][7]));

    //// Send command to get die temperatures
    //const uint16_t ADSTAT = (0x468 + (MD << 7) + CHST);
    //Io_LTC6813_SendCommand(ADSTAT);

    //Io_LTC6813_PollAdcConversions();
    //Io_DieTemperatures_ReadTemp();
    //App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE(can_tx, internal_die_temp[0]);

    const uint16_t ADAX = 0x460 + (MD << 7) + CHG;
    Io_LTC6813_SendCommand(ADAX);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);

    UNUSED(accumulator);
    UNUSED(error_table);

    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    UNUSED(can_tx);

    //static uint16_t counter = 0U;
    //// Start voltage conversion
    //if(counter % 10 == 0U) {
    //    App_Accumulator_StartCellVoltageConversion(accumulator);
    //    App_Accumulator_ReadCellVoltages(accumulator);
    //    App_Accumulator_AllStates100Hz(accumulator, can_tx, error_table);
    //}
    //counter++;
}

#ifdef NDEBUG
if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
{
    App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
}
#endif
