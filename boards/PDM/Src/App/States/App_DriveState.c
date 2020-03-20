#include "App/States/App_DriveState.h"

static StateHandle_t drive_state_handle;

StateHandle_t *App_DriveState_GetStateHandlePointer(void)
{
    return &drive_state_handle;
}

StateHandle_t App_DriveState_GetStateHandle(void)
{
    return drive_state_handle;
}

void App_DriveState_EntryAction(void) {}

void App_DriveState_ExitAction(void) {}

void App_DriveState_StateAction(void) {}
