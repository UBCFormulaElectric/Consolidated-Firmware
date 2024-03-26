#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"

static void driveStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_setState(POWER_MANAGER_DRIVE);
}

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
