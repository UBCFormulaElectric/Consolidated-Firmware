#pragma once

#include "StateMachine/App_SharedState.h"

StateHandle_t *App_InitState_GetStateHandlePointer(void);
StateHandle_t  App_InitState_GetStateHandle(void);
void           App_InitState_EntryAction(void);
void           App_InitState_ExitAction(void);
void           App_InitState_StateAction(void);
