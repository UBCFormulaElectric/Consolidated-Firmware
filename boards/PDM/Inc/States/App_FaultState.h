#pragma once

#include "App_SharedState.h"

extern StateHandle_t fault_state_handle;

void App_FaultState_EntryAction(void);
void App_FaultState_ExitAction(void);
void App_FaultState_StateAction(void);
