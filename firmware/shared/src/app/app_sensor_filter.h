#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "app_utils.h"

/**
 * @brief IIR Moving Average Filter
 * 
 * This filter provides an IIR approximation of a moving average filter.
 * It uses exponential smoothing with calculated alpha to approximate
 * the behavior of a moving average with specified window size.
 * 
 */

typedef struct
{
    float alpha;              // Calculated smoothing factor
    float previous_output;    // Previous filter output
    float initial_value;      // Initial value for reset
    bool is_initialized;      // Initialization flag
} IIRMovingAverageFilter;

typedef struct
{
    float alpha;              // Smoothing factor (0.0 to 1.0)
    float previous_output;    // Previous filter output
    float initial_value;      // Initial value for reset
    bool is_initialized;      // Initialization flag
} ExponentialFilter;

/**
 * @brief Initialize IIR Moving Average Filter
 * @param filter Pointer to filter structure
 * @param equivalent_window_size Equivalent window size for approximation
 * @param initial_value Initial value for the filter
 */
void app_sensor_filter_iir_moving_average_init(IIRMovingAverageFilter *filter, 
                                               uint32_t equivalent_window_size, 
                                               float initial_value);

/**
 * @brief Process input through IIR Moving Average Filter
 * @param filter Pointer to filter structure
 * @param input New input sample
 * @return Filtered output
 */
float app_sensor_filter_iir_moving_average_process(IIRMovingAverageFilter *filter, float input);

/**
 * @brief Reset IIR Moving Average Filter to initial state
 * @param filter Pointer to filter structure
 */
void app_sensor_filter_iir_moving_average_reset(IIRMovingAverageFilter *filter);

/**
 * @brief Get current output of IIR Moving Average Filter
 * @param filter Pointer to filter structure
 * @return Current filtered output
 */
float app_sensor_filter_iir_moving_average_get_output(const IIRMovingAverageFilter *filter);

/**
 * @brief Initialize Exponential Filter
 * @param filter Pointer to filter structure
 * @param alpha Smoothing factor (0.0 to 1.0, higher = more responsive)
 * @param initial_value Initial value for the filter
 */
void app_sensor_filter_exponential_init(ExponentialFilter *filter, 
                                        float alpha, 
                                        float initial_value);

/**
 * @brief Process input through Exponential Filter
 * @param filter Pointer to filter structure
 * @param input New input sample
 * @return Filtered output
 */
float app_sensor_filter_exponential_process(ExponentialFilter *filter, float input);

/**
 * @brief Reset Exponential Filter to initial state
 * @param filter Pointer to filter structure
 */
void app_sensor_filter_exponential_reset(ExponentialFilter *filter);

/**
 * @brief Get current output of Exponential Filter
 * @param filter Pointer to filter structure
 * @return Current filtered output
 */
float app_sensor_filter_exponential_get_output(const ExponentialFilter *filter);

/**
 * @brief First-Order Butterworth IIR Filter
 *
 * A classic low-pass filter with a maximally flat passband.
 */
typedef struct
{
    float a1;               // Feedback coefficient
    float b0;               // Feedforward coefficient
    float b1;               // Feedforward coefficient
    float previous_input;   // Previous input value
    float previous_output;  // Previous filter output
    float initial_value;    // Initial value for reset
    bool is_initialized;    // Initialization flag
} ButterworthFilter;

/**
 * @brief Initialize First-Order Butterworth Filter
 * @param filter Pointer to the filter structure
 * @param cutoff_frequency The -3dB cutoff frequency in Hz
 * @param sample_rate The rate at which data is sampled in Hz
 * @param initial_value The initial value for the filter
 */
void app_sensor_filter_butterworth_init(ButterworthFilter *filter,
                                        float cutoff_frequency,
                                        float sample_rate,
                                        float initial_value);

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
 * @brief Get the current output of the Butterworth filter
 * @param filter Pointer to the filter structure
 * @return The current filtered output
 */
float app_sensor_filter_butterworth_get_output(const ButterworthFilter *filter);

