#pragma once

typedef struct State State_t;

State_t **App_FaultState_GetStateHandlePointer(void);
State_t * App_FaultState_GetStateHandle(void);
void      App_FaultState_EntryAction(void);
void      App_FaultState_ExitAction(void);
void      App_FaultState_StateAction(void);
