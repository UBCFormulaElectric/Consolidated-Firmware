#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_FaultState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

#include "Io_Airs.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *const world =
        App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *const can_tx_interface =
        App_BmsWorld_GetCanTx(world);
    struct Airs *const airs = App_BmsWorld_GetAirs(world);

    Io_Airs_OpenAirPositive();

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);

    App_Airs_OpenAirPositive(airs);
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(
        can_tx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *const world =
        App_SharedStateMachine_GetWorld(state_machine);
    struct Airs *const       airs        = App_BmsWorld_GetAirs(world);
    struct ErrorTable *const error_table = App_BmsWorld_GetErrorTable(world);

    App_AllStatesRunOnTick1Hz(state_machine);

    if (!App_SharedErrorTable_HasAnyAirShutdownErrorSet(error_table) &&
        !App_Airs_IsAirNegativeClosed(airs))
    {
        // Transition to the init state once all AIR shutdown faults are cleared
        // and AIR- is opened
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    if (!Io_Airs_IsAirNegativeClosed())
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState()
{
    static struct State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = FaultStateRunOnEntry,
        .run_on_tick_1Hz   = FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = FaultStateRunOnTick100Hz,
        .run_on_exit       = FaultStateRunOnExit,
    };

    return &fault_state;
}
