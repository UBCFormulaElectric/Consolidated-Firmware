/**
 * @file  SharedFilters.h
 * @brief Shared library with filters for digital signal processing
 */
#ifndef SHARED_FILTERS_H
#define SHARED_FILTERS_H

/******************************************************************************
 * Includes
 ******************************************************************************/

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
 * @brief  Apply low pass filter on a given array of values
 * @param  input Pointer to an array of input samples
 * @param  output Pointer to an array of output samples
 * @param  smoothing_factor How fast the output samples respond to a change in 
 *                          the input samples
 */
void SharedFilter_LowPassFilter(uint32_t *input, uint32_t *output, uint32_t smoothing_factor);

#endif /* SHARED_FILTERS_H */
