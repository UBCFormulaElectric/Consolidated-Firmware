#include "States/App_StateMachine.h"

#include "States/App_InitState.h"
#include "States/App_DriveState.h"
#include "States/App_FaultState.h"

StateMachine_t *state_machine;

void App_StateMachine_Init(void)
{
    state_machine = App_SharedStateMachine_Alloc("STATE MACHINE");

    State_t **state;

    state  = App_InitState_GetStateHandlePointer();
    *state = App_SharedStateMachine_AddState(
        state_machine, "INIT", App_InitState_EntryAction,
        App_InitState_ExitAction, App_InitState_StateAction);

    state  = App_DriveState_GetStateHandlePointer();
    *state = App_SharedStateMachine_AddState(
        state_machine, "DRIVE", App_DriveState_EntryAction,
        App_DriveState_ExitAction, App_DriveState_StateAction);

    state  = App_FaultState_GetStateHandlePointer();
    *state = App_SharedStateMachine_AddState(
        state_machine, "FAULT", App_FaultState_EntryAction,
        App_FaultState_ExitAction, App_FaultState_StateAction);

    App_SharedStateMachine_Init(state_machine, App_InitState_GetStateHandle());
}
