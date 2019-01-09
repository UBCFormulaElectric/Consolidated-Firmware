/**
 * @file  Timers.h
 * @brief Timer library
 */

#ifndef TIMERS_H
#define TIMERS_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define ADC_DMA_TIMER      htim2
#define CONTROL_LOOP_TIMER htim17

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/
// TODO  (Issue #191): Should include header for TIM_HandleTypeDef externs
extern TIM_HandleTypeDef ADC_DMA_TIMER;
extern TIM_HandleTypeDef CONTROL_LOOP_TIMER;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
// TODO  (Issue #191): This should moved into MX_TIMERS_INIT() probably
/**
 * @brief Initialize Timers
 */
void Timers_Init(void);

#endif
