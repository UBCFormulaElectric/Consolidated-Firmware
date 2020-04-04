#include "states/App_InitState.h"
#include "states/App_RunState.h"

#include "unused.h"

static void initStateRunOnEnter(struct StateMachine *state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DCMCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    struct CanMsgs_dcm_startup_t payload = { .dummy = 0 };
    App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(can_tx_interface, &payload);
}

static void initStateRunOnTick(struct StateMachine *state_machine)
{
    // No need for any safety checks, just run! (this is a demo)
    App_SharedStateMachine_SetNextState(state_machine, App_GetRunState());
}

static void initStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState()
{
    static struct State initial_state = {
        .name         = "INIT",
        .run_on_enter = initStateRunOnEnter,
        .run_on_tick  = initStateRunOnTick,
        .run_on_exit  = initStateRunOnExit,
    };

    return &initial_state;
}
