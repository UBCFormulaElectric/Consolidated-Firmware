#include "app_chargeState.h"
#include "app_utils.h"
#include "io_irs.h"
#include "app_canTx.h"
#include "io_ltc6813.h"
#include "states/app_faultState.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canAlerts.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)

// Time to allow charger to disable before opening airs
#define CHARGER_SHUTDOWN_TIMEOUT (100U)
#define PREMATURE_EXIT_FILTER_THRESHOLD (20U)

// TODO: Add Error decoding for BRUSA by rounding out the whole rx signal
// from the charger in JSON, and charging profiles
#define INITIAL_CHARGING_VOLTAGE (268.8f)
#define INITIAL_CHARGING_CURRENT (12.5f)

// 0.05C is standard for a boundary to consider full charge
#define C_RATE_FOR_MAX_CHARGE (0.05f)
#define MAX_CELL_VOLTAGE_THRESHOLD (4.15f)
#define CURRENT_AT_MAX_CHARGE (C_RATE_FOR_MAX_CHARGE * C_RATE_TO_AMPS)
#define MAX_CELL_VOLTAGE_NOMINAL (4.2f)
#define MAX_CHARGING_VOLTAGE ((float)(NUM_SEGMENTS * CELLS_PER_SEGMENT * MAX_CELL_VOLTAGE_NOMINAL)) // 268.8V

// Each cell can handle 11.8A per the Datasheet, x3 in parallel = 35.4A, Setting as 15A for safety (limited by mains
// current at this stage)
#define MAX_CHARGING_CURRENT (15.0f)

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);
    app_canTx_BMS_Charging_set(true);

    // Setting these for run on entry right now, change later maybe.
    app_canTx_BMS_MaxChargingVoltage_set(INITIAL_CHARGING_VOLTAGE);
    app_canTx_BMS_MaxChargingCurrent_set(INITIAL_CHARGING_CURRENT);
}

static void runOnTick1Hz(void) {}

static void runOnTick100Hz(void)
{
    const bool has_charger_faulted =
        app_canRx_Elcon_HardwareFailure_get() || app_canRx_Elcon_ChargerOverTemperature_get() ||
        app_canRx_Elcon_InputVoltageError_get() || app_canRx_Elcon_CommunicationTimeout_get();
    app_canAlerts_BMS_Fault_ElconError_set(has_charger_faulted);
    if (has_charger_faulted)
    {
        app_stateMachine_setNextState(app_faultState_get());
    }

    // Checks if the charger has thrown a fault, the disabling of the charger, etc is done with ChargeStateRunOnExit
    const bool external_shutdown_occurred = !io_irs_isNegativeClosed();
    app_canAlerts_BMS_Fault_ChargerShutdownLoopOpen_set(external_shutdown_occurred);
    if (external_shutdown_occurred)
    {
        app_stateMachine_setNextState(app_faultState_get());
    }

    // If charging is disabled over CAN go back to init state.
    const bool charging_enabled = app_canRx_Debug_StartCharging_get();
    if (!charging_enabled)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Override based on CAN parameters
    const float charging_current = app_canRx_Debug_ChargingCurrentOverride_get()
                                       ? app_canRx_Debug_ChargingCurrentTargetValue_get()
                                       : INITIAL_CHARGING_CURRENT;
    if (IS_IN_RANGE(0.0f, MAX_CHARGING_CURRENT, charging_current))
    {
        app_canTx_BMS_MaxChargingCurrent_set(charging_current);
    }

    const float charging_voltage = app_canRx_Debug_ChargingVoltageOverride_get()
                                       ? app_canRx_Debug_ChargingVoltageTargetValue_get()
                                       : INITIAL_CHARGING_VOLTAGE;
    if (IS_IN_RANGE(0.0f, MAX_CHARGING_VOLTAGE, charging_voltage))
    {
        app_canTx_BMS_MaxChargingVoltage_set(charging_voltage);
    }
}

static void runOnExit(void)
{
    io_irs_openPositive();

    app_canTx_BMS_MaxChargingCurrent_set(0);
    app_canTx_BMS_MaxChargingVoltage_set(0);
    app_canRx_Debug_StartCharging_update(false);
    app_canTx_BMS_Charging_set(false);
}

const State *app_chargeState_get(void)
{
    static State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnTick1Hz,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = runOnExit,
    };

    return &charge_state;
}
