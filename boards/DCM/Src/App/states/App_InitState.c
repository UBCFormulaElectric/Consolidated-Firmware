#include "states/App_InitState.h"
#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void initStateRunOnEntry(struct StateMachine *state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void initStateRunOnTick(struct StateMachine *state_machine)
{
    // No need for any safety checks, just run! (this is a demo)
    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
}

static void initStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState()
{
    static struct State init_state = {
        .name         = "INIT",
        .run_on_entry = initStateRunOnEntry,
        .run_on_tick  = initStateRunOnTick,
        .run_on_exit  = initStateRunOnExit,
    };

    return &init_state;
}
