#pragma once

#include "App_SharedState.h"

extern StateHandle_t init_state_handle;

void App_InitState_EntryAction(void);
void App_InitState_ExitAction(void);
void App_InitState_StateAction(void);
