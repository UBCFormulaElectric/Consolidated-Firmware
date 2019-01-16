/**
 * @file  WheelSpeed.h
 * @brief FSM_2018 - Function library header for the Hall-effect wheel speed sensors
 */

#ifndef WHEELSPEED_H
#define WHEELSPEED_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "main.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Set timer clock prescaler to 192 to ensure stable wheel speed readings at high speeds (20-100km/hr) 
// and capture lowest speed of 0.48km/hr (48MHz / 192 = 250kHz; 1/250kHz = 0.000004sec)
// 2^16 = Number of ticks in TIM16/17
// 2^16 * 0.000004sec = 0.262144sec = The longest time TIM16/17 can detect between two input captures
// Slowest speed we can detect = Distance Between Two Adjacent Teeth / Longest Detectable Time =
// (pi * 0.52m / 48 teeth) / 0.262144sec = 0.129763m/s = 0.467148km/hr 
#define PRESCALED_WHEEL_SPEED_TIMER_FREQUENCY   SystemCoreClock / WHEEL_SPEED_TIMER_PRESCALER

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** @brief Specifies Wheel Enum*/
typedef enum{
    F_R_WHEEL,
    FL_WHEEL
} Wheel_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Determines front wheel speed in km/h based on the period between two wheel speed sensor pulses
 * @param CurrentTimerValue    Current input capture value of TIM16/17
 * @param PreviousTimerValue    Previous input capture value of TIM16/17
 * @return  Wheel speed in km/h
 */
float CalculateWheelSpeed(uint16_t CurrentTimerValue, uint16_t PreviousTimerValue);

/**
 * @brief Gets the WheelSpeedFaultState variable
 * @return  WheelSpeedFaultState
 */
uint8_t GetWheelSpeedFaultState(void);


/**
 * @brief  1. Calculates the front left or right wheel speed in km/h and stores it
 *             2. Stores the current input capture timer value for the next calculation
 * @param Wheel Specify which wheel speed to store
 */
void SetWheelSpeed(Wheel_Enum Wheel);

#endif /* WHEELSPEED_H */

