#include "states/App_PreChargeState.h"

// clang-format off
#define NUM_OF_INVERTERS        (2U)
#define PRECHARGE_RESISTOR_OHMS (500U)
#define INV_CAPACITANCE_F       (0.280e-3f)
#define PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_MS         (S_TO_MS(PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F))

// When TS reaches this threshold of ACC, AIR+ engages, and precharge is disabled, the voltage must be at least 90% EV.6.6.1
#define PRECHARGE_ACC_V_THRESHOLD (0.90f)

// 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_MS          ((float)PRECHARGE_RC_MS * 2.7f)
#define PRECHARGE_COMPLETION_LOWER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f))
#define PRECHARGE_COMPLETION_UPPER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 2.0f))

// clang-format on

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *      world           = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *         clock           = App_BmsWorld_GetClock(world);
    struct PrechargeRelay *precharge_relay = App_BmsWorld_GetPrechargeRelay(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_PRECHARGE_STATE);

    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    App_PrechargeRelay_Close(precharge_relay);
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *      world           = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *         clock           = App_BmsWorld_GetClock(world);
        struct Airs *          airs            = App_BmsWorld_GetAirs(world);
        struct Accumulator *   accumulator     = App_BmsWorld_GetAccumulator(world);
        struct Charger *       charger         = App_BmsWorld_GetCharger(world);
        struct TractiveSystem *ts              = App_BmsWorld_GetTractiveSystem(world);
        struct PrechargeRelay *precharge_relay = App_BmsWorld_GetPrechargeRelay(world);

        bool  precharge_fault_limit_exceeded = false;
        float ts_voltage                     = App_TractiveSystem_GetVoltage(ts);
        float threshold_voltage = App_Accumulator_GetAccumulatorVoltage(accumulator) * PRECHARGE_ACC_V_THRESHOLD;

        uint32_t elapsed_time =
            App_SharedClock_GetCurrentTimeInMilliseconds(clock) - App_SharedClock_GetPreviousTimeInMilliseconds(clock);

        const bool is_ts_rising_slowly =
            (ts_voltage < threshold_voltage) && (elapsed_time >= PRECHARGE_COMPLETION_UPPER_BOUND);
        const bool is_ts_rising_quickly =
            (ts_voltage > threshold_voltage) && (elapsed_time <= PRECHARGE_COMPLETION_LOWER_BOUND);
        const bool is_charger_connected = App_Charger_IsConnected(charger);
        const bool is_air_negative_open = !App_Airs_IsAirNegativeClosed(airs);
        const bool has_precharge_fault  = App_PrechargeRelay_CheckFaults(
            precharge_relay, is_charger_connected, is_ts_rising_slowly, is_ts_rising_quickly, is_air_negative_open,
            &precharge_fault_limit_exceeded);

        // If there is a pre-charge fault and there were no more than three previous pre-charge faults
        // Go back to Init State, add one to the pre-charge failed counter and set the CAN charging message to false
        // Else go to Fault State, reset the pre-charge failed counter and set the CAN charging message to false
        if (has_precharge_fault)
        {
            const struct State *next_state;
            if (precharge_fault_limit_exceeded)
            {
                next_state = App_GetFaultState();
                App_PrechargeRelay_ResetFaultCounterVal(precharge_relay);
            }
            else
            {
                next_state = App_GetInitState();
            }
            App_CanRx_DEBUG_ChargingSwitch_StartCharging_Update(false);

            App_SharedStateMachine_SetNextState(state_machine, next_state);
        }
        // If there is no precharge fault and the charger is connected
        // Close the AIRs+, reset fault counter and go to Charge State
        // Else close the AIRs+, reset fault counter and go to Drive State
        else if (ts_voltage >= threshold_voltage)
        {
            const struct State *next_state = (is_charger_connected) ? App_GetChargeState() : App_GetDriveState();
            App_Airs_CloseAirPositive(airs);
            App_PrechargeRelay_ResetFaultCounterVal(precharge_relay);
            App_SharedStateMachine_SetNextState(state_machine, next_state);
        }
    }
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *      world           = App_SharedStateMachine_GetWorld(state_machine);
    struct PrechargeRelay *precharge_relay = App_BmsWorld_GetPrechargeRelay(world);

    App_PrechargeRelay_Open(precharge_relay);
}

const struct State *App_GetPreChargeState(void)
{
    static struct State pre_charge_state = {
        .name              = "PRE_CHARGE",
        .run_on_entry      = PreChargeStateRunOnEntry,
        .run_on_tick_1Hz   = PreChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = PreChargeStateRunOnTick100Hz,
        .run_on_exit       = PreChargeStateRunOnExit,
    };

    return &pre_charge_state;
}
