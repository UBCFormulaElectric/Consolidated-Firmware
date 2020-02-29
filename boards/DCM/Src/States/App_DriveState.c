#include "States/App_States.h"
#include "States/App_DriveState.h"

static State_t *drive_state;

State_t **App_DriveState_GetStateHandlePointer(void)
{
    return &drive_state;
}

State_t *App_DriveState_GetStateHandle(void)
{
    return drive_state;
}
void App_DriveState_EntryAction(void) {}

void App_DriveState_ExitAction(void) {}

void App_DriveState_StateAction(void)
{
    // TODO: Remove before merging
    // Here's an example of how we would transition to another state
    App_SharedState_SetNextState(drive_state, FAULT_STATE);
}
