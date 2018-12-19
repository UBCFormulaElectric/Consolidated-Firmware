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
#define PRIMARY_APPS_MAX_VALUE   (float32_t)(1400.0f)
#define SECONDARY_APPS_MAX_VALUE (float32_t)(1950.0f)
#define APPS_PERCENT_DEADZONE    (float32_t)(0.03f)
#define PRIMARY_APPS_DEADZONE \
    (float32_t)PRIMARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE
#define SECONDARY_APPS_DEADZONE \
    (float32_t)SECONDARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE

// Accelerator pedal saturation point (%)
#define PEDAL_SATURATION_POINT (float32_t)(0.80f)
#define PEDAL_RELEASE_POINT    (float32_t)(0.04f)

// Fault handling variables
#define PEDAL_SATURATION_TIMEOUT  (uint32_t)(10)         // (sec)
#define APPS_IMPLAUSIBILITY_DELAY (float32_t)(1.0f)       // (sec)
#define APPS_BPPC_THRESHOLD       (float32_t)(0.25f)      // (decimal %)
#define MAX_APPS_FAULTS \
    (float32_t)APPS_IMPLAUSIBILITY_DELAY * (float32_t)CONTROL_LOOP_FREQUENCY
#define MAX_SATURATION_FAULTS \
    (uint32_t)PEDAL_SATURATION_TIMEOUT * (uint32_t)CONTROL_LOOP_FREQUENCY

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** Defines the modes in which APPS function could be run. */
typedef enum
{
    /* @brief Normal reading (Does not affect APPSFaultCounter) */
    APPS_NORMAL_MODE,
    /* @brief Control loop reading (Does affect APPSFaultCounter) */
    APPS_CONTROL_LOOP_MODE
} APPS_Mode_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern volatile uint32_t apps_fault_state;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Gets the latest accelerator pedal position
 *         1. Reads the latest APPS (encoder) position values from TIM2 and TIM3
 *         2. Checks for improperly connected APPS encoders
 *         3. Checks for out-of-bounds APPS readings (outside calibrated maximum
 *         values)
 *         4. Checks for implausible APPS readings (>10% difference between the
 *         primary and secondary readings as per EV2.3.6)
 *  	   5. Checks for APPS/Brake Pedal
 *         Plausibility Check, where accelerator pedal is pushed >25% while brake
 *         is pushed (EV2.5)
 *         6. Checks for accelerator pedal stuck at max. torque for
 *         longer than 10 secs
 *         7. Maps the APPS readings to a 10-bit number (0 =
 *         unpressed, 1023 = fully pressed)
 * @param  mode - Can be a value of APPS_Mode_Enum
 * @return Accelerator pedal position (10-bit)
 */
uint16_t getAcceleratorPedalPosition(APPS_Mode_Enum mode);
#endif /* APPS_H */
