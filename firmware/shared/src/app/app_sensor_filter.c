/*
 Digital Filter Library:
 find documentation @
 https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/939950081/Software+-+Documentation+-+Noise+Filters
*/

#include "app_sensor_filter.h"
#include <assert.h>
#include <string.h>
#include "app_units.h"

typedef float float32_t;

// Constant for Q inverse for 2nd order Butterworth filter (DON'T CHANGE)
#define Q_INV 1.41421356237f

/*
 Exponential Filter Implementation
 EMA filters resources:
 https://blog.mbedded.ninja/programming/signal-processing/digital-filters/exponential-moving-average-ema-filter/
 https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/Exponential-Moving-Average.pdf

 This filter has two modes of setting the alpha parameter:
 1) Directly specifying alpha (0.0 to 1.0)
 2) Calculating alpha from cutoff frequency and sample rate:

 app_sensor_filter_exponential_init(
     &filter,
     ALPHA_DIRECT,           // Use alpha directly
     0.2f,                   // alpha = 0.2 (20% new, 80% old)
     0.0f,                   // sample_rate not used
     0.0f                    // initial_value
 );

 OR

 app_sensor_filter_exponential_init(
     &filter,
     ALPHA_FROM_CUTOFF_FREQUENCY,  // Calculate from frequency
     10.0f,                        // cutoff_frequency = 10 Hz
     100.0f,                       // sample_rate = 100 Hz
     0.0f                          // initial_value
 );
*/

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
    filter->previous_input  = initial_value;
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
    filter->previous_input  = input;

    return output;
}

void app_sensor_filter_exponential_reset(ExponentialFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    filter->previous_output = filter->previous_input;
}

/*
 First-Order Butterworth Filter Implementation
 Based on bilinear transform method + pre-warping theory for 1st order Butterworth lowpass filter
 References:
 Butterworth filter logic: https://www.electronics-tutorials.ws/filter/filter_8.html
 Coefficient source: https://tttapa.github.io/Arduino-Filters/Doxygen/d4/d40/Butterworth_8hpp_source.html
 Bilinear transformation: https://en.wikipedia.org/wiki/Bilinear_transform
                        : https://drive.google.com/file/d/1N4QLeThiRkMvv1Coa4Jz7XBrzQYjDaYG/view?usp=sharing (page 505
                        of document) :
                        https://drive.google.com/file/d/1DWD-mlCiXZQRKY8twKZ-w1JyBcgIiOFM/view?usp=sharing (page 678)
 IIR basic formula and general IIR filter design:
 https://ethz.ch/content/dam/ethz/special-interest/mavt/dynamic-systems-n-control/idsc-dam/Lectures/Signals-and-Systems/Lectures/Lecture%20Notes%209.pdf
*/

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
    // Standard form: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // For DC gain = 1: (b0 + b1) / (1 + a1) = 1, so b0 + b1 = 1 + a1
    // With b0 = b1 = 1/(1+gamma) and a1 = (1-gamma)/(1+gamma):
    // b0 + b1 = 2/(1+gamma) and 1 + a1 = 2/(1+gamma)
    filter->b0 = one_over_one_plus_gamma;
    filter->b1 = filter->b0;
    filter->a1 = (1.0f - gamma) * one_over_one_plus_gamma; // This will be negative (gamma > 1 typically)

    filter->previous_input  = initial_value;
    filter->previous_output = initial_value;
    filter->initial_value   = initial_value;
    filter->is_initialized  = true;
}

float app_sensor_filter_butterworth_process(ButterworthFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Apply the difference equation: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // Note: a1 is stored as (1-gamma)/(1+gamma), which is negative when gamma > 1
    // So -a1 is positive, meaning we're adding a positive feedback term
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

    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    filter->previous_output = filter->previous_input;
}

/* Second-Order Butterworth DF1 Biquad Filter Implementation
 Based on bilinear transform method for 2nd order Butterworth lowpass filter
 References:
 - Bilinear transformation: https://drive.google.com/file/d/1DWD-mlCiXZQRKY8twKZ-w1JyBcgIiOFM/view?usp=sharing,
 https://drive.google.com/file/d/1N4QLeThiRkMvv1Coa4Jz7XBrzQYjDaYG/view?usp=sharing
 - https://www.dsprelated.com/freebooks/filters/Example_Second_Order_Butterworth_Lowpass.html
 - https://www.mathworks.com/help/signal/ref/butter.html
*/

