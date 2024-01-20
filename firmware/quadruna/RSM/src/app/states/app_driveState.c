#include <string.h>
#include "states/app_driveState.h"
#include <stdlib.h>
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedMacros.h"
#include "app_globals.h"
#include "App_CommitInfo.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888)

static void driveStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void driveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void driveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_CanTx_RSM_Heartbeat_Set(true);
}

static void driveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_driveState_get(void)
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
