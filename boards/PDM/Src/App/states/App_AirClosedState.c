#include "states/App_AllStates.h"
#include "states/App_AirClosedState.h"
#include "states/App_AirOpenState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

static void AirClosedStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void AirClosedStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void AirClosedStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    const bool air_positive_open = App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_OPEN;
    const bool air_negative_open = App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_OPEN;
    if (air_positive_open || air_negative_open)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetAirOpenState());
    }
}

static void AirClosedStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirClosedState(void)
{
    static struct State air_closed_state = {
        .name              = "AIR CLOSED",
        .run_on_entry      = AirClosedStateRunOnEntry,
        .run_on_tick_1Hz   = AirClosedStateRunOnTick1Hz,
        .run_on_tick_100Hz = AirClosedStateRunOnTick100Hz,
        .run_on_exit       = AirClosedStateRunOnExit,
    };

    return &air_closed_state;
}