void app_sensor_filter_butterworth_biquad_init(
    ButterworthBiquadFilter *filter,
    float                    cutoff_frequency,
    float                    sample_rate,
    float                    initial_value)
{
    assert(filter != NULL);
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);
    // Nyquist-Shannon sampling theorem: https://www.mathworks.com/discovery/nyquist-theorem.html
    assert(cutoff_frequency < sample_rate / 2.0f); // Nyquist-Shannon criteria

    // Calculate pre-warped frequency for bilinear transform
    // Reference of formulae: https://www.earlevel.com/main/2011/01/02/biquad-formulas/
    // K = tan(π·fc/fs) where fc is cutoff frequency and fs is sample rate
    // This pre-warping compensates for frequency warping in the bilinear transform
    float K         = tanf(M_PI_F * cutoff_frequency / sample_rate);
    float K_squared = SQUARE(K);

    // For 2nd order Butterworth filter: Q = 1/√2 ≈ 0.707106781
    const float Q_inv = Q_INV; // 1/Q = √2 for Butterworth

    // Calculate denominator: 1 + √2·K + K²
    // This comes from the bilinear transform of the analog Butterworth prototype
    float denominator = 1.0f + Q_inv * K + K_squared;

    // Calculate filter coefficients for Direct Form I
    // Format: {b0, b1, b2, a1, a2}
    filter->coeffs[0] = K_squared / denominator;                      // b0
    filter->coeffs[1] = 2.0f * K_squared / denominator;               // b1
    filter->coeffs[2] = K_squared / denominator;                      // b2
    filter->coeffs[3] = 2.0f * (K_squared - 1.0f) / denominator;      // a1 (stored positive, negated in processing)
    filter->coeffs[4] = (1.0f - Q_inv * K + K_squared) / denominator; // a2 (stored positive, negated in processing)

    // Initialize state variables for Direct Form I: {x[n-1], x[n-2], y[n-1], y[n-2]}
    // Set to initial value to avoid startup transients
    filter->state[0] = initial_value; // x[n-1]
    filter->state[1] = initial_value; // x[n-2]
    filter->state[2] = initial_value; // y[n-1]
    filter->state[3] = initial_value; // y[n-2]

    // Set initial value for reset functionality
    filter->initial_value  = initial_value;
    filter->previous_input = initial_value;

    filter->is_initialized = true;
}

float app_sensor_filter_butterworth_biquad_process(ButterworthBiquadFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Direct Form I implementation
    // Filter equation: y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
    // State variables: state[0] = x[n-1], state[1] = x[n-2], state[2] = y[n-1], state[3] = y[n-2]

    // Calculate output using current input and delayed inputs/outputs
    float output = filter->coeffs[0] * input               // b0*x[n]
                   + filter->coeffs[1] * filter->state[0]  // b1*x[n-1]
                   + filter->coeffs[2] * filter->state[1]  // b2*x[n-2]
                   - filter->coeffs[3] * filter->state[2]  // -a1*y[n-1]
                   - filter->coeffs[4] * filter->state[3]; // -a2*y[n-2]

    // Update state variables (shift delay line)
    filter->state[1] = filter->state[0]; // x[n-2] = x[n-1]
    filter->state[0] = input;            // x[n-1] = x[n]
    filter->state[3] = filter->state[2]; // y[n-2] = y[n-1]
    filter->state[2] = output;           // y[n-1] = y[n]

    filter->previous_input = input;

    return output;
}

void app_sensor_filter_butterworth_biquad_reset(ButterworthBiquadFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    // For Direct Form I, set all state variables to the previous input value
    // This approximates steady state where all delayed inputs and outputs equal the constant input
    float reset_value = filter->previous_input;
    filter->state[0]  = reset_value; // x[n-1]
    filter->state[1]  = reset_value; // x[n-2]
    filter->state[2]  = reset_value; // y[n-1]
    filter->state[3]  = reset_value; // y[n-2]
}