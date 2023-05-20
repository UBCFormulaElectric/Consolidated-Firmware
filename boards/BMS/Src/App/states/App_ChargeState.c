#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"
#include "App_CanAlerts.h"

#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

// TODO: move value into struct
static int exit_counter = 0;

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_CHARGE_STATE);
    App_CanTx_BMS_Charger_IsChargingComplete_Set(false);
    App_Charger_Enable(charger);
    App_Charger_ResetCounterVal(charger);
    exit_counter = 0;
}

static void ChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void ChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *            world   = App_SharedStateMachine_GetWorld(state_machine);
        struct Charger *             charger = App_BmsWorld_GetCharger(world);
        struct Airs *                airs    = App_BmsWorld_GetAirs(world);
        struct TractiveSystem *const ts      = App_BmsWorld_GetTractiveSystem(world);

        const bool external_shutdown_occurred = !App_Airs_IsAirNegativeClosed(airs);
        const bool charging_enabled           = App_CanRx_Debug_ChargingSwitch_StartCharging_Get();
        bool       charging_completed         = false;
        const bool is_charger_connected       = App_Charger_IsConnected(charger);

        const uint16_t ignore_chgr_fault_counter = App_Charger_GetCounterVal(charger);
        bool           has_charger_faulted       = false;
        bool           timeout_passed            = false;

        if (ignore_chgr_fault_counter >= CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            has_charger_faulted = App_Charger_HasFaulted(charger);
            timeout_passed      = true;
            // TODO: Re-implement charging_completed
            //            if(-App_TractiveSystem_GetCurrent(ts) <=  0.85f && (App_TractiveSystem_GetCurrent(ts) < 0)){
            //                charging_completed = true;
            //            }
        }
        else
        {
            App_Charger_IncrementCounterVal(charger);
        }

        App_CanAlerts_SetFault(BMS_FAULT_CHARGER_FAULT, has_charger_faulted);

        if (has_charger_faulted)
        {
            App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }

        App_CanTx_BMS_Charger_IsChargingComplete_Set(charging_completed);
        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (!is_charger_connected || external_shutdown_occurred)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
            App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
            App_CanAlerts_SetFault(BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE, !is_charger_connected);
            App_CanAlerts_SetFault(BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN, external_shutdown_occurred);
        }
        // If the current indicates charging is complete or charging is disabled over CAN go back to init state.
        if (!charging_enabled || (charging_completed && timeout_passed))
        {
            // Charger must be diabled and given time to shut down before air positive is opened
            App_Charger_Disable(charger);
            exit_counter++;
            if (exit_counter >= 100)
            {
                App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
            }
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
    App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
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
