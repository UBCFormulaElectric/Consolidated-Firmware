#include "states/app_states.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "io_irs.h"

static void driveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
}

static void driveStateRunOnTick100Hz(void) {}

static void driveStateRunOnExit(void)
{
    // IR+ opens upon exiting drive state
    io_irs_setPositive(CONTACTOR_STATE_OPEN);
}

const State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};