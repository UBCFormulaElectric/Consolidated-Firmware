#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"

#include "App_BmsWorld.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

#define NUM_OF_INVERTERS (2U)
#define PRECHARGE_RESISTANCE_OHMS (1000U)
#define INV_CAPACITANCE_F (0.280e-3f)
#define TOTAL_PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_S \
    (PRECHARGE_RESISTANCE_OHMS * TOTAL_PRECHARGE_CAPACITANCE_F)
#define PRECHARGE_COMPLETION_MS (S_TO_MS(4U * PRECHARGE_RC_S))

#include "Io_Charger.h"
#include "Io_PreCharge.h"
#include "Io_Airs.h"
#include "Io_Adc.h"
#include "Io_VoltageSense.h"
#include "main.h"
static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);

    // Enable precharge relay
    Io_PreCharge_Enable();

    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void
    PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *   world = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *      clock = App_BmsWorld_GetClock(world);
    struct Airs *       airs  = App_BmsWorld_GetAirs(world);
    struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);

    // lower and upper bounds on expected charge time. Used to recognize faults
    const uint32_t lower_precharge_time =
        (uint32_t)((float)PRECHARGE_COMPLETION_MS * 0.10f);
    const uint32_t upper_precharge_time =
        (uint32_t)((float)PRECHARGE_COMPLETION_MS * 2.0f);

    // percentage threshold, which when TS reaches this threshold of ACC, AIR+
    // engages and precharge is disabled must be at least 90% EV.6.6.1
    const float precharge_threshold_percentage = 0.78f;

    // Voltage information
    float tractive_system_voltage = Io_VoltageSense_GetTractiveSystemVoltage(
        Io_Adc_GetAdc1Channel3Voltage());
    float accumulator_voltage = App_Accumulator_GetPackVoltage(accumulator);
    float threshold_voltage =
        accumulator_voltage * precharge_threshold_percentage;

    // track the time since precharge started
    uint32_t elapsed_time =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
        App_SharedClock_GetPreviousTimeInMilliseconds(clock);

    // if the voltage increases too slowly or too quickly, there may be an issue
    if ((tractive_system_voltage < threshold_voltage &&
         elapsed_time >= upper_precharge_time) ||
        (tractive_system_voltage > threshold_voltage &&
         elapsed_time <= lower_precharge_time))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    // Successful precharge if tractive system reaches threshold between the
    // lower and upper time bounds (expected range precharge time)
    else if (tractive_system_voltage >= threshold_voltage)
    {
        // close AIR+
        App_Airs_CloseAirPositive(airs);

        if (Io_Charger_IsConnected())
        {
            // enter charge state if charger is connected
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetChargeState());
        }
        else
        {
            // if charger disconnected, enter drive state
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
