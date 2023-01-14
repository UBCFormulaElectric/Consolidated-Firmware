#include "states/App_AllStates.h"
#include "states/App_DefaultState.h"
#include "App_SharedMacros.h"

static void DefaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct GsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    // JSONCAN -> App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_GSM_STATE_MACHINE_STATE_DEFAULT_CHOICE);
}

static void DefaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DefaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
}

static void DefaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDefaultState(void)
{
    static struct State air_open_state = {
        .name              = "AIR OPEN",
        .run_on_entry      = DefaultStateRunOnEntry,
        .run_on_tick_1Hz   = DefaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = DefaultStateRunOnTick100Hz,
        .run_on_exit       = DefaultStateRunOnExit,
    };

    return &air_open_state;
}
