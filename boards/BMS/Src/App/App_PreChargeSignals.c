#include "App_PreChargeSignals.h"
#include "App_PreChargeStateMachine.h"

bool App_PrechargeSignals_IsWaitingAfterInit(struct BmsWorld *world)
{
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct PreChargeStateMachine *state_machine =
        App_PreChargeSequence_GetStateMachine(pre_charge_sequence);

    return App_PreChargeStateMachine_GetCurrentState(state_machine) ==
           App_PreChargeState_GetInitState();
}

void App_PrechargeSignals_WaitingAfterInitCompleteCallback(
    struct BmsWorld *world)
{
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);

    App_PreChargeStateMachine_SetNextState(
        pre_charge_sequence, App_PreChargeState_GetAirOpenState());
}
