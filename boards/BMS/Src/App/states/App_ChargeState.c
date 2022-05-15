#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_BalanceState.h"
#include "App_SharedMacros.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)
#define MIN_BALANCING_CELL_VOLTAGE (3.8f)

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger          = App_BmsWorld_GetCharger(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
    App_CanTx_SetPeriodicSignal_IS_CHARGING_COMPLETE(can_tx_interface, false);
    App_Charger_Enable(charger);
}

static void ChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void ChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
        struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
        struct Charger *          charger     = App_BmsWorld_GetCharger(world);
        struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
        struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);

        const struct State *next_state = App_GetChargeState();

        static uint16_t ignore_chgr_fault_counter = 0U;
        const bool      is_charger_disconnected   = !App_Charger_IsConnected(charger);
        bool            has_charger_faulted       = false;

        if (ignore_chgr_fault_counter < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            ignore_chgr_fault_counter++;
        }
        else
        {
            has_charger_faulted = App_Charger_HasFaulted(charger);
        }

        uint8_t     segment          = 0U;
        uint8_t     cell             = 0U;
        const bool has_reached_max_v = App_Accumulator_GetMaxVoltage(accumulator, &segment, &cell) > MAX_CELL_V_TARGET;
        App_CanTx_SetPeriodicSignal_IS_CHARGING_COMPLETE(can_tx, has_reached_max_v);

        App_CanTx_SetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(can_tx, is_charger_disconnected);
        App_CanTx_SetPeriodicSignal_CHARGER_FAULT_DETECTED(can_tx, has_charger_faulted);
        App_SharedErrorTable_SetError(
            error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, is_charger_disconnected);
        App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_CHARGER_FAULT_DETECTED, has_charger_faulted);
        App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_HAS_REACHED_MAX_V, has_reached_max_v);

        const float min_cell_v       = App_Accumulator_GetMinVoltage(accumulator, &segment, &cell);
        const bool  is_pack_balanced = (App_Accumulator_GetMaxVoltage(accumulator, &segment, &cell) - min_cell_v) <=
                                       CONVERT_100UV_TO_VOLTAGE(CELL_VOLTAGE_DISCHARGE_WINDOW_UV);

        if (is_charger_disconnected || has_charger_faulted)
        {
            next_state = App_GetFaultState();
        }
        else if (has_reached_max_v || (!is_pack_balanced && (min_cell_v > MIN_BALANCING_CELL_VOLTAGE)))
        {
            next_state = App_GetBalanceState();
        }

        App_SharedStateMachine_SetNextState(state_machine, next_state);
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_Charger_Disable(charger);
}

const struct State *App_GetChargeState(void)
{
    static struct State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = ChargeStateRunOnEntry,
        .run_on_tick_1Hz   = ChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = ChargeStateRunOnTick100Hz,
        .run_on_exit       = ChargeStateRunOnExit,
    };

    return &charge_state;
}
