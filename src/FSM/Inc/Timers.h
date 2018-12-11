#ifndef TIMERS_H
#define TIMERS_H

#include "main.h"

/* Initilized externally by STMCube.*/
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/* Initializes timers; using the appropriate HAL functions, and params*/
void Timers_StartTimers();

#endif