#include "states/app_allStates.h"
#include "app_accumulator.h"
#include "app_utils.h"
#include "io_airs.h"

static void balancingStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_BALANCING_STATE);
    app_accumulator_enableBalancing(true);
}

static void balancingStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void balancingStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        const bool air_negative_open          = !io_airs_isNegativeClosed();
        const bool stopped_requesting_balance =  false; //!app_canRx_Debug_CellBalancingRequest_get();
        if (air_negative_open || stopped_requesting_balance)
        {
            app_stateMachine_setNextState(app_initState_get());
        }
    }
}

static void balancingStateRunOnExit(void)
{
    app_accumulator_enableBalancing(false);
}

const State *app_balancingState_get(void)
{
    static State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = balancingStateRunOnEntry,
        .run_on_tick_1Hz   = balancingStateRunOnTick1Hz,
        .run_on_tick_100Hz = balancingStateRunOnTick100Hz,
        .run_on_exit       = balancingStateRunOnExit,
    };

    return &balancing_state;
}
