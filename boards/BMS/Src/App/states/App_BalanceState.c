#include <stdlib.h>
#include "states/App_InitState.h"
#include "states/App_BalanceState.h"
#include "states/App_AllStates.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "states/App_FaultState.h"

#define Low_Cut_Off_Voltage (400U)
//#define Bal_Enable_Threshold (400U)
//#define Bal_Disable_Threshold (400U)

static void BalanceStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Airs *             airs   = App_BmsWorld_GetAirs(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_BALANCE_CHOICE);
    App_Airs_OpenAirPositive(airs);
}

static void BalanceStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct BmsCanRxInterface *can_rx      = App_BmsWorld_GetCanRx(world);

    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        uint8_t segment = 0U;
        uint8_t cell    = 0U;

        const float min_cell_v = App_Accumulator_GetMinVoltage(accumulator, &segment, &cell);
        if (min_cell_v <= Low_Cut_Off_Voltage)
        {
            const float target_voltage = Low_Cut_Off_Voltage;
        }
        else
        {
            const float target_voltage = min_cell_v;
        }

        const float voltage_diff_to_target = 400U;

        const float max_cell_v       = App_Accumulator_GetMaxVoltage(accumulator, &segment, &cell);

        const bool  is_pack_balanced = (max_cell_v - App_Accumulator_GetMinVoltage(accumulator, &segment, &cell)) <=
                                      CONVERT_100UV_TO_VOLTAGE(CELL_VOLTAGE_DISCHARGE_WINDOW_UV);

        const struct State *next_state = App_GetBalanceState();

        if (is_pack_balanced && (max_cell_v >= MAX_CELL_V_TARGET))
        {
            // Set a flag signifying that the pack is balanced. Skip charging state while we wait in init state
            App_Accumulator_SetPackFullyChargedStatus(accumulator, true);

            // Disable cell balancing
            App_Accumulator_SetCellBalancingStatus(accumulator, false);

            next_state = App_GetInitState();
        }
        else
        {
            bool is_balancing_enabled = true;

            // Overriding cell balancing, transition back into the init state
            if (App_CanRx_CAN_DEBUGGING_SIGNALS_GetSignal_DISABLE_CELL_BALANCING(can_rx))
            {
                App_Accumulator_SetCellBalancingStatus(accumulator, false);
                next_state = App_GetInitState();
            }

            // Set a flag signifying that the pack is NOT fully charged. Remain in discharge state until the pack is
            App_Accumulator_SetPackFullyChargedStatus(accumulator, false);

            // Enable cell balancing
            App_Accumulator_SetCellBalancingStatus(accumulator, is_balancing_enabled);
        }

        App_SharedStateMachine_SetNextState(state_machine, next_state);
    }
}

const struct State *App_GetBalanceState(void)
{
    static struct State balance_state = {
        .name              = "BALANCE",
        .run_on_entry      = BalanceStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = BalanceStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &balance_state;
}