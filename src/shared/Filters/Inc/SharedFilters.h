/**
 * @file  SharedFilters.h
 * @brief Shared library with filters for digital signal processing
 */
#ifndef SHARED_FILTERS_H
#define SHARED_FILTERS_H

/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

#include "arm_math.h"

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

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Apply low pass filter on a given array of values
 * @param input Pointer to an array of input samples
 * @param output Pointer to an array of output samples
 * @param sampling_time Sampling time interval for input and output samples
 * @param rc RC time constant
 */
void SharedFilter_LowPassFilter(
    float32_t *input,
    float32_t *output,
    uint32_t   sampling_time,
    uint32_t   rc);

#endif /* SHARED_FILTERS_H */
