#include "states/App_AllStates.h"
#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE);
}

static void FaultStateRunOnTick(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick(state_machine);
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name         = "FAULT",
        .run_on_entry = FaultStateRunOnEntry,
        .run_on_tick  = FaultStateRunOnTick,
        .run_on_exit  = FaultStateRunOnExit,
    };

    return &fault_state;
}
