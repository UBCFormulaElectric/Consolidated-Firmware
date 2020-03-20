#include "App/StateMachine/App_FaultState.h"

static StateHandle_t fault_state_handle;

StateHandle_t *App_FaultState_GetStateHandlePointer(void)
{
    return &fault_state_handle;
}

StateHandle_t App_FaultState_GetStateHandle(void)
{
    return fault_state_handle;
}

void App_FaultState_EntryAction(void) {}

void App_FaultState_ExitAction(void) {}

void App_FaultState_StateAction(void) {}
