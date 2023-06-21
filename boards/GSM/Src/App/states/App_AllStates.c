#include "states/App_AllStates.h"
#include "App_SharedConstants.h"
#include "App_SharedMacros.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_CanTx_GSM_Vitals_Heartbeat_Set(true);
}

void App_AllStatesRunOnTick1kHz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}
