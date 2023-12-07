#include <math.h>

#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "App_SharedMacros.h"
#include "App_CanUtils.h"
#include "app_powerManager.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "states/app_initState.h"

static void driveStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_PDM_State_Set(PDM_DRIVE_STATE);
    app_powerManager_setState(POWER_MANAGER_CONTRACTOR_DRIVE);
}

static void driveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void driveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick100Hz(state_machine);

    if (App_CanRx_BMS_State_Get() != BMS_DRIVE_STATE)
    {
        App_SharedStateMachine_SetNextState(state_machine, app_initState_get());
    }
}

static void driveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_driveState_get()
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
