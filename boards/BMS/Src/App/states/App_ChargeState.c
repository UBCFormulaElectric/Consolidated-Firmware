#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_CHARGE_STATE);
    App_CanTx_BMS_Charger_IsChargingComplete_Set(false);
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
<<<<<<< HEAD
        struct BmsWorld *   world       = App_SharedStateMachine_GetWorld(state_machine);
        struct Charger *    charger     = App_BmsWorld_GetCharger(world);
        struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
        struct Airs *       airs        = App_BmsWorld_GetAirs(world);
        struct TractiveSystem * const ts      = App_BmsWorld_GetTractiveSystem(world);

=======
        struct BmsWorld *            world   = App_SharedStateMachine_GetWorld(state_machine);
        struct BmsCanTxInterface *   can_tx  = App_BmsWorld_GetCanTx(world);
        struct BmsCanRxInterface *   can_rx  = App_BmsWorld_GetCanRx(world);
        struct Charger *             charger = App_BmsWorld_GetCharger(world);
        struct Airs *                airs    = App_BmsWorld_GetAirs(world);
        struct TractiveSystem *const ts      = App_BmsWorld_GetTractiveSystem(world);
>>>>>>> e564c21f (Formatting)


        static uint16_t ignore_chgr_fault_counter      = 0U;
        const bool      is_charger_disconnected        = !App_Charger_IsConnected(charger);
        bool            has_charger_faulted            = false;
        bool            external_shutdown_occurred = !App_Airs_IsAirNegativeClosed(airs);
        bool            charging_enabled               = App_CanRx_CHARGING_STATUS_GetSignal_CHARGING_SWITCH(can_rx);

        if (ignore_chgr_fault_counter < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            ignore_chgr_fault_counter++;
        }
        else
        {
            has_charger_faulted = App_Charger_HasFaulted(charger);
        }

        const bool charging_completed = App_TractiveSystem_GetCurrent(ts) <= CURRENT_AT_MAX_CHARGE;

        App_CanTx_BMS_Faults_ChargerDisconnectedInChargeState_Set(is_charger_disconnected);
        App_CanTx_BMS_Faults_ChargerFault_Set(has_charger_faulted);
        App_CanTx_BMS_Charger_IsChargingComplete_Set(has_reached_max_v);
        App_CanTx_BMS_Faults_ChargingExtShutdownOccurred_Set(has_external_shutdown_occurred);


        if(has_reached_max_v||has_external_shutdown_occurred|| !charge_over_can){
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger *          charger = App_BmsWorld_GetCharger(world);
    struct BmsCanRxInterface *can_rx  = App_BmsWorld_GetCanRx(world);
    // If the charger is not turned off, or the CAN message for charging is still set to charge when exiting Charge
    // State Disable them
    App_Charger_Disable(charger);
    App_CanRx_CHARGING_STATUS_SetSignal_CHARGING_SWITCH(can_rx, false);
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
