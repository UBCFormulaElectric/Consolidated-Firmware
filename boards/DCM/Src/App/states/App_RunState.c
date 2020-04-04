#include "states/App_RunState.h"

static void runStateRunOnEnter(struct StateMachine *state_machine) {}

static void runStateRunOnTick(struct StateMachine *state_machine) {}

static void runStateRunOnExit(struct StateMachine *state_machine) {}

const struct State *App_State_getRunState()
{
    static struct State run_state;
    // TODO: figure out how to set name here
    run_state.run_on_enter = runStateRunOnEnter;
    run_state.run_on_tick  = runStateRunOnTick;
    run_state.run_on_exit  = runStateRunOnExit;

    return &run_state;
}
