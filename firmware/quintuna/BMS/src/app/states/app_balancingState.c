#include "states/app_states.h"

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
    const bool air_negative_open          = false;
    //io_irs_negativeState() == CONTACTOR_STATE_OPEN;
    const bool stopped_requesting_balance = !app_canRx_Debug_CellBalancingRequest_get();
    if (air_negative_open || stopped_requesting_balance)
    {
        app_stateMachine_setNextState(&init_state);
    }
}

static void balancingStateRunOnExit(void)
{
    app_canRx_Debug_CellBalancingRequest_update(false);
}

const State balancing_state = {
    .name              = "BALANCING",
    .run_on_entry      = balancingStateRunOnEntry,
    .run_on_tick_100Hz = balancingStateRunOnTick100Hz,
    .run_on_exit       = balancingStateRunOnExit,
};
