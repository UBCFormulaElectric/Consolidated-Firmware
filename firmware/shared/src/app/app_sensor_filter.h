#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "app_utils.h"

/**
 * @brief Enumeration for alpha parameter specification type
 */
typedef enum
{
    ALPHA_DIRECT,               // Use alpha value directly
    ALPHA_FROM_CUTOFF_FREQUENCY // Calculate alpha from cutoff frequency and sample rate
} AlphaParameterType;

typedef struct
{
    float alpha;           // Smoothing factor (0.0 to 1.0)
    float previous_output; // Previous filter output
    float previous_input;  // Previous input value (for reset functionality)
    float initial_value;   // Initial value for reset functionality
    bool  is_initialized;  // Initialization flag
} ExponentialFilter;

/**
 * @brief Initialize Exponential Filter
 * @param filter Pointer to filter structure
 * @param param_type Type of alpha param (ALPHA_DIRECT or ALPHA_FROM_CUTOFF_FREQUENCY)
 * @param param_value If param_type is ALPHA_DIRECT: alpha value (0.0 to 1.0).
 *                    If param_type is ALPHA_FROM_CUTOFF_FREQUENCY: cutoff frequency in Hz
 * @param sample_rate Sample rate in Hz (only used when param_type is ALPHA_FROM_CUTOFF_FREQUENCY)
 * @param initial_value Initial value for the filter
 */
void app_sensor_filter_exponential_init(
    ExponentialFilter *filter,
    AlphaParameterType param_type,
    float              param_value,
    float              sample_rate,
    float              initial_value);

/**
 * @brief Process input through Exponential Filter
 * @param filter Pointer to filter structure
 * @param input New input sample
 * @return Filtered output
 */
float app_sensor_filter_exponential_process(ExponentialFilter *filter, float input);

/**
 * @brief Reset Exponential Filter to previous input states
 * @param filter Pointer to filter structure
 */
void app_sensor_filter_exponential_reset(ExponentialFilter *filter);

/**
 * @brief First-Order Butterworth IIR Filter
 *
 * A classic low-pass filter with a maximally flat passband.
 */
typedef struct
{
    float a1;              // Feedback coefficient
    float b0;              // Feedforward coefficient
    float b1;              // Feedforward coefficient
    float previous_input;  // Previous input value
    float previous_output; // Previous filter output
    float initial_value;   // Initial value for reset functionality
    bool  is_initialized;  // Initialization flag
} ButterworthFilter;

/**
 * @brief Initialize First-Order Butterworth Filter
 * @param filter Pointer to the filter structure
 * @param cutoff_frequency The -3dB cutoff frequency in Hz
 * @param sample_rate The rate at which data is sampled in Hz
 * @param initial_value The initial value for the filter
 */
void app_sensor_filter_butterworth_init(
    ButterworthFilter *filter,
    float              cutoff_frequency,
    float              sample_rate,
    float              initial_value);

/**
 * @brief Process an input value through the Butterworth filter
 * @param filter Pointer to the filter structure
 * @param input The new input sample
 * @return The filtered output
 */
float app_sensor_filter_butterworth_process(ButterworthFilter *filter, float input);

/**
 * @brief Reset the Butterworth filter to its initial state
 * @param filter Pointer to the filter structure
 */
void app_sensor_filter_butterworth_reset(ButterworthFilter *filter);

/**
 * @brief Second-Order Butterworth IIR Biquad Filter
 *
 */
typedef struct
{
    float coeffs[5];      // Filter coefficients: {b0, b1, b2, a1, a2}
    float state[4];       // State variables for Direct Form I: {x[n-1], x[n-2], y[n-1], y[n-2]}
    float previous_input; // Previous input value (for reset functionality)
    float initial_value;  // Initial value for reset functionality
    bool  is_initialized; // Initialization flag
} ButterworthBiquadFilter;

/**
 * @brief Initialize Second-Order Butterworth Biquad Filter
 * @param filter Pointer to the filter structure
 * @param cutoff_frequency The -3dB cutoff frequency in Hz
 * @param sample_rate The rate at which data is sampled in Hz
 * @param initial_value The initial value for the filter
 */
void app_sensor_filter_butterworth_biquad_init(
    ButterworthBiquadFilter *filter,
    float                    cutoff_frequency,
    float                    sample_rate,
    float                    initial_value);

/**
 * @brief Process an input value through the Butterworth biquad filter
 * @param filter Pointer to the filter structure
 * @param input The new input sample
 * @return The filtered output
 */
float app_sensor_filter_butterworth_biquad_process(ButterworthBiquadFilter *filter, float input);

/**
 * @brief Reset the Butterworth biquad filter to its previous input state
 * @param filter Pointer to the filter structure
 */
void app_sensor_filter_butterworth_biquad_reset(ButterworthBiquadFilter *filter);