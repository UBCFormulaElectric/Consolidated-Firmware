/**
 *   @file       Timers.h
 *   @brief      PDM 2018 Code
 *   @author     UBC Formula Electric
**/

#ifndef TIMERS_H
#define TIMERS_H

#include "FaultHandling.h"
#include "stm32f3xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim17;
extern __IO GPIO_PinState DSEL_State;
extern __IO uint8_t e_fuse_fault_states[ADC_CHANNEL_COUNT * NUM_READINGS];

// Function Declarations
/**
 * 	@brief Initialize Timers
 *	@param None
 *	@return None
**/
void Timers_Init(void);

#endif
