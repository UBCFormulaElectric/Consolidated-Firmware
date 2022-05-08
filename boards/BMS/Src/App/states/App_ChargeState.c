#include "states/App_AllStates.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "App_Accumulator.h"

#include "App_SharedMacros.h"

#define MAX_CELL_VOLTAGE_THRESHOLD (4.0f) // Charging is complete when the max cell voltage reaches this threshold

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger          = App_BmsWorld_GetCharger(world);
    struct Accumulator *      accumulator      = App_BmsWorld_GetAccumulator(world);

    // Disable cell discharging before enabling BRUSA charging
    App_Accumulator_DisableDischarge(accumulator);
    App_Charger_Enable(charger);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
}

static void ChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void ChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger     = App_BmsWorld_GetCharger(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);

    // Enter the fault state once one of these conditions are true
    // and indicate the reason for the transition over CAN:
    // - Charger cable has disconnected
    // - Charger has faulted
    // - Maximum cell voltage reaches MAX_CELL_VOLTAGE_THRESHOLD V (Charging complete)
    if (!App_Charger_IsConnected(charger) || App_Charger_HasFaulted(charger) ||
        App_Accumulator_GetMaxVoltage(accumulator, NULL, NULL) > MAX_CELL_VOLTAGE_THRESHOLD)
    {
        if (!App_Charger_IsConnected(charger))
        {
            App_CanTx_SetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(can_tx, true);
        }
        if (App_Charger_HasFaulted(charger))
        {
            App_CanTx_SetPeriodicSignal_CHARGER_FAULT_DETECTED(can_tx, true);
        }
        if (App_Accumulator_GetMaxVoltage(accumulator, NULL, NULL) > MAX_CELL_VOLTAGE_THRESHOLD)
        {
            App_CanTx_SetPeriodicSignal_CHARGING_DONE(can_tx, true);
        }

        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger *    charger     = App_BmsWorld_GetCharger(world);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);

    // Stop BRUSA charging and enable cell discharging
    App_Charger_Disable(charger);
    App_Accumulator_EnableDischarge(accumulator);
}

const struct State *App_GetChargeState(void)
{
    static struct State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = ChargeStateRunOnEntry,
        .run_on_tick_1Hz   = ChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = ChargeStateRunOnTick100Hz,
        .run_on_exit       = ChargeStateRunOnExit,
    };

    return &charge_state;
}
