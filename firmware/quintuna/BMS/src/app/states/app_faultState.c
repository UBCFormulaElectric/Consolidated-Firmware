#include "app_faultState.h"
#include "app_allStates.h"
#include "app_precharge.h"
#include "app_segments.h"
#include "io_irs.h"
#include "io_faultLatch.h"
#include "app_canTx.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"

static void app_faultStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);

    io_irs_openPositive();
    io_faultLatch_setCurrentStatus(&bms_ok_latch, false);
}

static void app_faultStateRunOnTick100Hz(void)
{
    const bool acc_fault_cleared = !app_segments_checkFaults();
    const bool precharge_ok      = !app_precharge_limitExceeded();
    const bool bms_fault_cleared = io_faultLatch_getLatchedStatus(&bms_ok_latch);

    if (acc_fault_cleared && precharge_ok && bms_fault_cleared)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

const State *app_faultState_get(void)
{
    static State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = app_faultStateRunOnEntry,
        .run_on_tick_100Hz = app_faultStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &fault_state;
}
