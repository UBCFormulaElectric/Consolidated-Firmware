#include "states/App_AllStates.h"
#include "App_SharedConstants.h"
#include "App_SharedMacros.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct GsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    // JSONCAN -> App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);
}
