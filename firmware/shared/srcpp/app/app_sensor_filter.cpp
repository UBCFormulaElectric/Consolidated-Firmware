/*
 Digital Filter Library:
 find documentation @
 https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/939950081/Software+-+Documentation+-+Noise+Filters
*/

#include "app_sensor_filter.hpp"
#include <cassert>
#include <cmath>

namespace app
{
/*
 Exponential Filter Implementation
 EMA filters resources:
 https://blog.mbedded.ninja/programming/signal-processing/digital-filters/exponential-moving-average-ema-filter/
 https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/Exponential-Moving-Average.pdf

 This filter has two modes of setting the alpha parameter:
 1) Directly specifying alpha (0.0 to 1.0)
 2) Calculating alpha from cutoff frequency and sample rate:

 ExponentialFilter filter(
     AlphaParameterType::ALPHA_DIRECT,           // Use alpha directly
     0.2f,                                       // alpha = 0.2 (20% new, 80% old)
     0.0f,                                       // sample_rate not used
     0.0f                                        // initial_value
 );

 OR

 ExponentialFilter filter(
     AlphaParameterType::ALPHA_FROM_CUTOFF_FREQUENCY,  // Calculate from frequency
     10.0f,                                            // cutoff_frequency = 10 Hz
     100.0f,                                           // sample_rate = 100 Hz
     0.0f                                              // initial_value
 );
*/

ExponentialFilter::ExponentialFilter(
    AlphaParameterType param_type,
    float              param_value,
    float              sample_rate,
    float              initial_value)
  : previous_output(initial_value), previous_input(initial_value), initial_value(initial_value), is_initialized(true)
{
    switch (param_type)
    {
        case AlphaParameterType::ALPHA_DIRECT:
            // Use alpha value directly
            assert(param_value >= 0.0f && param_value <= 1.0f);
            alpha = param_value;
            break;

        case AlphaParameterType::ALPHA_FROM_CUTOFF_FREQUENCY:
            // Calculate alpha from cutoff frequency
            assert(param_value > 0.0f); // cutoff_frequency
            assert(sample_rate > 0.0f);
            // Formula: α = 1 - exp(-2π * fc / fs)
            // where fc is cutoff frequency and fs is sample rate
            alpha = 1.0f - std::exp(-2.0f * M_PI_F * param_value / sample_rate);
            break;

        default:
            // Invalid param type -> default to ALPHA_DIRECT
            assert(param_value >= 0.0f && param_value <= 1.0f);
            alpha = param_value;
            break;
    }
}

float ExponentialFilter::process(float input)
{
    assert(is_initialized);
    // Exponential moving average: output = alpha * input + (1 - alpha) * previous_output
    float output    = alpha * input + (1.0f - alpha) * previous_output;
    previous_output = output;
    previous_input  = input;
    return output;
}

void ExponentialFilter::reset()
{
    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    assert(is_initialized);
    previous_output = previous_input;
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

ButterworthFilter::ButterworthFilter(float cutoff_frequency, float sample_rate, float initial_value)
  : previous_input(initial_value), previous_output(initial_value), initial_value(initial_value), is_initialized(true)
{
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);

    float tan_arg                 = M_PI_F * cutoff_frequency / sample_rate; // got off research
    float gamma                   = 1.0f / std::tan(tan_arg); // dont understand uses laplace transform it seems.
    float one_over_one_plus_gamma = 1.0f / (1.0f + gamma);

    // Calculate coefficients
    // Standard form: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // For DC gain = 1: (b0 + b1) / (1 + a1) = 1, so b0 + b1 = 1 + a1
    // With b0 = b1 = 1/(1+gamma) and a1 = (1-gamma)/(1+gamma):
    // b0 + b1 = 2/(1+gamma) and 1 + a1 = 2/(1+gamma)
    b0 = one_over_one_plus_gamma;
    b1 = b0;
    a1 = (1.0f - gamma) * one_over_one_plus_gamma;
}

float ButterworthFilter::process(float input)
{
    assert(is_initialized);
    // Apply the difference equation: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // Note: a1 is stored as (1-gamma)/(1+gamma), which is negative when gamma > 1
    // So -a1 is positive, meaning we're adding a positive feedback term
    // Update state for the next iteration
    float output    = b0 * input + b1 * previous_input - a1 * previous_output;
    previous_input  = input;
    previous_output = output;
    return output;
}

void ButterworthFilter::reset()
{
    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    assert(is_initialized);
    previous_output = previous_input;
}

/* Second-Order Butterworth DF1 Biquad Filter Implementation
 Based on bilinear transform method for 2nd order Butterworth lowpass filter
 References:
 - Bilinear transformation: https://drive.google.com/file/d/1DWD-mlCiXZQRKY8twKZ-w1JyBcgIiOFM/view?usp=sharing,
 https://drive.google.com/file/d/1N4QLeThiRkMvv1Coa4Jz7XBrzQYjDaYG/view?usp=sharing
 - https://www.dsprelated.com/freebooks/filters/Example_Second_Order_Butterworth_Lowpass.html
 - https://www.mathworks.com/help/signal/ref/butter.html
*/

ButterworthBiquadFilter::ButterworthBiquadFilter(float cutoff_frequency, float sample_rate, float initial_value)
  : previous_input(initial_value), initial_value(initial_value), is_initialized(true)
{
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);
    // Nyquist-Shannon sampling theorem: https://www.mathworks.com/discovery/nyquist-theorem.html
    assert(cutoff_frequency < sample_rate / 2.0f); // Nyquist-Shannon criteria

