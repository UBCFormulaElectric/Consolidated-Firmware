#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void AirClosedStateRunOnEntry(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void AirClosedStateRunOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void AirClosedStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirClosedState(void)
{
    static struct State air_closed_state = {
        .name         = "FAULT",
        .run_on_entry = AirClosedStateRunOnEntry,
        .run_on_tick  = AirClosedStateRunOnTick,
        .run_on_exit  = AirClosedStateRunOnExit,
    };

    return &air_closed_state;
}
