#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"

#include "App_BmsWorld.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "App_TractiveSystem.h"

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct PreChargeSequence *preChargeSequence =
        App_BmsWorld_GetPreChargeSequence(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);

    // Enable precharge relay
    App_PreChargeSequence_Enable(preChargeSequence);

    // set the start time for precharge
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
    struct TractiveSystem *tractive_system =
        App_BmsWorld_GetTractiveSystem(world);
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    // lower and upper bounds on expected charge time. Used to recognize faults
    const uint32_t lower_precharge_time =
        App_PreChargeSequence_GetLowerTimeBound_MS(
            pre_charge_sequence,
        App_TractiveSystem_GetSystemCapacitance());
    const uint32_t upper_precharge_time =
        App_PreChargeSequence_GetUpperTimeBound_MS(
            pre_charge_sequence,
            App_TractiveSystem_GetSystemCapacitance());

    // Voltage information
    float ts_voltage = App_TractiveSystem_GetSystemVoltage(tractive_system);
    float threshold_voltage =
        App_Accumulator_GetPackVoltage(accumulator) *
        App_PreChargeSequence_GetThreshold_Percent(pre_charge_sequence);

    // Time since precharge started
    uint32_t elapsed_time =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
        App_SharedClock_GetPreviousTimeInMilliseconds(clock);

    // CHARGE MODE
    if (App_Charger_IsConnected(charger))
    {
        // if the charger is connected, we want to ignore the case where
        // pre-charge happens too quickly we expect an OC in this case
        if ((ts_voltage < threshold_voltage &&
             elapsed_time >= upper_precharge_time))
        {
            // TODO: send non-crititcal fault message
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetFaultState());
        }
        else if (ts_voltage >= threshold_voltage)
        {
            // if precharge happens within expected range enter
            // charge state
            App_Airs_CloseAirPositive(airs);
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetChargeState());
        }
    }

    // DRIVE MODE
    else
    {
        // if the charger is disconnected, we want to watch for both cases of
        // pre-charging too quickly or too slowly

        if ((ts_voltage < threshold_voltage &&
             elapsed_time >= upper_precharge_time) ||
            (ts_voltage > threshold_voltage &&
             elapsed_time <= lower_precharge_time))
        {
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetFaultState());
            // TODO: Send non-critical fault message
        }
        // if precharge happens within expected range, close air+ and enter
        // drive state
        else if (ts_voltage >= threshold_voltage)
        {
            App_Airs_CloseAirPositive(airs);
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetDriveState());
        }
    }

    // if the AIR- opens again, send back to Init_State
    if (!App_Airs_IsAirNegativeClosed(airs))
    {
        // TODO: send non-critical fault msg
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct PreChargeSequence *preChargeSequence =
        App_BmsWorld_GetPreChargeSequence(world);

    App_PreChargeSequence_Disable(preChargeSequence);
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
