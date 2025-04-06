#include "app_driveWarningState.h"

static void driveWarningStateRunOnEntry(void) {}
static void driveWarningStateRunOnTick1Hz(void) {}
static void driveWarningStateRunOnTick100Hz(void) {}
static void driveWarningStateRunOnExit(void) {}

const State *app_driveWarningState_get(void)
{
    static State driveWarning_state = { .name              = "DRIVE WARNING",
                                        .run_on_entry      = driveWarningStateRunOnEntry,
                                        .run_on_tick_1Hz   = driveWarningStateRunOnTick1Hz,
                                        .run_on_tick_100Hz = driveWarningStateRunOnTick100Hz,
                                        .run_on_exit       = driveWarningStateRunOnExit };

    return &driveWarning_state;
}
