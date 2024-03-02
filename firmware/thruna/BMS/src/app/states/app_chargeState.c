#include "states/app_allStates.h"
#include "io_airs.h"
#include "io_charger.h"

// 0.05C is standard for a boundary to consider full charge
#define CURRENT_AT_MAX_CHARGE (0.05f * C_RATE_TO_AMPS)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)

static void chargeStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);
    app_canTx_BMS_ChargerEnable_set(true);

    globals->ignore_charger_fault_counter = 0;
    globals->charger_exit_counter         = 0;
}

static void chargeStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void chargeStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        const bool external_shutdown_occurred = !io_airs_isNegativeClosed();
        const bool charging_enabled           = app_canRx_Debug_StartCharging_get();
        const bool is_charger_connected       = app_canRx_BRUSA_OnStatus_get();

        bool has_charger_faulted = false;
        if (globals->ignore_charger_fault_counter >= CYCLES_TO_IGNORE_CHGR_FAULT)
        {
            has_charger_faulted = app_canRx_BRUSA_Error_get();
        }
        else
        {
            globals->ignore_charger_fault_counter++;
        }

        app_canAlerts_BMS_Fault_Charger_set(has_charger_faulted);

        if (has_charger_faulted)
        {
            app_stateMachine_setNextState(app_faultState_get());
        }

        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (!is_charger_connected || external_shutdown_occurred)
        {
            app_stateMachine_setNextState(app_faultState_get());
            app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_set(!is_charger_connected);
            app_canAlerts_BMS_Fault_ChargerExternalShutdown_set(external_shutdown_occurred);
        }
        // If charging is disabled over CAN go back to init state.
        if (!charging_enabled)
        {
            // Charger must be diabled and given time to shut down before air positive is opened
            app_canTx_BMS_ChargerEnable_set(false);
            globals->charger_exit_counter++;

            if (globals->charger_exit_counter >= CHARGER_SHUTDOWN_TIMEOUT)
            {
                app_stateMachine_setNextState(app_initState_get());
            }
        }
    }
}

static void chargeStateRunOnExit(void)
{
    app_canTx_BMS_ChargerEnable_set(false);
    io_airs_openPositive();
    app_canRx_Debug_StartCharging_update(false);
}

const State *app_chargeState_get(void)
{
    static State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = chargeStateRunOnEntry,
        .run_on_tick_1Hz   = chargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = chargeStateRunOnTick100Hz,
        .run_on_exit       = chargeStateRunOnExit,
    };

    return &charge_state;
}
