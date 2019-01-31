/**
 * @file  ErrorHandling.h
 * @brief Error Handling Library
 */

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CanDefinitions.h"
#include "SharedMacros.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// TODO: Convert this numer of retries into sensible values, the current values
//       are just some placeholder

#define CONTROL_LOOP_PERIOD \
    (uint32_t)(SECOND_TO_MILLSECOND(1) / CONTROL_LOOP_FREQUENCY)

// Overcurrent condition must persist for this amount of time (in milliseconds)
// before we cut off power to the GLV output
#define AUX1_OVERCURRENT_TIMEOUT_MS          (uint32_t)(500)
#define AUX2_OVERCURRENT_TIMEOUT_MS          (uint32_t)(500)
#define COOLING_OVERCURRENT_TIMEOUT_MS       (uint32_t)(500)
#define PDMFAN_OVERCURRENT_TIMEOUT_MS        (uint32_t)(500)
#define AIRSHDN_OVERCURRENT_TIMEOUT_MS       (uint32_t)(500)
#define CANGLV_OVERCURRENT_TIMEOUT_MS        (uint32_t)(500)
#define ACCSEGFAN_OVERCURRENT_TIMEOUT_MS     (uint32_t)(500)
#define ACCENCFAN_OVERCURRENT_TIMEOUT_MS     (uint32_t)(500)
#define LEFTINVERTER_OVERCURRENT_TIMEOUT_MS  (uint32_t)(500)
#define RIGHTINVERTER_OVERCURRENT_TIMEOUT_MS (uint32_t)(500)

#define AUX1_MAX_NUM_OF_RETRIES \
        (uint32_t)(AUX1_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define AUX2_MAX_NUM_OF_RETRIES \
        (uint32_t)(AUX2_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define COOLING_MAX_NUM_OF_RETRIES \
        (uint32_t)(COOLING_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define PDMFAN_MAX_NUM_OF_RETRIES \
        (uint32_t)(PDMFAN_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define AIRSHDN_MAX_NUM_OF_RETRIES \
        (uint32_t)(AIRSHDN_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define CANGLV_MAX_NUM_OF_RETRIES \
        (uint32_t)(CANGLV_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define ACCSEGFAN_MAX_NUM_OF_RETRIES \
        (uint32_t)(ACCSEGFAN_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define ACCENCFAN_MAX_NUM_OF_RETRIES \
        (uint32_t)(ACCENCFAN_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define LEFTINVERTER_MAX_NUM_OF_RETRIES \
        (uint32_t)(LEFTINVERTER_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)
#define RIGHTINVERTER_MAX_NUM_OF_RETRIES \
        (uint32_t)(RIGHTINVERTER_OVERCURRENT_TIMEOUT_MS / CONTROL_LOOP_PERIOD)

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void ErrorHandling_HandleError(PdmError_Enum error);

#endif /* ERRORHANDLING_H */
