#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
    app_powerManager_setState(POWER_MANAGER_DRIVE);
}

static void inverterOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void inverterOnStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
}

static void inverterOnStateRunOnExit(void) {}

const State *app_inverterOnState_get()
{
    static State inverter_on_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
        .run_on_exit       = inverterOnStateRunOnExit,
    };

    return &inverter_on_state;
}
