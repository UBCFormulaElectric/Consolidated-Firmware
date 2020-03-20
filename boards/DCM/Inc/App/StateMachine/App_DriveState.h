#pragma once

#include "StateMachine/App_SharedState.h"

StateHandle_t *App_DriveState_GetStateHandlePointer(void);
StateHandle_t  App_DriveState_GetStateHandle(void);
void           App_DriveState_EntryAction(void);
void           App_DriveState_ExitAction(void);
void           App_DriveState_StateAction(void);
