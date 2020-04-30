#include "states/App_AllStates.h"
#include "states/App_AirClosedState.h"

#include "App_SharedMacros.h"
#include "App_SetPeriodicCanSignals.h"

static void AirClosedStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx_interface = App_PdmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_PDM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE);
}

static void AirClosedStateRunOnTick(struct StateMachine *const state_machine)
{
    AllStatesRunOnTick(state_machine);
}

static void AirClosedStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirClosedState(void)
{
    static struct State air_closed_state = {
        .name         = "AIR CLOSED",
        .run_on_entry = AirClosedStateRunOnEntry,
        .run_on_tick  = AirClosedStateRunOnTick,
        .run_on_exit  = AirClosedStateRunOnExit,
    };

    return &air_closed_state;
}
