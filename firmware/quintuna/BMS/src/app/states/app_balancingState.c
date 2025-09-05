#include "app_stateMachine.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "io_irs.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_ltc6813.h"

static void balancingStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_BALANCING_STATE);
}

static void balancingStateRunOnTick100Hz(void)
{
    const bool air_negative_open          = !io_irs_isNegativeClosed();
    const bool stopped_requesting_balance = !app_canRx_Debug_CellBalancingRequest_get();
    if (stopped_requesting_balance)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void balancingStateRunOnExit(void)
{
    app_canRx_Debug_CellBalancingRequest_update(false);
}

const State *app_balancingState_get(void)
{
    static State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = balancingStateRunOnEntry,
        .run_on_tick_100Hz = balancingStateRunOnTick100Hz,
        .run_on_exit       = balancingStateRunOnExit,
    };

    return &balancing_state;
}
