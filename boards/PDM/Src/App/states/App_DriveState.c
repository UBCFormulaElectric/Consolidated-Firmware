#include "states/App_DriveState.h"

#include "unused.h"

static void DriveStateRunOnEntry(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void DriveStateRunOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void DriveStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState()
{
    static struct State drive_state = {
        .name         = "DRIVE",
        .run_on_enter = DriveStateRunOnEntry,
        .run_on_tick  = DriveStateRunOnTick,
        .run_on_exit  = DriveStateRunOnExit,
    };

    return &drive_state;
}
