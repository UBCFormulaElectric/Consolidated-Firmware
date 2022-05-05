#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_Accumulator.h"

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

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
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

    App_Accumulator_RunOnTick100Hz(accumulator);

    const bool has_comms_error = App_Accumulator_HasCommunicationError(accumulator);
    App_CanTx_SetPeriodicSignal_HAS_PEC_ERROR(can_tx, has_comms_error);
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_HAS_PEC_ERROR, has_comms_error);

    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)));
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirPositive(airs)));

    App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, App_OkStatus_IsEnabled(bms_ok));
    App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, App_OkStatus_IsEnabled(imd_ok));
    App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, App_OkStatus_IsEnabled(bspd_ok));

    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}
