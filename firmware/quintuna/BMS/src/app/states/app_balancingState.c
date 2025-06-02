#include "app_stateMachine.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_irs.h"
#include "io_ltc6813.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"

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
    const bool air_negative_open          = !io_irs_isNegativeClosed();
    const bool stopped_requesting_balance = !app_canRx_Debug_CellBalancingRequest_get();
    if (air_negative_open || stopped_requesting_balance)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void runOnExit(void)
{
    io_ltc6813_sendStopBalanceCommand();
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
