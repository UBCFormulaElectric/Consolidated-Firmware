#include "StateMachine/App_StateMachine.h"

#include "StateMachine/App_InitState.h"
#include "StateMachine/App_DriveState.h"
#include "StateMachine/App_FaultState.h"

static StateMachineHandle_t state_machine;

void App_StateMachine_Init(void)
{
    state_machine = App_SharedStateMachine_Alloc();

    StateHandle_t *state_handle_ptr;

    state_handle_ptr  = App_InitState_GetStateHandlePointer();
    *state_handle_ptr = App_SharedStateMachine_AddState(
        state_machine, "INIT", App_InitState_EntryAction,
        App_InitState_ExitAction, App_InitState_StateAction);

    state_handle_ptr  = App_DriveState_GetStateHandlePointer();
    *state_handle_ptr = App_SharedStateMachine_AddState(
        state_machine, "DRIVE", App_DriveState_EntryAction,
        App_DriveState_ExitAction, App_DriveState_StateAction);

    state_handle_ptr  = App_FaultState_GetStateHandlePointer();
    *state_handle_ptr = App_SharedStateMachine_AddState(
        state_machine, "FAULT", App_FaultState_EntryAction,
        App_FaultState_ExitAction, App_FaultState_StateAction);

    App_SharedStateMachine_Init(
        "STATE MACHINE", state_machine, App_InitState_GetStateHandle());
}

void App_StateMachine_Tick(void)
{
    App_SharedStateMachine_Tick(state_machine);
}

StateHandle_t App_StateMachine_GetCurrentState(void)
{
    return App_SharedStateMachine_GetCurrentState(state_machine);
}
