#include "app_sensor_filter.h"
#include "io_log.h"
#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846


// IIR Moving Average Filter Implementation

void app_sensor_filter_iir_moving_average_init(IIRMovingAverageFilter *filter, uint32_t equivalent_window_size, float initial_value){
    if (filter == NULL)
    {
        LOG_ERROR("IIR Moving Average Filter: NULL filter pointer");
        return;
    }

    if (equivalent_window_size == 0)
    {
        LOG_ERROR("IIR Moving Average Filter: Invalid window size: %u", equivalent_window_size);
        return;
    }

    // Calculate alpha for equivalent window size
    // Formula: α = 2/(N+1) where N is equivalent window size
    // This provides similar smoothing characteristics to a moving average of N samples
    filter->alpha = 2.0f / (equivalent_window_size + 1.0f);
    filter->previous_output = initial_value;
    filter->initial_value = initial_value;
    filter->is_initialized = true;

    LOG_INFO("IIR Moving Average Filter initialized: window=%u, alpha=%.3f, initial=%.3f", equivalent_window_size, filter->alpha, initial_value);
}

float app_sensor_filter_iir_moving_average_process(IIRMovingAverageFilter *filter, float input)
{
    if (filter == NULL)
    {
        LOG_ERROR("IIR Moving Average Filter: NULL filter pointer");
        return input;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("IIR Moving Average Filter: Filter not initialized");
        return input;
    }

    // IIR approximation of moving average: output = (alpha * input) + [(1 - alpha) * previous_output]
    float output = filter->alpha * input + (1.0f - filter->alpha) * filter->previous_output;
    
    filter->previous_output = output;

    return output;
}

void app_sensor_filter_iir_moving_average_reset(IIRMovingAverageFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("IIR Moving Average Filter: NULL filter pointer");
        return;
    }

    filter->previous_output = filter->initial_value;
    LOG_INFO("IIR Moving Average Filter reset to initial value: %.3f", filter->initial_value);
}

float app_sensor_filter_iir_moving_average_get_output(const IIRMovingAverageFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("IIR Moving Average Filter: NULL filter pointer");
        return 0.0f;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("IIR Moving Average Filter: Filter not initialized");
        return 0.0f;
    }

    return filter->previous_output;
}

// Exponential Filter Implementation

void app_sensor_filter_exponential_init(ExponentialFilter *filter, float alpha, float initial_value){
    if (filter == NULL)
    {
        LOG_ERROR("Exponential Filter: NULL filter pointer");
        return;
    }

    if (alpha < 0.0f || alpha > 1.0f)
    {
        LOG_ERROR("Exponential Filter: Invalid alpha: %.3f (must be 0.0 to 1.0)", alpha);
        return;
    }

    filter->alpha = alpha;
    filter->previous_output = initial_value;
    filter->initial_value = initial_value;
    filter->is_initialized = true;

    LOG_INFO("Exponential Filter initialized: alpha=%.3f, initial=%.3f", alpha, initial_value);
}

float app_sensor_filter_exponential_process(ExponentialFilter *filter, float input)
{
    if (filter == NULL)
    {
        LOG_ERROR("Exponential Filter: NULL filter pointer");
        return input;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("Exponential Filter: Filter not initialized");
        return input;
    }

    // Exponential moving average: output = alpha * input + (1 - alpha) * previous_output
    float output = filter->alpha * input + (1.0f - filter->alpha) * filter->previous_output;
    filter->previous_output = output;

    return output;
}

void app_sensor_filter_exponential_reset(ExponentialFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("Exponential Filter: NULL filter pointer");
        return;
    }

    filter->previous_output = filter->initial_value;
    LOG_INFO("Exponential Filter reset to initial value: %.3f", filter->initial_value);
}

float app_sensor_filter_exponential_get_output(const ExponentialFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("Exponential Filter: NULL filter pointer");
        return 0.0f;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("Exponential Filter: Filter not initialized");
        return 0.0f;
    }

    return filter->previous_output;
}

void app_sensor_filter_butterworth_init(ButterworthFilter *filter, float cutoff_frequency, float sample_rate, float initial_value)
{
    if (filter == NULL)
    {
        LOG_ERROR("Butterworth Filter: NULL filter pointer");
        return;
    }

    if (cutoff_frequency <= 0.0f || sample_rate <= 0.0f)
    {
        LOG_ERROR("Butterworth Filter: Invalid cutoff frequency or sample rate");
        return;
    }

    float tan_arg = PI * cutoff_frequency / sample_rate; // got off research
    float gamma = 1.0f / tanf(tan_arg); // dont understand uses laplace transform it seems.
    float one_over_one_plus_gamma = 1.0f / (1.0f + gamma);

    // Calculate coefficients
    filter->b0 = one_over_one_plus_gamma;
    filter->b1 = filter->b0;
    filter->a1 = (1.0f - gamma) * one_over_one_plus_gamma;

    filter->previous_input = initial_value;
    filter->previous_output = initial_value;
    filter->initial_value = initial_value;
    filter->is_initialized = true;

    LOG_INFO("Butterworth Filter initialized: cutoff=%.2fHz, sample_rate=%.2fHz, initial=%.3f", cutoff_frequency, sample_rate, initial_value);
    LOG_INFO("Butterworth Coefficients: b0=%.3f, b1=%.3f, a1=%.3f", filter->b0, filter->b1, filter->a1);
}

float app_sensor_filter_butterworth_process(ButterworthFilter *filter, float input)
{
    if (filter == NULL)
    {
        LOG_ERROR("Butterworth Filter: NULL filter pointer");
        return input;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("Butterworth Filter: Filter not initialized");
        return input;
    }

    // Apply the difference equation
    float output = filter->b0 * input + filter->b1 * filter->previous_input - filter->a1 * filter->previous_output;

    // Update state for the next iteration
    filter->previous_input = input;
    filter->previous_output = output;

    return output;
}

void app_sensor_filter_butterworth_reset(ButterworthFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("Butterworth Filter: NULL filter pointer");
        return;
    }

    filter->previous_input = filter->initial_value;
    filter->previous_output = filter->initial_value;
    LOG_INFO("Butterworth Filter reset to initial value: %.3f", filter->initial_value);
}

float app_sensor_filter_butterworth_get_output(const ButterworthFilter *filter)
{
    if (filter == NULL)
    {
        LOG_ERROR("Butterworth Filter: NULL filter pointer");
        return 0.0f;
    }

    if (!filter->is_initialized)
    {
        LOG_ERROR("Butterworth Filter: Filter not initialized");
        return 0.0f;
    }

    return filter->previous_output;
}

