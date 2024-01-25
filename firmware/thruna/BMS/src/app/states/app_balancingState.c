#include "states/app_allStates.h"
#include "app_accumulator.h"
#include "App_SharedMacros.h"
#include "io_airs.h"

static void balancingStateRunOnEntry(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
    App_CanTx_BMS_State_Set(BMS_BALANCING_STATE);
    app_accumulator_enableBalancing(true);
}

static void balancingStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void balancingStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        const bool air_negative_open          = !io_airs_isNegativeClosed();
        const bool stopped_requesting_balance = !App_CanRx_Debug_CellBalancingRequest_Get();
        if (air_negative_open || stopped_requesting_balance)
        {
            App_SharedStateMachine_SetNextState(state_machine, app_initState_get());
        }
    }
}

static void balancingStateRunOnExit(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
    app_accumulator_enableBalancing(false);
}

const struct State* app_balancingState_get(void)
{
    static struct State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = balancingStateRunOnEntry,
        .run_on_tick_1Hz   = balancingStateRunOnTick1Hz,
        .run_on_tick_100Hz = balancingStateRunOnTick100Hz,
        .run_on_exit       = balancingStateRunOnExit,
    };

    return &balancing_state;
}
