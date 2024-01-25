/**
 * @brief Shared library with filters for digital signal processing
 */
#pragma once

#include "hw_hal.h"

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
    float   *input,
    float   *output,
    uint32_t num_of_samples,
    float    sampling_time,
    float    rc);
