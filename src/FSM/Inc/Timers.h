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
#define PAPPS_TIMER        htim2
#define SAPPS_TIMER        htim3
#define CONTROL_LOOP_TIMER htim14
#define FRWHEELSPEED_TIMER htim16
#define FLWHEELSPEED_TIMER htim17


/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern TIM_HandleTypeDef CONTROL_LOOP_TIMER;
extern TIM_HandleTypeDef PAPPS_TIMER;
extern TIM_HandleTypeDef SAPPS_TIMER;
extern TIM_HandleTypeDef FRWHEELSPEED_TIMER;
extern TIM_HandleTypeDef FLWHEELSPEED_TIMER;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Start timers for control loop, PAPPS, and SAPPS
 */
void Timers_StartTimers();

#endif /* TIMERS_H */
