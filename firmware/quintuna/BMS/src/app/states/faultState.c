#include "app/states/states.h"

#include "app/precharge.h"
#include "app/segments.h"
#include "io/irs.h"
#include "io/faultLatch.h"
#include "app_canTx.h"

static void app_faultStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);
    io_irs_setPositive(CONTACTOR_STATE_OPEN);
}

static void app_faultStateRunOnTick100Hz(void)
{
#ifdef TARGET_HV_SUPPLY
    const bool acc_fault_cleared = true;
#else
    const bool acc_fault_cleared = !app_segments_checkFaults();
#endif
    // const bool precharge_ok      = !app_precharge_limitExceeded();
    const bool bms_fault_cleared = io_faultLatch_getLatchedStatus(&bms_ok_latch) == FAULT_LATCH_OK;

    if (acc_fault_cleared && bms_fault_cleared)
    {
        app_stateMachine_setNextState(&init_state);
    }
}

const State fault_state = {
    .name              = "FAULT",
    .run_on_entry      = app_faultStateRunOnEntry,
    .run_on_tick_100Hz = app_faultStateRunOnTick100Hz,
    .run_on_exit       = NULL,
};
