#pragma once
#include "main.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void tasks_preInit();
void tasks_init();
void tasks_run100Hz(void);
void tasks_runCanTx(void);
void tasks_runCanRx(void);
void tasks_run1kHz(void);
void tasks_run1Hz(void);
