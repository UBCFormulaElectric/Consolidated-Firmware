#include "states/app_allStates.h"

#include "io_airs.h"
#include "io_ltc6813.h"

static void balancingStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_BALANCING_STATE);
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
        const bool stopped_requesting_balance = !app_canRx_Debug_CellBalancingRequest_get();
        if (air_negative_open || stopped_requesting_balance)
        {
            app_stateMachine_setNextState(app_initState_get());
        }
    }
}

static void balancingStateRunOnExit(void)
{
    io_ltc6813_sendStopBalanceCommand(); // extra precaution
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
