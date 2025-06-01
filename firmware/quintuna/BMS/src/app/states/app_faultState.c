#include "app_faultState.h"
#include <stddef.h>
#include "app_precharge.h"
#include "io_irs.h"
#include "io_faultLatch.h"
#include "app_tractiveSystem.h"
#include "states/app_initState.h"

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);

    io_irs_openPositive();
    io_faultLatch_setCurrentStatus(&bms_ok_latch, false);
}

static void runOnTick1Hz(void) {}

static void runOnTick100Hz(void)
{
    const bool acc_fault_cleared = false; // TODO: Accumulator app module !app_accumulator_checkFaults();
    const bool precharge_ok      = !app_precharge_limitExceeded();
    const bool air_negative_open = !io_irs_isNegativeClosed();

    if (acc_fault_cleared && precharge_ok && air_negative_open)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
}

static void runOnExit(void) {}

const State *app_faultState_get(void)
{
    static State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnTick1Hz,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = runOnExit,
    };

    return &fault_state;
}
