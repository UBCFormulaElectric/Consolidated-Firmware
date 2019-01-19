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
#include <stdbool.h>

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
/** @brief Specifies Wheel Enum*/
typedef enum{
    F_R_WHEEL,
    F_L_WHEEL
} Wheel_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Gets the WheelSpeedFaultState variable
 * @return  WheelSpeedFaultState
 */
bool GetWheelSpeedSensorsFaulted(void);

/**
 * @brief  1. Calculates the front left or right wheel speed in km/h and stores it
 *         2. Stores the current input capture timer value for the next calculation
 * @param Wheel Specify which wheel speed to store
 * @param curr_timer_val Current wheel speed timer value from hall effect timer.
 */
void SetWheelSpeed(Wheel_Enum Wheel, uint32_t curr_timer_val);

/**
 * @brief  Returns the calculated Left Wheel Speed value.
 */
float GetLeftWheelSpeed(void);

/**
 * @brief  Returns the calculated Left Wheel Speed value.
 */
float GetRightWheelSpeed(void);

/**
 * @brief Transmits Wheel speed over CAN.
 */
void WS_HandleWheelSpeed(void);

/**
 * @brief Increments counter whenever control loop runs i.e gets the wheel speed
 *           Reset whenever wheel speed is set, if we exceed threshold set 
 *           set wheel speed to 0. (No IC call back we are startionary)
 */
void WheelSpeedIncrementControlLoopCounter(void);



#endif /* WHEELSPEED_H */

