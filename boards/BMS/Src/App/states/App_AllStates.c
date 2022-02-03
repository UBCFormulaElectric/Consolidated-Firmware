#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"

//static void App_AllStatesCanTxPeriodic100Hz(struct BmsWorld *world)
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
//        can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)));
//    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(
//        can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirPositive(airs)));
//
//    App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, App_OkStatus_IsEnabled(bms_ok));
//    App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, App_OkStatus_IsEnabled(imd_ok));
//    App_CanTx_SetPeriodicSignal_BSPD_OK(
//        can_tx, App_OkStatus_IsEnabled(bspd_ok));
//}

#include "Io_CellTemperatures.h"

extern uint16_t raw_therm_v[2][8];
extern uint32_t cell_temperatures[2][8];

#include "Io_LTC6813.h"
void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    Io_LTC6813_PollAdcConversions();
    Io_CellTemperatures_ReadTemperatures();

    const uint16_t ADAX = 0x460 + (MD << 7) + CHG;
    Io_LTC6813_SendCommand(ADAX);
    App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(can_tx, (float) (raw_therm_v[0][0]));

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(
        can_tx, App_Charger_IsConnected(charger));

    //const uint16_t ADAX = 0x460 + (MD << 7) + CHG;
    //Io_LTC6813_SendCommand(ADAX);
    //if(Io_LTC6813_PollAdcConversions())
    //{
    //    Io_CellTemperatures_ReadTemperatures();
    //    App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE(
    //            can_tx, (float)(raw_therm_v[0][0]));
    //}
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);

    static uint32_t counter = 0U;

    if(counter % 10U) {

        // Read current accumulator cell voltages
        App_Accumulator_ReadCellVoltages(accumulator);
        App_Accumulator_AllStates100Hz(accumulator, can_tx, error_table);

        //App_AllStatesCanTxPeriodic100Hz(world);

        // Update accumulator cell voltages
        App_Accumulator_StartCellVoltageConversion(accumulator);
    }

    counter++;

#ifdef NDEBUG
    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
#endif
}
