/**
 * @brief Shared library with filters for digital signal processing
 */
#pragma once

#include <stm32f3xx_hal.h>
#include <arm_math.h>

/**
 * @brief Apply low pass filter on a given array of values based on the
 *        implementation described here:
 *        https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 * @param input Pointer to an array of input samples
 * @param output Pointer to an array of output samples
 * @param num_samples Number of samples
 * @param sampling_time Sampling time interval for input and output samples
 *                      in seconds
 * @param rc RC time constant
 */
void Io_SharedFilters_LowPassFilter(
    float32_t *input,
    float32_t *output,
    uint32_t   num_of_samples,
    float32_t  sampling_time,
    float32_t  rc);

/**
  * @brief  Filters ADC readings by computing the mean of the previous ADC_FILTER_SIZE readings (oversampling filter)
  * @param  ChannelCount Number of ADC channels used
  * @param  FilterSize Oversampling filter size (number of readings)
  * @param  ADC_Readings Pointer to array containing the unfiltered ADC readings
  * @param  Filtered_ADC_Readings Pointer to array in which the filtered ADC readings will be stored
  * @retval None
 */
void Filter_ADC_Readings(uint32_t ChannelCount, uint32_t FilterSize, uint16_t *ADC_Readings, float *Filtered_ADC_Readings);