#include "app_sensor_filter.hpp"

#include <cassert>
#include <cmath>

namespace app
{
// =============================================================================
// ExponentialFilter
// =============================================================================

ExponentialFilter::ExponentialFilter(float alpha, float initial_value)
  : alpha_(alpha), previous_output_(initial_value), previous_input_(initial_value), initial_value_(initial_value)
{
}

ExponentialFilter ExponentialFilter::withAlpha(float alpha, float initial_value)
{
    assert(alpha >= 0.0f && alpha <= 1.0f);
    return { alpha, initial_value };
}

ExponentialFilter ExponentialFilter::withCutoffFrequency(float cutoff_frequency, float sample_rate, float initial_value)
{
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);

    // alpha = 1 - exp(-2*pi * fc / fs)
    // where fc is cutoff frequency and fs is sample rate
    const float alpha = 1.0f - std::exp(-2.0f * M_PI_F * cutoff_frequency / sample_rate);
    return { alpha, initial_value };
}

float ExponentialFilter::process(float input)
{
    // Exponential moving average: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
    const float output = alpha_ * input + (1.0f - alpha_) * previous_output_;
    previous_output_   = output;
    previous_input_    = input;
    return output;
}

void ExponentialFilter::reset()
{
    // Re-anchor to the most recent unfiltered input to avoid using a stale
    // initial value as the new "y[n-1]".
    previous_output_ = previous_input_;
}

// =============================================================================
// ButterworthFilter (1st order)
// =============================================================================

ButterworthFilter::ButterworthFilter(float cutoff_frequency, float sample_rate, float initial_value)
  : previous_input_(initial_value), previous_output_(initial_value), initial_value_(initial_value)
{
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);

    const float tan_arg                 = M_PI_F * cutoff_frequency / sample_rate;
    const float gamma                   = 1.0f / std::tan(tan_arg);
    const float one_over_one_plus_gamma = 1.0f / (1.0f + gamma);

    // Standard form: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // For DC gain = 1: (b0 + b1) / (1 + a1) = 1, so b0 + b1 = 1 + a1
    // With b0 = b1 = 1/(1+gamma) and a1 = (1-gamma)/(1+gamma):
    //   b0 + b1 = 2/(1+gamma) and 1 + a1 = 2/(1+gamma).
    b0_ = one_over_one_plus_gamma;
    b1_ = b0_;
    a1_ = (1.0f - gamma) * one_over_one_plus_gamma; // typically negative since gamma > 1
}

float ButterworthFilter::process(float input)
{
    // y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
    // Note: a1 is stored as (1-gamma)/(1+gamma); the sign carries through.
    const float output = b0_ * input + b1_ * previous_input_ - a1_ * previous_output_;

    previous_input_  = input;
    previous_output_ = output;
    return output;
}

void ButterworthFilter::reset()
{
    previous_output_ = previous_input_;
}

// =============================================================================
// ButterworthBiquadFilter (2nd order, Direct Form I)
// =============================================================================

ButterworthBiquadFilter::ButterworthBiquadFilter(float cutoff_frequency, float sample_rate, float initial_value)
  : previous_input_(initial_value), initial_value_(initial_value)
{
    assert(cutoff_frequency > 0.0f);
    assert(sample_rate > 0.0f);
    // Nyquist-Shannon sampling theorem: cutoff must be < fs / 2.
    assert(cutoff_frequency < sample_rate / 2.0f);

    // Pre-warped frequency for the bilinear transform:
    //   K = tan(pi * fc / fs)
    const float K         = std::tan(M_PI_F * cutoff_frequency / sample_rate);
    const float K_squared = K * K;

    // Bilinear-transformed analog Butterworth prototype:
    //   denominator = 1 + (1/Q)*K + K^2     with 1/Q = sqrt(2)
    const float denominator = 1.0f + kQInv * K + K_squared;

    coeffs_[0] = K_squared / denominator;                      // b0
    coeffs_[1] = 2.0f * K_squared / denominator;               // b1
    coeffs_[2] = K_squared / denominator;                      // b2
    coeffs_[3] = 2.0f * (K_squared - 1.0f) / denominator;      // a1 (stored positive, negated in process())
    coeffs_[4] = (1.0f - kQInv * K + K_squared) / denominator; // a2 (stored positive, negated in process())

    // Prime the delay line so a constant initial value produces no transient.
    state_[0] = initial_value; // x[n-1]
    state_[1] = initial_value; // x[n-2]
    state_[2] = initial_value; // y[n-1]
    state_[3] = initial_value; // y[n-2]
}

float ButterworthBiquadFilter::process(float input)
{
    // Direct Form I difference equation:
    //   y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
    const float output = coeffs_[0] * input + coeffs_[1] * state_[0] + coeffs_[2] * state_[1] - coeffs_[3] * state_[2] -
                         coeffs_[4] * state_[3];

    // Shift the delay line.
    state_[1] = state_[0]; // x[n-2] = x[n-1]
    state_[0] = input;     // x[n-1] = x[n]
    state_[3] = state_[2]; // y[n-2] = y[n-1]
    state_[2] = output;    // y[n-1] = y[n]

    previous_input_ = input;
    return output;
}

void ButterworthBiquadFilter::reset()
{
    // Re-anchor to the most recent input. For Direct Form I this approximates
    // the steady state where every delayed sample equals the constant input.
    const float reset_value = previous_input_;
    state_[0]               = reset_value; // x[n-1]
    state_[1]               = reset_value; // x[n-2]
    state_[2]               = reset_value; // y[n-1]
    state_[3]               = reset_value; // y[n-2]
}
} // namespace app
