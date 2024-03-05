#include <stddef.h>

#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "app_canUtils.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "states/app_initState.h"

static void driveStateRunOnEntry(void)
{
    app_canTx_PDM_State_set(PDM_DRIVE_STATE);
    app_powerManager_setState(POWER_MANAGER_DRIVE);
}

static void driveStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
    app_powerManager_check_efuses(POWER_MANAGER_DRIVE);
}

const State *app_driveState_get(void)
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &drive_state;
}
