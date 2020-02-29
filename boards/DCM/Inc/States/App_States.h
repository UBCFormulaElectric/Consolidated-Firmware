#include "App_SharedState.h"

extern State_t *App_InitState_GetStateHandle(void);
#define INIT_STATE App_InitState_GetStateHandle()

extern State_t *App_DriveState_GetStateHandle(void);
#define DRIVE_STATE App_DriveState_GetStateHandle()

extern State_t *App_FaultState_GetStateHandle(void);
#define FAULT_STATE App_FaultState_GetStateHandle()
