#include "States/App_StateMachine.h"

#include "States/App_InitState.h"
#include "States/App_DriveState.h"
#include "States/App_FaultState.h"

StateMachineHandle_t state_machine;

void App_StateMachine_Init(void)
{
    state_machine = App_SharedStateMachine_Alloc();

    init_state_handle = App_SharedStateMachine_AddState(
        state_machine, "INIT", App_InitState_EntryAction,
        App_InitState_ExitAction, App_InitState_StateAction);

    drive_state_handle = App_SharedStateMachine_AddState(
        state_machine, "DRIVE", App_DriveState_EntryAction,
        App_DriveState_ExitAction, App_DriveState_StateAction);

    fault_state_handle = App_SharedStateMachine_AddState(
        state_machine, "FAULT", App_FaultState_EntryAction,
        App_FaultState_ExitAction, App_FaultState_StateAction);

    App_SharedStateMachine_Init(
        "STATE MACHINE", state_machine, init_state_handle);
}
