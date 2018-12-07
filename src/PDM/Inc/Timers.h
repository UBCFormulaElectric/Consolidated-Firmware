/**
 * @file  Timers.h
 * @brief Timer library
 */

#ifndef TIMERS_H
#define TIMERS_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "FaultHandling.h"
#include "stm32f3xx_hal.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
* Global Variables
*******************************************************************************/
// TODO  (Issue #191): Should include header for TIM_HandleTypeDef externs
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim17;

extern volatile GPIO_PinState dsel_state;
extern volatile uint8_t e_fuse_fault_states[ADC_CHANNEL_COUNT * NUM_READINGS];

/******************************************************************************
* Function Prototypes
*******************************************************************************/
// TODO  (Issue #191): This should moved into MX_TIMERS_INIT() probably
/**
 * @brief Initialize Timers 
 */
void Timers_Init(void);

#endif
