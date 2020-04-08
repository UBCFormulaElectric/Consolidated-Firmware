#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void FaultStateRunOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void FaultStateRunOnExit(struct StateMachine *state_machine)
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
