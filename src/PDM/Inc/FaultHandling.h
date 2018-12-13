/**
 * @file  FaultHandling.h
 * @brief Fault Handling Library
 */
#ifndef FAULTHANDLING_H
#define FAULTHANDLING_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stm32f3xx_hal.h"
#include "CurrentSense.h"

/******************************************************************************
 * Preprocessor Constants
 *****************************************************************************/
// clang-format off

/******************************************************************************
* Preprocessor Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/
// clang-format on
typedef enum
{
  EFUSE_ON = GPIO_PIN_SET,
  EFUSE_OFF = !EFUSE_ON
} EfuseOnOff_GPIO_PinState;

/******************************************************************************
 * Global Variables
 *****************************************************************************/
extern volatile GPIO_PinState dsel_state;

/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
/**
 * @brief Checks if any current or voltage readings exceed their respective
 *        limits and executes output-specific fault handling behaviour.
 * @param fault_states Array with NumReadings*ChannelCount
 * elements which tracks outputs that need to be renabled or are permanently
 * faulted
 * @param converted_readings Array with NumReadings*ChannelCount
 * elements that tracks converted current/voltage readings from ADC counts to A
 * or V
 */
void FaultHandling_Handler(
    volatile uint8_t *fault_states,
    volatile float *  converted_readings);

/**
 * @brief  Renable any E-Fuses that have faulted but have not exceeded their
 *         max number of retries
 * @param  fault_states Array with (2 x ChannelCount) elements that tracks
 *         outputs which need to be renabled or are permanently off
 *         (error state)
 */
void FaultHandling_RetryEFuse(volatile uint8_t *fault_states);

#endif /* FAULTHANDLING_H */