    // Calculate pre-warped frequency for bilinear transform
    // Reference of formulae: https://www.earlevel.com/main/2011/01/02/biquad-formulas/
    // K = tan(π·fc/fs) where fc is cutoff frequency and fs is sample rate
    // This pre-warping compensates for frequency warping in the bilinear transform
    float K         = std::tan(M_PI_F * cutoff_frequency / sample_rate);
    float K_squared = SQUARE(K);

    // For 2nd order Butterworth filter: Q = 1/√2 ≈ 0.707106781
    const float Q_inv = Q_INV;
    // Calculate denominator: 1 + √2·K + K²
    // This comes from the bilinear transform of the analog Butterworth prototype
    float denominator = 1.0f + Q_inv * K + K_squared;

    coeffs[0] = K_squared / denominator;                      // b0
    coeffs[1] = 2.0f * K_squared / denominator;               // b1
    coeffs[2] = K_squared / denominator;                      // b2
    coeffs[3] = 2.0f * (K_squared - 1.0f) / denominator;      // a1 (stored positive, negated in processing)
    coeffs[4] = (1.0f - Q_inv * K + K_squared) / denominator; // a2 (stored positive, negated in processing)

    // Initialize state variables for Direct Form I: {x[n-1], x[n-2], y[n-1], y[n-2]}
    // Set to initial value to avoid startup transients
    state[0] = initial_value;
    state[1] = initial_value;
    state[2] = initial_value;
    state[3] = initial_value;
}

float ButterworthBiquadFilter::process(float input)
{
    assert(is_initialized);
    // Direct Form I implementation
    // Filter equation: y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
    // State variables: state[0] = x[n-1], state[1] = x[n-2], state[2] = y[n-1], state[3] = y[n-2]

    // Calculate output using current input and delayed inputs/outputs
    float output =
        coeffs[0] * input + coeffs[1] * state[0] + coeffs[2] * state[1] - coeffs[3] * state[2] - coeffs[4] * state[3];

    state[1]       = state[0];
    state[0]       = input;
    state[3]       = state[2];
    state[2]       = output;
    previous_input = input;
    return output;
}

void ButterworthBiquadFilter::reset()
{
    assert(is_initialized);
    // Reset to current unfiltered output (previous input) to avoid using outdated initial value
    // For Direct Form I, set all state variables to the previous input value
    // This approximates steady state where all delayed inputs and outputs equal the constant input
    float reset_value = previous_input;
    state[0]          = reset_value;
    state[1]          = reset_value;
    state[2]          = reset_value;
    state[3]          = reset_value;
}
} // namespace app