#pragma once

#include "App_SharedState.h"

extern StateHandle_t drive_state_handle;

void App_DriveState_EntryAction(void);
void App_DriveState_ExitAction(void);
void App_DriveState_StateAction(void);
