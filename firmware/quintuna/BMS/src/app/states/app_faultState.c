#include "app_faultState.h"
#include "app_allStates.h"
#include "app_precharge.h"
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

static void app_faultStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void app_faultStateRunOnTick100Hz(void)
{
    const bool acc_fault_cleared = false; // !app_accumulator_checkFaults();
    const bool precharge_ok      = !app_precharge_limitExceeded();
    const bool air_negative_open = !io_irs_isNegativeClosed();

    if (acc_fault_cleared && precharge_ok && air_negative_open)
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
        .run_on_tick_1Hz   = app_faultStateRunOnTick1Hz,
        .run_on_tick_100Hz = app_faultStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &fault_state;
}
