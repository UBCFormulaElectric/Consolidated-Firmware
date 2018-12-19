/**
 * @file  Timer.h
 * @brief Library for timer start helper function and timer callbacks
 */

#ifndef TIMERS_H
#define TIMERS_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f0xx_hal.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** @brief Enum for motor shutdown/re-enable handling status */
typedef enum
{
    ON,
    OFF
} Motor_Shutdown_Status_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Start timers for Control Loop, P-APPS, and S-APPS
 */
void Timers_StartTimers();

#endif /* TIMERS_H */
