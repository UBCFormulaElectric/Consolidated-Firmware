// EMA filters:
// https://blog.mbedded.ninja/programming/signal-processing/digital-filters/exponential-moving-average-ema-filter/
//              https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/Exponential-Moving-Average.pdf
//      The new alpha formula:
//              https://tomroelandts.com/articles/low-pass-single-pole-iir-filter
// IIR filter wrapper: smoothing factor formula: https://www.wealthsimple.com/en-ca/learn/moving-average
//                     this wrapper was created first as a learning exercise and doesn't have any practical benefits
//                     over the actual IIR moving average filter only over a simple moving average filter.
// Butterworth filter: https://www.electronics-tutorials.ws/filter/filter_8.html
// 2nd order Butterworth biquad: https://www.dsprelated.com/freebooks/filters/Example_Second_Order_Butterworth_Lowpass.html
//                               https://www.mathworks.com/help/signal/ref/butter.html
#include "app_sensor_filter.h"
#include <assert.h>
#include <string.h>
#include "app_units.h"

// Include CMSIS-DSP for optimized biquad filter processing
#ifdef ARM_MATH_CM4
#include "arm_math.h"
#elif defined(ARM_MATH_CM7)
#include "arm_math.h"
#elif defined(ARM_MATH_CM33)
#include "arm_math.h"
#else
// Fallback: define float32_t if CMSIS-DSP not available
typedef float float32_t;
#endif

#define Q_INV 1.41421356237f

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

// Second-Order Butterworth Biquad Filter Implementation using CMSIS-DSP
// Based on bilinear transform method for 2nd order Butterworth lowpass filter
// References:
// - Bilinear transformation: https://drive.google.com/file/d/1DWD-mlCiXZQRKY8twKZ-w1JyBcgIiOFM/view?usp=sharing, https://drive.google.com/file/d/1N4QLeThiRkMvv1Coa4Jz7XBrzQYjDaYG/view?usp=sharing
// - https://www.dsprelated.com/freebooks/filters/Example_Second_Order_Butterworth_Lowpass.html
// - https://www.mathworks.com/help/signal/ref/butter.html

void app_sensor_filter_butterworth_biquad_init(
    ButterworthBiquadFilter *filter,
    float                    cutoff_frequency,
    float                    sample_rate,
    float                    initial_value)
{
    assert(filter != NULL);
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);
    assert(cutoff_frequency < sample_rate / 2.0f); // nyquist criteria for anti-aliasing

    // Calculate pre-warped frequency for bilinear transform
    // K = tan(π·fc/fs) where fc is cutoff frequency and fs is sample rate
    // This pre-warping compensates for frequency warping in the bilinear transform
    float K = tanf(M_PI_F * cutoff_frequency / sample_rate);
    float K_squared = SQUARE(K);

    // For 2nd order Butterworth filter: Q = 1/√2 ≈ 0.707106781
    const float Q_inv = Q_INV;                 // 1/Q = √2 for Butterworth

    // Calculate denominator: 1 + √2·K + K²
    // This comes from the bilinear transform of the analog Butterworth prototype
    float denominator = 1.0f + Q_inv * K + K_squared;

    // Calculate filter coefficients for Direct Form II Transposed
    // Format: {b0, b1, b2, a1, a2}
    filter->coeffs[0] = K_squared / denominator;                    // b0
    filter->coeffs[1] = 2.0f * K_squared / denominator;            // b1
    filter->coeffs[2] = K_squared / denominator;                   // b2
    filter->coeffs[3] = 2.0f * (K_squared - 1.0f) / denominator;  // a1 (stored positive, negated in processing)
    filter->coeffs[4] = (1.0f - Q_inv * K + K_squared) / denominator; // a2 (stored positive, negated in processing)

    // Initialize state variables to zero
    filter->state[0] = 0.0f; // d1
    filter->state[1] = 0.0f; // d2

    // Set initial value for reset functionality
    filter->initial_value = initial_value;

    // Pre-fill state with initial value to avoid startup transients
    // This approximates the filter being at steady state with the initial value
    filter->state[0] = initial_value;
    filter->state[1] = initial_value;

    filter->is_initialized = true;
}

float app_sensor_filter_butterworth_biquad_process(ButterworthBiquadFilter *filter, float input)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Direct Form II Transposed implementation
    // CMSIS-DSP structure (arm_biquad_cascade_df2T)

    // --------- IGNORE PERSONAL NOTES ---------------------
    // The implementation is optimized for single sample processing to match the existing API pattern
    // For block processing of multiple samples, consider using arm_biquad_cascade_df2T_f32 directly
    // -----------------------------------------------------

    // Filter equations:
    // y[n] = b0*x[n] + d1
    // d1 = b1*x[n] - a1*y[n] + d2
    // d2 = b2*x[n] - a2*y[n]

    float output = filter->coeffs[0] * input + filter->state[0];

    // Update state variables
    // Note: a1 and a2 are stored as positive values, so negate them here
    float new_d1 = filter->coeffs[1] * input - filter->coeffs[3] * output + filter->state[1];
    float new_d2 = filter->coeffs[2] * input - filter->coeffs[4] * output;

    filter->state[0] = new_d1;
    filter->state[1] = new_d2;

    return output;
}

void app_sensor_filter_butterworth_biquad_reset(ButterworthBiquadFilter *filter)
{
    assert(filter != NULL);
    assert(filter->is_initialized);

    // Reset state variables to initial value
    filter->state[0] = filter->initial_value;
    filter->state[1] = filter->initial_value;
}