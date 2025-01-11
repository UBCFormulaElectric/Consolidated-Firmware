#pragma once
#include "main.h"

void           HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_runCanTx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_run1Hz(void);
