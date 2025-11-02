// EMA filters:
// https://blog.mbedded.ninja/programming/signal-processing/digital-filters/exponential-moving-average-ema-filter/
//              https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/Exponential-Moving-Average.pdf
//      The new alpha formula:
//              https://tomroelandts.com/articles/low-pass-single-pole-iir-filter
// IIR filter wrapper: smoothing factor formula: https://www.wealthsimple.com/en-ca/learn/moving-average
//                     this wrapper was created first as a learning exercise and doesn't have any practical benefits
//                     over the actual IIR moving average filter only over a simple moving average filter.
// Butterworth filter: https://www.electronics-tutorials.ws/filter/filter_8.html
#include "app_sensor_filter.h"
#include <assert.h>
#include <string.h>
#include "app_units.h"

// IIR Moving Average Filter Implementation.
// Not a pure recursive filter, it's an approximation that tries to mimic a moving average filter using IIR techniques.
// It acts as a target spec; so giving a certain N is basically saying "act as a 5 sample moving average".
// Window size is just used to approximate a good alpha value.
void app_sensor_filter_iir_moving_average_init(
    IIRMovingAverageFilter *filter,
    uint32_t                equivalent_window_size,
    float                   initial_value)
{
    assert(filter != NULL);
    assert(equivalent_window_size > 0);

    // Calculate alpha for equivalent window size
    // Formula: α = 2/(N+1) where N is equivalent window size
    // This provides similar smoothing characteristics to a moving average of N samples
    filter->alpha           = 2.0f / ((float)equivalent_window_size + 1.0f);
    filter->previous_output = initial_value;
    filter->initial_value   = initial_value;
    filter->is_initialized  = true;
}

float app_sensor_filter_iir_moving_average_process(IIRMovingAverageFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // IIR approximation of moving average: output = (alpha * input) + [(1 - alpha) * previous_output]
    float output = filter->alpha * input + (1.0f - filter->alpha) * filter->previous_output;

    filter->previous_output = output;

    return output;
}

void app_sensor_filter_iir_moving_average_reset(IIRMovingAverageFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset to initial state by restoring the initial value
    filter->previous_output = filter->initial_value;
}

// Exponential Filter Implementation

void app_sensor_filter_exponential_init(
    ExponentialFilter *filter,
    AlphaParameterType param_type,
    float              param_value,
    float              sample_rate,
    float              initial_value)
{
    assert(filter != NULL);

    float alpha;

    switch (param_type)
    {
        case ALPHA_DIRECT:
            // Use alpha value directly
            assert(param_value >= 0.0f && param_value <= 1.0f);
            alpha = param_value;
            break;

        case ALPHA_FROM_CUTOFF_FREQUENCY:
            // Calculate alpha from cutoff frequency
            assert(param_value > 0.0f); // cutoff_frequency
            assert(sample_rate > 0.0f);
            // Formula: α = 1 - exp(-2π * fc / fs)
            // where fc is cutoff frequency and fs is sample rate
            alpha = 1.0f - expf(-2.0f * M_PI_F * param_value / sample_rate);
            break;

        default:
            // Invalid param type -> default to ALPHA_DIRECT
            assert(param_value >= 0.0f && param_value <= 1.0f);
            alpha = param_value;
            break;
    }

    filter->alpha           = alpha;
    filter->previous_output = initial_value;
    filter->initial_value   = initial_value;
    filter->is_initialized  = true;
}

float app_sensor_filter_exponential_process(ExponentialFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Exponential moving average: output = alpha * input + (1 - alpha) * previous_output
    float output            = filter->alpha * input + (1.0f - filter->alpha) * filter->previous_output;
    filter->previous_output = output;

    return output;
}

void app_sensor_filter_exponential_reset(ExponentialFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset to initial state by restoring the initial value
    filter->previous_output = filter->initial_value;
}

void app_sensor_filter_butterworth_init(
    ButterworthFilter *filter,
    float              cutoff_frequency,
    float              sample_rate,
    float              initial_value)
{
    assert(filter != NULL);
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);

    float tan_arg                 = M_PI_F * cutoff_frequency / sample_rate; // got off research
    float gamma                   = 1.0f / tanf(tan_arg); // dont understand uses laplace transform it seems.
    float one_over_one_plus_gamma = 1.0f / (1.0f + gamma);

    // Calculate coefficients
    filter->b0 = one_over_one_plus_gamma;
    filter->b1 = filter->b0;
    filter->a1 = (1.0f - gamma) * one_over_one_plus_gamma;

    filter->previous_input  = initial_value;
    filter->previous_output = initial_value;
    filter->initial_value   = initial_value;
    filter->is_initialized  = true;
}

float app_sensor_filter_butterworth_process(ButterworthFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Apply the difference equation
    float output = filter->b0 * input + filter->b1 * filter->previous_input - filter->a1 * filter->previous_output;

    // Update state for the next iteration
    filter->previous_input  = input;
    filter->previous_output = output;

    return output;
}

void app_sensor_filter_butterworth_reset(ButterworthFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset to initial state by restoring the initial value
    filter->previous_input  = filter->initial_value;
    filter->previous_output = filter->initial_value;
}