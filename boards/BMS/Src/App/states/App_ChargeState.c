#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"

#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_CHARGE_STATE);
    App_CanTx_BMS_Charger_IsChargingComplete_Set(false);
    App_Charger_Enable(charger);
    App_Charger_ResetCounterVal(charger);
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

        bool       external_shutdown_occurred = !App_Airs_IsAirNegativeClosed(airs);
        bool       charging_enabled           = App_CanRx_DEBUG_ChargingSwitch_StartCharging_Get();
        const bool charging_completed         = App_TractiveSystem_GetCurrent(ts) <= CURRENT_AT_MAX_CHARGE;
        const bool is_charger_connected       = App_Charger_IsConnected(charger);

        // Increment charger fault ignore counter
        if (App_Charger_GetCounterVal(charger) < CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            App_Charger_IncrementCounterVal(charger);
        }

        App_CanTx_BMS_Charger_IsChargingComplete_Set(charging_completed);
        App_CanTx_BMS_Faults_ChargingExtShutdownOccurred_Set(external_shutdown_occurred);

        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (!is_charger_connected || external_shutdown_occurred || external_shutdown_occurred)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
            App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);
            App_CanTx_BMS_Faults_ChargerDisconnectedInChargeState_Set(!is_charger_connected);
        }
        // If the current indicates charging is complete or charging is disabled over CAN go back to init state.
        if (!charging_enabled)
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
