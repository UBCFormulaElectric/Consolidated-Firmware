#include "states/App_InitState.h"
#include "states/App_BalanceState.h"
#include "states/App_AllStates.h"

static void BalanceStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct Airs *             airs        = App_BmsWorld_GetAirs(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_BALANCE_CHOICE);
    App_Airs_OpenAirPositive(airs);

    // Enable cell balancing as we enter the balance state
    App_Accumulator_SetIsCellBalancingEnabled(accumulator, true);
}

static void BalanceStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void BalanceStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct BmsCanRxInterface *can_rx      = App_BmsWorld_GetCanRx(world);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);

    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        const struct State *next_state           = App_GetBalanceState();
        bool                is_charging_complete = false;

        if (App_CanRx_CAN_DEBUGGING_SIGNALS_GetSignal_DISABLE_CELL_BALANCING(can_rx))
        {
            // If cell balancing is manually disabled on CAN, transition to the init state
            next_state = App_GetInitState();
        }
        else if (App_Accumulator_IsPackFullyCharged(accumulator))
        {
            // Notify on CAN when charging is complete
            is_charging_complete = true;
            next_state           = App_GetInitState();
        }
        else if (App_Accumulator_IsPackBalanced(accumulator))
        {
            // Pack is not yet fully charged, but the pack is balanced. Return to the init state to be charged
            next_state = App_GetInitState();
        }

        App_CanTx_SetPeriodicSignal_IS_CHARGING_COMPLETE(can_tx, is_charging_complete);
        App_SharedStateMachine_SetNextState(state_machine, next_state);
    }
}

static void BalanceStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);

    // Disable cell balancing as we leave the balance state
    App_Accumulator_SetIsCellBalancingEnabled(accumulator, false);
}

const struct State *App_GetBalanceState(void)
{
    static struct State balance_state = {
        .name              = "BALANCE",
        .run_on_entry      = BalanceStateRunOnEntry,
        .run_on_tick_1Hz   = BalanceStateRunOnTick1Hz,
        .run_on_tick_100Hz = BalanceStateRunOnTick100Hz,
        .run_on_exit       = BalanceStateRunOnExit,
    };

    return &balance_state;
}
