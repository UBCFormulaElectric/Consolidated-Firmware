#include "states/App_AllStates.h"
#include "states/App_InitState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void BalancingStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_BALANCING_STATE);
    App_Accumulator_EnableBalancing(accumulator, true);
}

static void BalancingStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void BalancingStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Airs *    airs  = App_BmsWorld_GetAirs(world);

        const bool air_negative_open          = !App_Airs_IsAirNegativeClosed(airs);
        const bool stopped_requesting_balance = !App_CanRx_Debug_CellBalancing_RequestCellBalancing_Get();
        if (air_negative_open || stopped_requesting_balance)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void BalancingStateRunOnTick1kHz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1kHz(state_machine);
}

static void BalancingStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    App_Accumulator_EnableBalancing(accumulator, false);
}

const struct State *App_GetBalancingState(void)
{
    static struct State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = BalancingStateRunOnEntry,
        .run_on_tick_1Hz   = BalancingStateRunOnTick1Hz,
        .run_on_tick_100Hz = BalancingStateRunOnTick100Hz,
        .run_on_tick_1kHz  = BalancingStateRunOnTick1kHz,
        .run_on_exit       = BalancingStateRunOnExit,
    };

    return &balancing_state;
}
