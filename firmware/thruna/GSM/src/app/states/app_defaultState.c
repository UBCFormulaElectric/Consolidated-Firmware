#include "states/app_defaultState.h"
#include "App_SharedMacros.h"

static void defaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void defaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void defaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void defaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_defaultState_get(void)
{
    static struct State air_open_state = {
        .name              = "DEFAULT",
        .run_on_entry      = defaultStateRunOnEntry,
        .run_on_tick_1Hz   = defaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = defaultStateRunOnTick100Hz,
        .run_on_exit       = defaultStateRunOnExit,
    };

    return &air_open_state;
}
