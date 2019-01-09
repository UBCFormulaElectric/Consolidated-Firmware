/**
 * @file  FaultHandling.h
 * @brief Fault Handling Library
 */
#ifndef FAULTHANDLING_H
#define FAULTHANDLING_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"
#include "CurrentSense.h"
#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

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
/**
 * @brief Checks if any current or voltage readings exceed their respective
 *        limits and executes output-specific fault handling behaviour.
 */
void FaultHandling_Handler(void);

/**
 * @brief Re-enable any E-Fuses that have faulted but have not exceeded their
 *        max number of retries
 */
void FaultHandling_RetryEFuse(void);

#endif /* FAULTHANDLING_H */
