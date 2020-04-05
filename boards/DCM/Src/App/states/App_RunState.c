#include "states/App_RunState.h"
#include "App_SharedMacros.h"

static void runStateRunOnEnter(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void runStateRunOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void runStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetRunState()
{
    static struct State run_state = {
        .name         = "RUN",
        .run_on_enter = runStateRunOnEnter,
        .run_on_tick  = runStateRunOnTick,
        .run_on_exit  = runStateRunOnExit,
    };

    return &run_state;
}
