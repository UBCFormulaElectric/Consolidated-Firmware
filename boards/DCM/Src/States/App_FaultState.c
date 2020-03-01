#include "States/App_FaultState.h"

static State_t *fault_state;

State_t **App_FaultState_GetStateHandlePointer(void)
{
    return &fault_state;
}

State_t *App_FaultState_GetStateHandle(void)
{
    return fault_state;
}
void App_FaultState_EntryAction(void) {}

void App_FaultState_ExitAction(void) {}

void App_FaultState_StateAction(void) {}
