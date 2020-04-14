#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);
}

static void FaultStateRunOnTick(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState()
{
    static struct State fault_state = {
        .name         = "FAULT",
        .run_on_entry = FaultStateRunOnEntry,
        .run_on_tick  = FaultStateRunOnTick,
        .run_on_exit  = FaultStateRunOnExit,
    };

    return &fault_state;
}
