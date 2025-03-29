#include "app_fir_filter.h"

/**
 * Initialize the FIR filter with the specified window size and coefficients.
 */
void app_fir_filter_init(AppFIRFilter *filter, uint16_t window_size, const float *coefficients)
{
    // Ensure the window size is valid
    assert(window_size > 0 && window_size <= APP_FIR_FILTER_MAX_SIZE);

    // Initialize the buffer and variables
    for (uint16_t i = 0; i < APP_FIR_FILTER_MAX_SIZE; i++)
    {
        filter->coefficients[i] = i < window_size ? coefficients[i] : 0.0f;
    }

    filter->index       = 0;
    filter->window_size = window_size;
}

/**
 * Apply the FIR filter to a new sample and return the filtered result.
 */
float app_fir_filter_apply(AppFIRFilter *filter, float new_sample)
{
    // Ensure the window size is valid
    assert(filter->window_size > 0);

    // Insert the new sample into the circular buffer
    filter->samples[filter->index] = new_sample;

    // Compute the weighted sum of the samples
    float result = 0.0f;
    for (uint16_t i = 0; i < filter->window_size; i++)
    {
        const uint16_t sample_index = (uint16_t)((filter->index + filter->window_size - i) % filter->window_size);
        result += filter->samples[sample_index] * filter->coefficients[i];
    }

    // Advance the index in the circular buffer
    filter->index = (uint16_t)((filter->index + 1) % filter->window_size);

    return result;
}
