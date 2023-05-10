#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"
#include "App_CanAlerts.h"

#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static bool has_seen_current;

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_CHARGE_STATE);
    App_CanTx_BMS_Charger_IsChargingComplete_Set(false);
    App_Charger_Enable(charger);
    App_Charger_ResetCounterVal(charger);
    has_seen_current = false;
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
        const bool charging_completed         = App_TractiveSystem_GetCurrent(ts) <= CURRENT_AT_MAX_CHARGE;
        const bool is_charger_connected       = App_Charger_IsConnected(charger);

        if (App_TractiveSystem_GetCurrent(ts) > 1.0f)
        {
            has_seen_current = true;
        }

        // Increment charger fault ignore counter
        if (App_Charger_GetCounterVal(charger) < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            App_Charger_IncrementCounterVal(charger);
        }

        App_CanTx_BMS_Charger_IsChargingComplete_Set(charging_completed);
        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (!is_charger_connected || external_shutdown_occurred)
        {
            if(!is_charger_connected)
            {
                App_CanTx_BMS_LatchedFaults_ChargerDisconnectedDuringCharge_Set(true);
            }
            if(external_shutdown_occurred)
            {
                App_CanTx_BMS_LatchedFaults_ChargerExternalShutdown_Set(true);
            }

            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
            App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
            App_CanAlerts_SetFault(BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE, !is_charger_connected);
            App_CanAlerts_SetFault(BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN, external_shutdown_occurred);
        }
        // If the current indicates charging is complete or charging is disabled over CAN go back to init state.
        if (!charging_enabled || (charging_completed && has_seen_current))
        {
//                        App_SharedStateMachine_SetNextState(state_machine, App_GetfState());
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
