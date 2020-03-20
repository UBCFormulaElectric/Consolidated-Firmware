#pragma once

#include "App_SharedState.h"

StateHandle_t *App_FaultState_GetStateHandlePointer(void);
StateHandle_t  App_FaultState_GetStateHandle(void);
void           App_FaultState_EntryAction(void);
void           App_FaultState_ExitAction(void);
void           App_FaultState_StateAction(void);
