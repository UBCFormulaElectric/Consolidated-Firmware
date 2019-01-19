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
//Primary and secondary timers started in encoder modes to caputure encoder
// ticks.
#define PAPPS_TIMER        htim2
#define SAPPS_TIMER        htim3
//The control loop timer triggers callback at the set Hz, specifed in tim14
// settings.
#define CONTROL_LOOP_TIMER htim14
//Timer for Front Right wheel speed works as an Input Capture to capture Hall
// effect sensor ticks.
#define F_R_WHEELSPEED_TIMER htim16
//Timer for Front Left Wheel speed.
#define F_L_WHEELSPEED_TIMER htim17


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
// Thesse TIM_HandleTypeDef are declared with the extern keyboard because they
// are intialized in `main.c`, but we need to reference them in `Timers.c` and
// `Apps.c`
extern TIM_HandleTypeDef CONTROL_LOOP_TIMER;
extern TIM_HandleTypeDef PAPPS_TIMER;
extern TIM_HandleTypeDef SAPPS_TIMER;
extern TIM_HandleTypeDef F_R_WHEELSPEED_TIMER;
extern TIM_HandleTypeDef F_L_WHEELSPEED_TIMER;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Start timers for control loop, PAPPS, SAPPS, Front Right and Left 
	*        Wheel sensors.
 */
void Timers_StartTimers();

#endif /* TIMERS_H */
