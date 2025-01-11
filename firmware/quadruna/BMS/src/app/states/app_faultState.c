#include "states/app_allStates.h"
#include "app_utils.h"
#include "io_airs.h"
#include "io_faultLatch.h"
#include "io_airs.h"

static void faultStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);
    app_canAlerts_BMS_Fault_StateMachine_set(true);

    io_airs_openPositive();
    io_faultLatch_setCurrentStatus(&bms_ok_latch, false);
}

static void faultStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void faultStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        const bool acc_fault_cleared = !app_accumulator_checkFaults();
        const bool ts_fault_cleared  = !app_tractveSystem_checkFaults();
        const bool precharge_ok      = !globals->precharge_limit_exceeded;
        const bool air_negative_open = !io_airs_isNegativeClosed();

        if (acc_fault_cleared && ts_fault_cleared && precharge_ok && air_negative_open)
        {
            app_stateMachine_setNextState(app_initState_get());
        }
    }
}

static void faultStateRunOnExit(void)
{
    app_canAlerts_BMS_Fault_StateMachine_set(false);
}

const State *app_faultState_get()
{
    static State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = faultStateRunOnEntry,
        .run_on_tick_1Hz   = faultStateRunOnTick1Hz,
        .run_on_tick_100Hz = faultStateRunOnTick100Hz,
        .run_on_exit       = faultStateRunOnExit,
    };

    return &fault_state;
}
