#include "states/app_allStates.h"

#include "io_airs.h"
#include "io_ltc6813.h"

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_BALANCING_STATE);
}

static void runOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void runOnTick100Hz(void)
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

static void runOnExit(void)
{
    io_ltc6813_sendStopBalanceCommand(); // extra precaution
}

const State *app_balancingState_get(void)
{
    static State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnTick1Hz,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = runOnExit,
    };

    return &balancing_state;
}
