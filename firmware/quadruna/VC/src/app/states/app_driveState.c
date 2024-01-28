#include "states/app_driveState.h"
#include "states/app_allStates.h"

static void driveStateRunOnEntry(void) {}

static void driveStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void driveStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
}

static void driveStateRunOnExit(void) {}

const State *app_driveState_get()
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
