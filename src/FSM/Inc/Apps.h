/**
 * @file  Apps.h
 * @brief Function library for the Accelerator Pedal Position Sensors (APPS)
 */

#ifndef APPS_H
#define APPS_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "main.h"
#include "arm_math.h"
#include "Constants.h"
#include "Timers.h"

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
extern TIM_HandleTypeDef PAPPS_TIMER;
extern TIM_HandleTypeDef SAPPS_TIMER;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Determine and transmit the appropriate and accelerator pedal position
 *        to DCM over the CAN bus
 */
void Apps_HandleAcceleratorPedalPosition(void);

#endif /* APPS_H */
