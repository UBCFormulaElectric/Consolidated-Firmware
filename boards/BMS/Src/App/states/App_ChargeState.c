#include <stm32f3xx.h>
#include "main.h"
#include "states/App_AllStates.h"
#include "states/App_FaultState.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
    App_CanTx_SetPeriodicSignal_IS_CHARGING_COMPLETE(can_tx_interface, false);

    // Turn on the charger
    HAL_GPIO_WritePin(BRUSA_PON_GPIO_Port, BRUSA_PON_Pin, GPIO_PIN_SET);
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
        struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
        struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);
        struct Airs *             airs        = App_BmsWorld_GetAirs(world);

        static uint16_t ignore_chgr_fault_counter = 0U;
        const bool      is_charger_disconnected =
            HAL_GPIO_ReadPin(CHARGE_STATE_GPIO_Port, CHARGE_STATE_Pin) == GPIO_PIN_RESET;
        bool has_charger_faulted            = false;
        bool has_external_shutdown_occurred = !App_Airs_IsAirNegativeClosed(airs);

        if (ignore_chgr_fault_counter < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            ignore_chgr_fault_counter++;
        }
        else
        {
            has_charger_faulted = HAL_GPIO_ReadPin(BRUSA_FLT_GPIO_Port, BRUSA_FLT_Pin) == GPIO_PIN_RESET;
        }

        uint8_t    segment = 0U;
        uint8_t    cell    = 0U;
        const bool has_reached_max_v =
            App_Accumulator_GetMaxVoltage(accumulator, &segment, &cell) > MAX_CELL_VOLTAGE_THRESHOLD;

        App_CanTx_SetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(can_tx, is_charger_disconnected);
        App_CanTx_SetPeriodicSignal_CHARGER_FAULT_DETECTED(can_tx, has_charger_faulted);
        App_CanTx_SetPeriodicSignal_IS_CHARGING_COMPLETE(can_tx, has_reached_max_v);
        App_CanTx_SetPeriodicSignal_CHARGING_EXT_SHUTDOWN_OCCURRED(can_tx, has_external_shutdown_occurred);

        App_SharedErrorTable_SetError(
            error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, is_charger_disconnected);
        App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_CHARGER_FAULT_DETECTED, has_charger_faulted);
        App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_HAS_REACHED_MAX_V, has_reached_max_v);
        App_SharedErrorTable_SetError(
            error_table, BMS_AIR_SHUTDOWN_CHARGING_EXT_SHUTDOWN_OCCURRED, has_external_shutdown_occurred);

        if (is_charger_disconnected || has_charger_faulted || has_reached_max_v || has_external_shutdown_occurred)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    // Turn off the charger
    HAL_GPIO_WritePin(BRUSA_PON_GPIO_Port, BRUSA_PON_Pin, GPIO_PIN_RESET);
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
