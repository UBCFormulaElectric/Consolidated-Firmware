/**
 * @file  SteeringAngle.h
 * @brief FSM_2017 - Function library for the steering angle sensor
 */

#ifndef STEERINGANGLE_H
#define STEERINGANGLE_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
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

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Determines steering angle in degrees based on steering angle potentiometer voltage.
 * @param  *FilteredADCReadings Filtered ADC readings by computing the mean of prior readings.
 */
void CalculateSteeringAngle(float *FilteredADCReadings);

/**
 * @brief  Determines steering angle in degrees based on steering angle potentiometer voltage.
 * @return SteeringFaultState
 */
uint8_t GetSteeringFaultState(void);


#endif /* STEERINGANGLE_H */
