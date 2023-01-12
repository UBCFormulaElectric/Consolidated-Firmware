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

    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    // TODO: JSONCAN
    // if (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE ||
    //     App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_OPEN_CHOICE)
    if (false)
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
