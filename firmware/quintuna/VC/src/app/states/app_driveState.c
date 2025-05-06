#include "app_allStates.h"
#include "app_states.h"

static void driveStateRunOnEntry(void) {}

static void driveStateRunOnTick1Hz(void) {}

static void driveStateRunOnTick100Hz(void) {}

static void driveStateRunOnExit(void) {}

State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};
