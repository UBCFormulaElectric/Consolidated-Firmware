#include "app_chargeState.h"
#include "states/app_allStates.h"
#include "io_airs.h"
#include "io_charger.h"

// 0.05C is standard for a boundary to consider full charge
#define C_RATE_FOR_MAX_CHARGE (0.05f)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)
#define CURRENT_AT_MAX_CHARGE (C_RATE_FOR_MAX_CHARGE * C_RATE_TO_AMPS)

static uint16_t sixteenByteEndianSwap(uint16_t can_signal)
{
    uint16_t swapped_signal = (uint16_t)(can_signal >> 8) | (uint16_t)(can_signal << 8);

    return swapped_signal;
}

static float translateChargingParams(float charging_value)
{
    charging_value              = charging_value * 10.0f;
    uint16_t charging_value_int = (uint16_t)charging_value;

    charging_value_int = sixteenByteEndianSwap(charging_value_int);

    float charging_value_translated = (float)charging_value_int / 10.0f;

    return charging_value_translated;
}

static void chargeStateRunOnEntry(void)
{
    app_canTx_BMS_ChargerEnable_set(true);
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);
    const float mains_current = translateChargingParams(MAX_MAINS_CURRENT);
    const float batt_voltage  = translateChargingParams(CHARGING_VOLTAGE);
    const float batt_current  = translateChargingParams(CHARGING_CURRENT);
    // Setting these for run on entry right now, change later maybe.
    app_canTx_BMS_MaxMainsCurrent_set(mains_current);
    app_canTx_BMS_ChargingVoltage_set(batt_voltage);
    app_canTx_BMS_ChargingCurrent_set(batt_current);

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
        const bool is_charger_connected       = io_charger_isConnected();

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
            
            globals->charger_exit_counter++;

            // Charger must be diabled and given time to shut down before air positive is opened
            // app_canTx_BMS_ChargerEnable_set(false);

            // HERE FOR TESTING PURPOSES
            // Seemed to be hitting this multiple times and going between init and charge state a few times before settling in charge state and charging
            // without this line and the charger disable above it will charge without issue.
            // We exit charge state with the charger shutdown timeout instead. It seems to be getting the wrong Debug charging enable signal
            // for the first few cycles of charge state and disabling the charger immediately. However because of the counter for exiting the state
            // it will not go back to init state as it does not exceed 100U.
            // app_stateMachine_setNextState(app_initState_get());

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
    io_charger_enable(false);
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
