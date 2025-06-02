#include "states/app_prechargeForDriveState.h"
#include <stddef.h>
#include "app_precharge.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "io_irs.h"
#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "states/app_faultState.h"
#include "states/app_initState.h"

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_FOR_DRIVE_STATE);

    // Close precharge relay.
    io_irs_closePrecharge();
    app_precharge_restart();
}

static void runOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void runOnTick100Hz(void)
{
    const bool           precharge_for_charging = false;
    const PrechargeState state                  = app_precharge_poll(precharge_for_charging);

    if (state == PRECHARGE_STATE_FAILED_CRITICAL)
    {
        app_stateMachine_setNextState(app_faultState_get());
    }
    else if (state == PRECHARGE_STATE_FAILED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (state == PRECHARGE_STATE_SUCCESS)
    {
        app_stateMachine_setNextState(app_driveState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void runOnExit(void)
{
    // If we entered precharge to charge, disable this request on exit.
    app_canRx_Debug_StartCharging_update(false);

    // Open precharge relay.
    io_irs_openPrecharge();
}

const State *app_prechargeForDriveState_get(void)
{
    static State pre_charge_for_drive_state = {
        .name              = "PRECHARGE_DRIVE",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnTick1Hz,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = runOnExit,
    };

    return &pre_charge_for_drive_state;
}
