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
        struct BmsWorld *            world       = App_SharedStateMachine_GetWorld(state_machine);
        struct Charger *             charger     = App_BmsWorld_GetCharger(world);
        struct Airs *                airs        = App_BmsWorld_GetAirs(world);
        struct TractiveSystem *const ts          = App_BmsWorld_GetTractiveSystem(world);

        static uint16_t ignore_chgr_fault_counter  = 0U;
        bool            has_charger_faulted        = false;
        bool            external_shutdown_occurred = !App_Airs_IsAirNegativeClosed(airs);
        bool            charging_enabled           = App_CanRx_DEBUG_ChargingSwitch_StartCharging_Get();

        if (ignore_chgr_fault_counter < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            ignore_chgr_fault_counter++;
        }
        else
        {
            has_charger_faulted = App_Charger_HasFaulted(charger);
        }

        const bool charging_completed = App_TractiveSystem_GetCurrent(ts) <= CURRENT_AT_MAX_CHARGE;
        App_CanTx_BMS_Faults_ChargerFault_Set(has_charger_faulted);
        App_CanTx_BMS_Charger_IsChargingComplete_Set(charging_completed);
        App_CanTx_BMS_Faults_ChargingExtShutdownOccurred_Set(external_shutdown_occurred);

        struct HeartbeatMonitor *hb_monitor    = App_BmsWorld_GetHeartbeatMonitor(world);
        const bool               is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
        App_CanTx_BMS_Warnings_MissingHeartBeat_Set(is_missing_hb);

        if (has_charger_faulted || external_shutdown_occurred || is_missing_hb)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
        // If the current indicates charging is complete or charging is disabled over CAN go back to init state.
        if (charging_completed || !charging_enabled)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);
    struct Airs *    airs    = App_BmsWorld_GetAirs(world);
    // If the charger is not turned off, or the CAN message for charging is still set to charge when exiting Charge
    // state, disable them when leaving
    // Airs+ also has to be opened once again since it is closed in Pre-Charge state for charging/driving
    App_Charger_Disable(charger);
    App_Airs_OpenAirPositive(airs);
    App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);
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
