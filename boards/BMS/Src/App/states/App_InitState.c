#include "states/App_InitState.h"
#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void InitStateRunOnTick(struct StateMachine *state_machine)
{
    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
}

static void InitStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState()
{
    static struct State init_state = {
        .name         = "INIT",
        .run_on_enter = InitStateRunOnEntry,
        .run_on_tick  = InitStateRunOnTick,
        .run_on_exit  = InitStateRunOnExit,
    };

    return &init_state;
}
