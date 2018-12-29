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

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Maximum PAPPS value that should not be exceeded or else the pedal box may
// physically break. This should be manually calibrated every once in a while.
#define PRIMARY_APPS_MAX_VALUE   (uint32_t)(1400)

// Maximum SAPPS value that should not be exceeded or else the pedal box may
// physically break. This should be manually calibrated every once in a while.
#define SECONDARY_APPS_MAX_VALUE (uint32_t)(1950)

// The driver should not have to push the pedal to its maximum travel
// to request maximum torque or else the pedal box will wear out more
// easily
#define PAPPS_SATURATION_THRESHOLD (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.80f)

// The initial pedal travel is treated as a "deadzone" - as if the pedal was
// not pressed at all
#define PAPPS_DEADZONE_THRESHOLD (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.04f)

// If the encoder reading is larger than this threshold, then the pedal has
// likely deflected beyond its original position
#define APPS_UNDERFLOW_THRESHOLD (uint32_t)(ENCODER_MAX_COUNT * 0.95)

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
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Determine and transmit the appropriate and accelerator pedal position
 *        to DCM over the CAN bus
 */
void Apps_HandleAcceleratorPedalPosition(void);

#endif /* APPS_H */
