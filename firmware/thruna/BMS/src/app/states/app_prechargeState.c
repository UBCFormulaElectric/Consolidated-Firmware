#include "states/app_prechargeState.h"
#include "io_airs.h"

// When TS reaches this threshold of ACC, AIR+ engages, and precharge is disabled, the voltage must be at least 90%
// EV.6.6.1
#define PRECHARGE_ACC_V_THRESHOLD (0.90f)

#define MAX_PRECHARGE_ATTEMPTS 3U

static bool checkPrechargeFaults(
    bool is_charger_connected,
    bool is_ts_rising_slowly,
    bool is_ts_rising_quickly,
    bool is_air_negative_open)
{
    bool has_precharge_fault =
        (is_charger_connected) ? is_ts_rising_slowly : (is_ts_rising_slowly | is_ts_rising_quickly);
    has_precharge_fault |= is_air_negative_open;

    if (has_precharge_fault && globals->num_precharge_failures < MAX_PRECHARGE_ATTEMPTS)
    {
        globals->num_precharge_failures++;
    }

    globals->precharge_limit_exceeded = globals->num_precharge_failures >= MAX_PRECHARGE_ATTEMPTS;
    App_CanAlerts_BMS_Fault_PrechargeFailure_Set(globals->precharge_limit_exceeded);

    return has_precharge_fault;
}

static void preChargeStateRunOnEntry(struct StateMachine* const state_machine)
{
    App_CanTx_BMS_State_Set(BMS_PRECHARGE_STATE);
    io_airs_closePrecharge();

    // Restart timers for checking if we're precharging too slow/quick.
    App_Timer_Restart(&globals->precharge_lower_bound_timer);
    App_Timer_Restart(&globals->precharge_upper_bound_timer);
}

static void preChargeStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void preChargeStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        float ts_voltage        = app_tractiveSystem_getVoltage();
        float threshold_voltage = app_accumulator_getPackVoltage() * PRECHARGE_ACC_V_THRESHOLD;

        const bool is_charger_connected = io_charger_isConnected();
        const bool is_air_negative_open = !io_airs_isNegativeClosed();
        const bool is_ts_rising_slowly =
            (ts_voltage < threshold_voltage) &&
            (App_Timer_UpdateAndGetState(&globals->precharge_upper_bound_timer) == TIMER_STATE_EXPIRED);
        const bool is_ts_rising_quickly =
            (ts_voltage > threshold_voltage) &&
            (App_Timer_UpdateAndGetState(&globals->precharge_lower_bound_timer) == TIMER_STATE_RUNNING);

        const bool has_precharge_fault =
            checkPrechargeFaults(is_charger_connected, is_ts_rising_slowly, is_ts_rising_quickly, is_air_negative_open);

        // If there is a pre-charge fault and there were no more than three previous pre-charge faults
        // Go back to Init State, add one to the pre-charge failed counter and set the CAN charging message to false
        // Else go to Fault State, reset the pre-charge failed counter and set the CAN charging message to false
        if (has_precharge_fault)
        {
            if (globals->precharge_limit_exceeded)
            {
                App_SharedStateMachine_SetNextState(state_machine, app_faultState_get());
            }
            else
            {
                App_SharedStateMachine_SetNextState(state_machine, app_initState_get());
            }
        }
        // If there is no precharge fault and the charger is connected
        // Close the AIRs+, reset fault counter and go to Charge State
        // Else close the AIRs+, reset fault counter and go to Drive State
        else if (ts_voltage >= threshold_voltage)
        {
            const struct State* next_state;
            if (is_charger_connected)
            {
                next_state = app_chargeState_get();
            }
            else
            {
                next_state = app_driveState_get();
            }

            globals->num_precharge_failures = 0U;
            App_SharedStateMachine_SetNextState(state_machine, next_state);
            io_airs_closePositive();
        }

        if (is_air_negative_open && is_charger_connected)
        {
            // TODO: Consider reworking this transition.
            App_SharedStateMachine_SetNextState(state_machine, app_faultState_get());
            App_CanRx_Debug_StartCharging_Update(false);
            App_CanAlerts_BMS_Fault_ChargerExternalShutdown_Set(!is_charger_connected);
        }
    }
}

static void preChargeStateRunOnExit(struct StateMachine* const state_machine)
{
    // If we entered precharge to charge, disable this request on exit.
    App_CanRx_Debug_StartCharging_Update(false);

    // Open precharge relay.
    io_airs_openPrecharge();
}

const struct State* app_prechargeState_get(void)
{
    static struct State pre_charge_state = {
        .name              = "PRECHARGE",
        .run_on_entry      = preChargeStateRunOnEntry,
        .run_on_tick_1Hz   = preChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = preChargeStateRunOnTick100Hz,
        .run_on_exit       = preChargeStateRunOnExit,
    };

    return &pre_charge_state;
}