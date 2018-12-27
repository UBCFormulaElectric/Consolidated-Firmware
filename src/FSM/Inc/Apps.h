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

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Maximum APPS values (calibrated to pedal box)
#define PRIMARY_APPS_MAX_VALUE   (uint32_t)(1400)
#define SECONDARY_APPS_MAX_VALUE (uint32_t)(1950)
#define APPS_PERCENT_DEADZONE    (float32_t)(0.03f)
#define PRIMARY_APPS_DEADZONE \
    (uint32_t)(PRIMARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE)
#define SECONDARY_APPS_DEADZONE \
    (uint32_t)(SECONDARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE)

// Accelerator pedal saturation point (%)
#define PEDAL_SATURATION_POINT (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.80f)
#define PEDAL_TRAVEL_DEADZONE_THRESHOLD    (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.04f)

#define PEDAL_SATURATION_TIMEOUT    (uint32_t)(10)         // (sec)
#define APPS_IMPLAUSIBILITY_TIMEOUT (uint32_t)(1)          // (sec)
#define APPS_PLAUSIBILITY_THRESHOLD (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.25f)
#define APPS_PLAUSIBILITRY_RECOVERY_THRESHOLD \
    (uint32_t)(PRIMARY_APPS_MAX_VALUE * 0.05f)
#define APPS_DEVIATION_THRESHOLD    (float32_t)(0.1f) // (%)
#define MAX_APPS_FAULTS \
    (uint32_t)(APPS_IMPLAUSIBILITY_TIMEOUT * CONTROL_LOOP_FREQUENCY)
#define MAX_SATURATION_FAULTS \
    (uint32_t)(PEDAL_SATURATION_TIMEOUT * CONTROL_LOOP_FREQUENCY)
#define ACCELERATOR_PEDAL_BIT_RESOLUTION MAX_10_BITS_VALUE

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
 * @brief  Get the latest accelerator pedal position
 *         1. Read the PAPPS/SAPPS position values from TIM2 and TIM3
 *         2. Checks for improperly connected APPS encoders
 *         3. Checks if APPS readings are outside of calibrated maximum values
 *         4. Checks for implausible APPS readings (>10% difference between
 *            PAPPS and SAPPS as per EV2.3.6)
 *         5. Perform APPS/Brake Pedal Plausibility Check (Accelerator pedal is
 *            pushed >25% while brake is pushed as per EV2.5)
 *         6. Checks if accelerator pedal has been stuck at maximum torque for
 *            more than 10 seconds
 *         7. Maps the APPS readings to a 10-bit value (0 = unpressed, 1023 =
 *            fully pressed)
 * @return Accelerator pedal position mapped to a 10-bit vaue
 */
uint32_t Apps_GetAcceleratorPedalPosition(void);
#endif /* APPS_H */
