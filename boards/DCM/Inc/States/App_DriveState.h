#pragma once

typedef struct State State_t;

State_t **App_DriveState_GetStateHandlePointer(void);
State_t * App_DriveState_GetStateHandle(void);
void      App_DriveState_EntryAction(void);
void      App_DriveState_ExitAction(void);
void      App_DriveState_StateAction(void);
