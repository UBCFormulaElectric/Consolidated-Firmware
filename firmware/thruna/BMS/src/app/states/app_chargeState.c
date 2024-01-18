#include "states/app_allStates.h"
#include "io_airs.h"
#include "io_charger.h"

// 0.05C is standard for a boundary to consider full charge
#define CURRENT_AT_MAX_CHARGE (0.05f * C_RATE_TO_AMPS)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static void chargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_BMS_State_Set(BMS_CHARGE_STATE);
    io_charger_enable(true);

    globals->ignore_charger_fault_counter = 0;
    globals->charger_exit_counter         = 0;
}

static void chargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void chargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        const bool external_shutdown_occurred = !io_airs_isNegativeClosed();
        const bool charging_enabled           = App_CanRx_Debug_StartCharging_Get();
        const bool is_charger_connected       = io_charger_isConnected();

        bool has_charger_faulted = false;
        if (globals->ignore_charger_fault_counter >= CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            has_charger_faulted = io_charger_hasFaulted();
        }
        else
        {
            globals->ignore_charger_fault_counter++;
        }

        App_CanAlerts_BMS_Fault_Charger_Set(has_charger_faulted);

        if (has_charger_faulted)
        {
            app_stateMachine_setNextState(state_machine, app_faultState_get());
        }

        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (!is_charger_connected || external_shutdown_occurred)
        {
            app_stateMachine_setNextState(state_machine, app_faultState_get());
            App_CanAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_Set(!is_charger_connected);
            App_CanAlerts_BMS_Fault_ChargerExternalShutdown_Set(external_shutdown_occurred);
        }
        // If charging is disabled over CAN go back to init state.
        if (!charging_enabled)
        {
            // Charger must be diabled and given time to shut down before air positive is opened
            io_charger_enable(false);
            globals->charger_exit_counter++;

            if (globals->charger_exit_counter >= CHARGER_SHUTDOWN_TIMEOUT)
            {
                app_stateMachine_setNextState(state_machine, app_initState_get());
            }
        }
    }
}

static void chargeStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);

    io_charger_enable(false);
    io_airs_openPositive();
    App_CanRx_Debug_StartCharging_Update(false);
}

const struct State *app_chargeState_get(void)
{
    static struct State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = chargeStateRunOnEntry,
        .run_on_tick_1Hz   = chargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = chargeStateRunOnTick100Hz,
        .run_on_exit       = chargeStateRunOnExit,
    };

    return &charge_state;
}
