#pragma once

#include <stdint.h>
#include <assert.h>

// Maximum size of the FIR filter window
#define APP_FIR_FILTER_MAX_SIZE (64U)

/**
 * Structure to represent an FIR filter.
 */
typedef struct
{
    // Circular buffer to store the samples
    float samples[APP_FIR_FILTER_MAX_SIZE];

    // Coefficients for the FIR filter
    float coefficients[APP_FIR_FILTER_MAX_SIZE];

    // Current index in the circular buffer
    uint16_t index;

    // Number of samples in the filter window
    uint16_t window_size;
} AppFIRFilter;

/**
 * Initialize the FIR filter.
 *
 * @param filter Pointer to the AppFIRFilter structure.
 * @param window_size The size of the FIR filter window (must not exceed APP_FIR_FILTER_MAX_SIZE).
 * @param coefficients Array of filter coefficients (must have at least window_size elements).
 */
void app_fir_filter_init(AppFIRFilter *filter, uint16_t window_size, const float *coefficients);

/**
 * Apply the FIR filter to a new sample.
 *
 * @param filter Pointer to the AppFIRFilter structure.
 * @param new_sample The new sample to be added to the filter.
 * @return The filtered value.
 */
float app_fir_filter_apply(AppFIRFilter *filter, float new_sample);
