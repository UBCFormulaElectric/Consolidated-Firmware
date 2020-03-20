#include "StateMachine/App_InitState.h"

static StateHandle_t init_state_handle;

StateHandle_t *App_InitState_GetStateHandlePointer(void)
{
    return &init_state_handle;
}

StateHandle_t App_InitState_GetStateHandle(void)
{
    return init_state_handle;
}

void App_InitState_EntryAction(void) {}

void App_InitState_ExitAction(void) {}

void App_InitState_StateAction(void) {}
