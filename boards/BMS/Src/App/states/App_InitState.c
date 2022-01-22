#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_DriveState.h"
#include "App_SharedMacros.h"

#include "App_SetPeriodicCanSignals.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    const struct Accumulator *accumulator = App_BmsWorld_GetAccumulator(world);
    struct Clock *            clock       = App_BmsWorld_GetClock(world);

    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    // Send start ADC conversion commands to wake up the LTC6813 chip and
    // configure configuration register A
    App_Accumulator_StartAdcConversions(accumulator);
    App_Accumulator_ConfigureCellMonitors(accumulator);

    // Start ADC conversions
    App_Accumulator_StartAdcConversions(accumulator);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *   clock = App_BmsWorld_GetClock(world);

    // After entering the init state for 5 seconds, enter the AIR open state
    if (App_SharedClock_GetCurrentTimeInMilliseconds(clock) -
            App_SharedClock_GetPreviousTimeInMilliseconds(clock) >=
        5000U)
    {
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetAirOpenState());
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}
