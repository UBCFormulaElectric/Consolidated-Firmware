/**
 * @file  SteeringAngle.h
 * @brief Function library for the steering angle sensor
 */

#ifndef STEERINGANGLE_H
#define STEERINGANGLE_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
#include "ADC.h"
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
//Pointer to array containing the unfiltered SteeringAngleADC ADC readings
static volatile uint16_t SteeringAngleADCReadings[ADC_CHANNEL_COUNT * ADC_FILTER_SIZE];

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

/**
 * @brief  Determines steering angle in degrees based on filtered steering angle potentiometer
 *         voltage.
 * @return SteeringFaultState
 */
void GetSteeringAngle(void);

#endif /* STEERINGANGLE_H */
