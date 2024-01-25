#include <math.h>
#include "states/app_initState.h"
#include "states/app_allStates.h"

static void initStateRunOnEntry(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

static void initStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void initStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick100Hz(state_machine);
}

static void initStateRunOnExit(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

const struct State* app_initState_get()
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = initStateRunOnExit,
    };

    return &init_state;
}
