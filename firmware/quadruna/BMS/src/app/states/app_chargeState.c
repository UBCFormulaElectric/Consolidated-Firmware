#include "app_chargeState.h"
#include "states/app_allStates.h"
#include "io_airs.h"
#include "io_charger.h"

// 0.05C is standard for a boundary to consider full charge
#define C_RATE_FOR_MAX_CHARGE (0.05f)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)
#define CURRENT_AT_MAX_CHARGE (C_RATE_FOR_MAX_CHARGE * C_RATE_TO_AMPS)
#define MAX_CHARGING_VOLTAGE (336.0f)
// Each cell can handle 11.8A per the Datasheet, x3 in parallel = 35.4A, Setting as 15A for safety (limited by mains
// current at this stage)
#define MAX_CHARGING_CURRENT (15.0f)

static uint16_t canMsgEndianSwap(uint16_t can_signal)
{
    uint16_t swapped_signal = (uint16_t)(can_signal >> 8) | (uint16_t)(can_signal << 8);

    return swapped_signal;
}

static float translateChargingParams(float charging_value)
{
    charging_value              = charging_value * 10.0f;
    uint16_t charging_value_int = (uint16_t)charging_value;

    charging_value_int = canMsgEndianSwap(charging_value_int);

    float charging_value_translated = (float)charging_value_int / 10.0f;

    return charging_value_translated;
}

static void chargeStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);
    app_canTx_BMS_ChargerEnable_set(true);

    globals->ignore_charger_fault_counter = 0;
    globals->charger_exit_counter         = 0;
    globals->charger_connected_counter    = 0;
    const float mains_current             = translateChargingParams(INITIAL_MAX_MAINS_CURRENT);
    const float batt_voltage              = translateChargingParams(INITIAL_CHARGING_VOLTAGE);
    const float batt_current              = translateChargingParams(INITIAL_CHARGING_CURRENT);
    // Setting these for run on entry right now, change later maybe.
    app_canTx_BMS_MaxMainsCurrent_set(mains_current);
    app_canTx_BMS_ChargingVoltage_set(batt_voltage);
    app_canTx_BMS_ChargingCurrent_set(batt_current);
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
        const bool is_charger_connected       = app_canRx_BRUSA_IsConnected_get();

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

        globals->charger_connected_counter++;
        if (globals->charger_connected_counter >= 100)
        {
            app_canRx_BRUSA_IsConnected_update(false);
            globals->charger_connected_counter = 0;
        }
        else if (globals->charger_connected_counter >= 50)
        {
            if (!is_charger_connected)
            {
                app_stateMachine_setNextState(app_faultState_get());
                app_canAlerts_BMS_Fault_ChargerDisconnectedDuringCharge_set(!is_charger_connected);
            }
        }

        // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
        if (external_shutdown_occurred)
        {
            app_stateMachine_setNextState(app_faultState_get());
            app_canAlerts_BMS_Fault_ChargerExternalShutdown_set(external_shutdown_occurred);
        }
        // If charging is disabled over CAN go back to init state.
        if (!charging_enabled)
        {
            // Charger must be diabled and given time to shut down before air positive is opened
            globals->charger_exit_counter++;
            if (globals->charger_exit_counter >= PREMATURE_EXIT_FILTER_THRESHOLD)
            {
                app_canTx_BMS_ChargerEnable_set(false);
            }
            if (globals->charger_exit_counter >= CHARGER_SHUTDOWN_TIMEOUT)
            {
                app_stateMachine_setNextState(app_initState_get());
            }
        }

        // Override based on CAN parameters
        const float charging_current = app_canRx_Debug_ChargingCurrentOverride_get()
                                           ? app_canRx_Debug_ChargingCurrentTargetValue_get()
                                           : INITIAL_CHARGING_VOLTAGE;

        if (IS_IN_RANGE(0.0f, MAX_CHARGING_CURRENT, charging_current))
        {
            app_canTx_BMS_ChargingCurrent_set(translateChargingParams(charging_current));
        }

        const float charging_voltage = app_canRx_Debug_ChargingVoltageOverride_get()
                                           ? app_canRx_Debug_ChargingVoltageTargetValue_get()
                                           : INITIAL_CHARGING_VOLTAGE;

        if (IS_IN_RANGE(0.0f, MAX_CHARGING_VOLTAGE, charging_voltage))
        {
            app_canTx_BMS_ChargingVoltage_set(translateChargingParams(charging_voltage));
        }
    }
}

static void chargeStateRunOnExit(void)
{
    globals->charger_connected_counter    = 0;
    globals->charger_exit_counter         = 0;
    globals->ignore_charger_fault_counter = 0;
    io_charger_enable(false);
    io_airs_openPositive();
    app_canRx_Debug_StartCharging_update(false);
    app_canTx_BMS_ChargerEnable_set(false);
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
