#include "states/app_driveState.h"
#include "states/app_allStates.h"

static void driveStateRunOnEntry(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

static void driveStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void driveStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick100Hz(state_machine);
}

static void driveStateRunOnExit(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

const struct State* app_driveState_get()
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
