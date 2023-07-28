#include "states/App_AllStates.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

static void AirOpenStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

static void AirOpenStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void AirOpenStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    const bool air_positive_closed = App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_CLOSED;
    const bool air_negative_closed = App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_CLOSED;

    struct PdmWorld *        world      = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor *hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);
    const bool               hb_ok      = App_SharedHeartbeatMonitor_Tick(hb_monitor);

    if (air_positive_closed && air_negative_closed && hb_ok)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetAirClosedState());
    }
}

static void AirOpenStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirOpenState(void)
{
    static struct State air_open_state = {
        .name              = "AIR OPEN",
        .run_on_entry      = AirOpenStateRunOnEntry,
        .run_on_tick_1Hz   = AirOpenStateRunOnTick1Hz,
        .run_on_tick_100Hz = AirOpenStateRunOnTick100Hz,
        .run_on_exit       = AirOpenStateRunOnExit,
    };

    return &air_open_state;
}
