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
/** @brief Maximum PAPPS value that should not be exceeded or else the pedal
 *         box may physically break */
#define PRIMARY_APPS_MAX_VALUE   (uint32_t)(1400)

/** @brief Maximum SAPPS value that should not be exceeded or else the pedal
 *         box may physically break */
#define SECONDARY_APPS_MAX_VALUE (uint32_t)(1950)

/** @brief The driver should not have to push the pedal to its maximum travel
 *         to request maximum torque or else the pedal box will wear out more
 *         easily */
#define PAPPS_SATURATION_THRESHOLD \
            (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.80f)

/** @brief The initial pedal travel is mapped to zero torque request for better
 *         driving experience */
#define PAPPS_DEADZONE_THRESHOLD \
            (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.04f)


/** @brief The APPS threshold specified in 2017-18 EV2.5 */
#define APPS_PLAUSIBILITY_THRESHOLD \
    (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.25f)

/** @brief The APPS threshold specified in 2017-18 EV2.5.1 */
#define APPS_PLAUSIBILITRY_RECOVERY_THRESHOLD \
    (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.05f)

/** @brief The maximum percentage difference in pedal travel between PAPPS and SAPPS 
 *         allowed according to 2017-18 EV.2.3.6 */
#define APPS_DEVIATION_THRESHOLD    (float32_t)(0.1f) // (10%)

#define PEDAL_SATURATION_TIMEOUT    (uint32_t)(10)         // (sec)
#define APPS_IMPLAUSIBILITY_TIMEOUT (uint32_t)(1)          // (sec)

#define MAX_APPS_FAULTS \
    (uint32_t)(APPS_IMPLAUSIBILITY_TIMEOUT * CONTROL_LOOP_FREQUENCY)

#define MAX_SATURATION_FAULTS \
    (uint32_t)(PEDAL_SATURATION_TIMEOUT * CONTROL_LOOP_FREQUENCY)

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
extern volatile uint32_t apps_fault_state;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Determine the appropriate brake status and accelerator pedal position
 *         and transmit the information to DCM over CAN
 * 
 *         2. Checks for improperly connected APPS encoders
 *         4. Checks for implausible APPS readings (>10% difference between
 *            PAPPS and SAPPS as per EV2.3.6)
 *         5. Perform APPS/Brake Pedal Plausibility Check (Accelerator pedal is
 *            pushed >25% while brake is pushed as per EV2.5)
 *         6. Checks if accelerator pedal has been stuck at maximum torque for
 *            more than 10 seconds
 */
void void Apps_HandleBrakeAndAcceleratorPedals(void);
#endif /* APPS_H */
