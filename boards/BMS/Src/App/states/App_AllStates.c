#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *    charger     = App_BmsWorld_GetCharger(world);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    App_SetPeriodicSignals_AccumulatorTemperaturesInRangeChecks(
        can_tx, accumulator);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(can_tx, charger_is_connected);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct OkStatus *         bms_ok      = App_BmsWorld_GetBmsOkStatus(world);
    struct OkStatus *         imd_ok      = App_BmsWorld_GetImdOkStatus(world);
    struct OkStatus *         bspd_ok     = App_BmsWorld_GetBspdOkStatus(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct BinaryStatus *     air_negative = App_BmsWorld_GetAirNegative(world);
    struct BinaryStatus *     air_positive = App_BmsWorld_GetAirPositive(world);

    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(
        can_tx, App_SharedBinaryStatus_IsActive(air_negative));
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(
        can_tx, App_SharedBinaryStatus_IsActive(air_positive));

    if (App_OkStatus_IsEnabled(bms_ok))
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(imd_ok))
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(bspd_ok))
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, false);
    }

    App_SetPeriodicSignals_AccumulatorVoltagesInRangeChecks(
        can_tx, accumulator);
    if (App_CanTx_GetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE(can_tx) !=
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE ||
        App_CanTx_GetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE(can_tx) !=
            CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}
