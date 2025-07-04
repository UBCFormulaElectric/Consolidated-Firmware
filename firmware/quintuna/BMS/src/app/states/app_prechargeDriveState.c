#include "app_precharge.h"
#include "app_canTx.h"
#include "io_irs.h"
#include "app_canRx.h"
#include "app_prechargeLatchState.h"
#include "app_initState.h"
#include "app_driveState.h"
#include "app_allStates.h"

static void app_prechargeDriveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_DRIVE_STATE);

    io_irs_closePrecharge();
    app_precharge_restart();
}

static void app_prechargeDriveStateRunOnTick100Hz(void)
{
    const PrechargeState state = app_precharge_poll(false);

    if (state == PRECHARGE_STATE_FAILED_CRITICAL)
    {
        app_stateMachine_setNextState(app_prechargeLatchState_get());
    }
    else if (state == PRECHARGE_STATE_FAILED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (state == PRECHARGE_STATE_SUCCESS)
    {
        // Precharge successful, close positive contactor.

        // i am not sure this is smart? cause if all states overrides into fault state, does it close contactors in
        // time?
        // also on the other hand you want to close positive before you open precharge, so i guess?? but i think the
        // time between the two should be very minimal since they get handled in the onentry and onexit of each
        // state transition, which happens consecutively
        io_irs_closePositive();

        app_stateMachine_setNextState(app_driveState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void app_prechargeDriveStateRunOnExit(void)
{
    io_irs_openPrecharge();
}

const State *app_prechargeDriveState_get(void)
{
    static State precharge_drive_state = {
        .name              = "PRECHARGE DRIVE",
        .run_on_entry      = app_prechargeDriveStateRunOnEntry,
        .run_on_tick_100Hz = app_prechargeDriveStateRunOnTick100Hz,
        .run_on_exit       = app_prechargeDriveStateRunOnExit,
    };

    return &precharge_drive_state;
}
