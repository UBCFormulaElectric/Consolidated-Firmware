#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"

#include "Io_VoltageSense.h"
#include "Io_PreCharge.h"
#include "Io_Charger.h"
#include "Io_Airs.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

// expected charge time (to 98%) = 4RC * 1000ms/s
#define PRECHARGE_RESISTANCE (1000U)
#define PRECHARGE_CAPACITANCE (0.68e-3f)
#define PRECHARGE_EXPECTED_CHARGE_TIME_MS \
    ((uint32_t)(PRECHARGE_RESISTANCE * PRECHARGE_CAPACITANCE * S_TO_MS * 4U))

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);

    // capture start time of precharge
    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    // enable PreCharge relay
    Io_PreCharge_Enable();
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void
    PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *   clock = App_BmsWorld_GetClock(world);
    struct Airs *    airs  = App_BmsWorld_GetAirs(world);

    // lower and upper bounds on expected charge time. Used to recognize faults
    const float lower_precharge_time = PRECHARGE_EXPECTED_CHARGE_TIME_MS * 0.10;
    const float upper_precharge_time = PRECHARGE_EXPECTED_CHARGE_TIME_MS * 1.50;

    // percentage threshold, which when TS reaches this threshold of ACC, AIR+
    // engages and precharge is disabled must be at least 90% EV.6.6.1
    const float precharge_threshold_percentage = 0.80f;

    // Voltage information
    float tractive_system_voltage = Io_VoltageSense_GetTractiveSystemVoltage();
    float accumulator_voltage = 60.0f; // TODO: Replace value with function to
    float threshold_voltage =
        accumulator_voltage * precharge_threshold_percentage;

    // track the time since precharge started
    uint32_t elapsed_time =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
        App_SharedClock_GetPreviousTimeInMilliseconds(clock);

    // if the voltage increases too slowly
    //      or too quickly, there may be an issue
    if ((tractive_system_voltage < threshold_voltage &&
         (float)elapsed_time >= upper_precharge_time) ||
        (tractive_system_voltage > threshold_voltage &&
         (float)elapsed_time <= lower_precharge_time))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    // Successful precharge if tractive system reaches threshold between the
    // lower and upper time bounds (expected range precharge time)
    else if (tractive_system_voltage >= threshold_voltage)
    {
        // close AIR+
        App_Airs_CloseAirPositive(airs);

        // enter charge state if charger is connected
        if (Io_Charger_IsConnected())
        {
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetChargeState());
        }

        // if charger disconnected, enter drive state
        else
        {
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetDriveState());
        }
    }

    // if the AIR- opens again, send back to Init_State
    if (!Io_Airs_IsAirNegativeClosed())
    {
        // TODO: throw non-critical fault msg
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);

    // disengage PreCharge relay
    Io_PreCharge_Disable();
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
