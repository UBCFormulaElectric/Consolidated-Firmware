#pragma once

#include <cstdint>
#include <cmath>

namespace app
{

// Constant for Q inverse for 2nd order Butterworth filter (DON'T CHANGE)
constexpr float Q_INV = 1.41421356237f;

// Helper constant for M_PI as float as most cpp files use their own constexpressions eventhough it is redundant to
// shared code.
constexpr float M_PI_F = static_cast<float>(M_PI);

// Helper macro for squaring
#define SQUARE(x) ((x) * (x))

/**
 * @brief Enum class for alpha parameter specification type
 */
enum class AlphaParameterType
{
    ALPHA_DIRECT,               // Use alpha value directly
    ALPHA_FROM_CUTOFF_FREQUENCY // Calculate alpha from cutoff frequency and sample rate
};

/**
 * @brief Exponential Moving Average (EMA) Filter
 *
 * This filter has two modes of setting the alpha parameter:
 * 1) Directly specifying alpha (0.0 to 1.0)
 * 2) Calculating alpha from cutoff frequency and sample rate
 *
 * Example usage:
 * ExponentialFilter filter(AlphaParameterType::ALPHA_DIRECT, 0.2f, 0.0f, 0.0f);
 * OR
 * ExponentialFilter filter(AlphaParameterType::ALPHA_FROM_CUTOFF_FREQUENCY, 10.0f, 100.0f, 0.0f);
 */
class ExponentialFilter
{
  private:
    float alpha;           // Smoothing factor (0.0 to 1.0)
    float previous_output; // Previous filter output
    float previous_input;  // Previous input value (for reset functionality)
    float initial_value;   // Initial value for reset functionality
    bool  is_initialized;  // Initialization flag

  public:
    /**
     * @brief Initialize Exponential Filter
     * @param param_type Type of alpha param (ALPHA_DIRECT or ALPHA_FROM_CUTOFF_FREQUENCY)
     * @param param_value If param_type is ALPHA_DIRECT: alpha value (0.0 to 1.0).
     *                    If param_type is ALPHA_FROM_CUTOFF_FREQUENCY: cutoff frequency in Hz
     * @param sample_rate Sample rate in Hz (only used when param_type is ALPHA_FROM_CUTOFF_FREQUENCY)
     * @param initial_value Initial value for the filter
     */
    ExponentialFilter(AlphaParameterType param_type, float param_value, float sample_rate, float initial_value);

    /**
     * @brief Process input through Exponential Filter
     * @param input New input sample
     * @return Filtered output
     */
    [[nodiscard]] float process(float input);

    /**
     * @brief Reset Exponential Filter to previous input states
     */
    void reset();
};

/**
 * @brief First-Order Butterworth IIR Filter
 *
 * A classic low-pass filter with a maximally flat passband.
 */
class ButterworthFilter
{
  private:
    float a1;              // Feedback coefficient
    float b0;              // Feedforward coefficient
    float b1;              // Feedforward coefficient
    float previous_input;  // Previous input value
    float previous_output; // Previous filter output
    float initial_value;   // Initial value for reset functionality
    bool  is_initialized;  // Initialization flag

  public:
    /**
     * @brief Initialize First-Order Butterworth Filter
     * @param cutoff_frequency The -3dB cutoff frequency in Hz
     * @param sample_rate The rate at which data is sampled in Hz
     * @param initial_value The initial value for the filter
     */
    ButterworthFilter(float cutoff_frequency, float sample_rate, float initial_value);

    /**
     * @brief Process an input value through the Butterworth filter
     * @param input The new input sample
     * @return The filtered output
     */
    [[nodiscard]] float process(float input);

    /**
     * @brief Reset the Butterworth filter to its initial state
     */
    void reset();
};

/**
 * @brief Second-Order Butterworth IIR Biquad Filter
 */
class ButterworthBiquadFilter
{
  private:
    float coeffs[5];      // Filter coefficients: {b0, b1, b2, a1, a2}
    float state[4];       // State variables for Direct Form I: {x[n-1], x[n-2], y[n-1], y[n-2]}
    float previous_input; // Previous input value (for reset functionality)
    float initial_value;  // Initial value for reset functionality
    bool  is_initialized; // Initialization flag

  public:
    /**
     * @brief Initialize Second-Order Butterworth Biquad Filter
     * @param cutoff_frequency The -3dB cutoff frequency in Hz
     * @param sample_rate The rate at which data is sampled in Hz
     * @param initial_value The initial value for the filter
     */
    ButterworthBiquadFilter(float cutoff_frequency, float sample_rate, float initial_value);

    /**
     * @brief Process an input value through the Butterworth biquad filter
     * @param input The new input sample
     * @return The filtered output
     */
    [[nodiscard]] float process(float input);

    /**
     * @brief Reset the Butterworth biquad filter to its previous input state
     */
    void reset();
};
} // namespace app