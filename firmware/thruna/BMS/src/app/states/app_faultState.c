#include "states/app_allStates.h"
#include "app_utils.h"
#include "io_airs.h"
#include "io_faultLatch.h"
#include "io_airs.h"

static void faultStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_BMS_State_Set(BMS_FAULT_STATE);
    App_CanAlerts_BMS_Fault_StateMachine_Set(true);

    io_airs_openPositive();
    io_faultLatch_setCurrentStatus(globals->config->bms_ok_latch, false);
}

static void faultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void faultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        const bool acc_fault_cleared = !app_accumulator_checkFaults();
        const bool ts_fault_cleared  = !app_tractveSystem_checkFaults();
        const bool precharge_ok      = !globals->precharge_limit_exceeded;
        const bool air_negative_open = !io_airs_isNegativeClosed();

        if (acc_fault_cleared && ts_fault_cleared && precharge_ok && air_negative_open)
        {
            app_stateMachine_setNextState(state_machine, app_initState_get());
        }
    }
}

static void faultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    App_CanAlerts_BMS_Fault_StateMachine_Set(false);
}

const struct State *app_faultState_get()
{
    static struct State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = faultStateRunOnEntry,
        .run_on_tick_1Hz   = faultStateRunOnTick1Hz,
        .run_on_tick_100Hz = faultStateRunOnTick100Hz,
        .run_on_exit       = faultStateRunOnExit,
    };

    return &fault_state;
}
