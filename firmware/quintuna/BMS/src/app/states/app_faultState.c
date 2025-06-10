#include "states/app_states.h"

#include "app_precharge.h"
#include "io_irs.h"
#include "io_faultLatch.h"
#include "app_canTx.h"

static void app_faultStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);

    io_irs_setPositive(false);
    io_faultLatch_setCurrentStatus(&bms_ok_latch, false);
}

static void app_faultStateRunOnTick100Hz(void)
{
    const bool acc_fault_cleared = true; //! app_accumulator_checkFaults();
    const bool bms_fault_cleared = io_faultLatch_getLatchedStatus(&bms_ok_latch);

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
