#ifndef TIMERS_H
#define TIMERS_H

#include "main.h"

/* Initilized externally by STMCube.*/
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/**
  * @brief  Initializes timers; using the appropriate HAL functions, params, and settings.
 */
void Timers_StartTimers();

#endif