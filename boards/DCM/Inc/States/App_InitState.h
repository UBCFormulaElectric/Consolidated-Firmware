#pragma once

typedef struct State State_t;

State_t **App_InitState_GetStateHandlePointer(void);
State_t * App_InitState_GetStateHandle(void);
void      App_InitState_EntryAction(void);
void      App_InitState_ExitAction(void);
void      App_InitState_StateAction(void);
