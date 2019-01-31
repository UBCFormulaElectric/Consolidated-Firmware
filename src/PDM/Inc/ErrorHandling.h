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

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// TODO: Convert this numer of retries into sensible values, the current values
//       are just some placeholder
#define AUX1_MAX_NUM_OF_RETRIES             (uint32_t)(10)
#define AUX2_MAX_NUM_OF_RETRIES             (uint32_t)(10)
#define COOLING_MAX_NUM_OF_RETRIES          (uint32_t)(10)
#define PDMFAN_MAX_NUM_OF_RETRIES           (uint32_t)(10)
#define AIRSHDN_MAX_NUM_OF_RETRIES          (uint32_t)(10)
#define CANGLV_MAX_NUM_OF_RETRIES           (uint32_t)(10)
#define ACCSEGFAN_MAX_NUM_OF_RETRIES        (uint32_t)(10)
#define ACCENCFAN_MAX_NUM_OF_RETRIES        (uint32_t)(10)
#define LEFTINVERTER_MAX_NUM_OF_RETRIES     (uint32_t)(10)
#define RIGHTINVERTER_MAX_NUM_OF_RETRIES    (uint32_t)(10)

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
