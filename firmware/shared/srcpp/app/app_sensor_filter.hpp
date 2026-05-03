#pragma once

/*
 * Digital Filter Library (C++ refactor of app_sensor_filter.[ch]).
 *
 * Find documentation @
 *   https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/939950081/Software+-+Documentation+-+Noise+Filters
 *
 * The library exposes three IIR filters, all derived from a common abstract
 * `SensorFilter` interface so they can be used polymorphically:
 *   - `ExponentialFilter`         : Single-pole exponential moving average
 *   - `ButterworthFilter`         : First-order Butterworth low-pass
 *   - `ButterworthBiquadFilter`   : Second-order Butterworth biquad (Direct Form I)
 *
 * Construction sets up filter coefficients and initial state in one shot, so
 * there is no separate "init" step. State can be re-anchored to the most
 * recently seen input via `reset()`.
 */

#include <array>
#include "util_units.hpp"

namespace app
{
/**
 * Abstract base class for sample-by-sample digital filters.
 *
 * A filter consumes one input sample at a time and produces one output sample
 * at a time. `reset()` re-anchors the filter's internal delay line to the
 * most recently processed input value.
 */
class SensorFilter
{
  public:
    virtual ~SensorFilter() = default;

    SensorFilter()                                    = default;
    SensorFilter(const SensorFilter &)                = default;
    SensorFilter &operator=(const SensorFilter &)     = default;
    SensorFilter(SensorFilter &&) noexcept            = default;
    SensorFilter &operator=(SensorFilter &&) noexcept = default;

    /**
     * Push a new input sample through the filter and return the filtered output.
     * @param input New input sample.
     * @return Filtered output for this sample.
     */
    [[nodiscard]] virtual float process(float input) = 0;

    /**
     * Re-anchor the filter's internal state to the most recently seen input
     * (or the configured initial value if no input has been processed yet),
     * which avoids "kicks" caused by stale initial values.
     */
    virtual void reset() = 0;
};

/**
 * Exponential Moving Average filter.
 *
 * Implements y[n] = alpha * x[n] + (1 - alpha) * y[n-1].
 *
 * Two construction modes are supported, exposed as named factory methods:
 *   - `withAlpha`            : Provide alpha directly in [0, 1].
 *   - `withCutoffFrequency`  : Compute alpha from a cutoff and sample rate.
 *
 * EMA filters resources:
 *  https://blog.mbedded.ninja/programming/signal-processing/digital-filters/exponential-moving-average-ema-filter/
 *  https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/Exponential-Moving-Average.pdf
 */
class ExponentialFilter final : public SensorFilter
{
  public:
    /**
     * Construct an exponential filter with a directly-specified alpha.
     * @param alpha Smoothing factor in [0, 1]. Higher = follow input faster.
     * @param initial_value Initial value for the delay line.
     */
    [[nodiscard]] static ExponentialFilter withAlpha(float alpha, float initial_value = 0.0f);

    /**
     * Construct an exponential filter where alpha is computed from cutoff
     * frequency and sample rate using:
     *   alpha = 1 - exp(-2*pi * fc / fs)
     * @param cutoff_frequency -3dB cutoff frequency, Hz (must be > 0).
     * @param sample_rate Sample rate, Hz (must be > 0).
     * @param initial_value Initial value for the delay line.
     */
    [[nodiscard]] static ExponentialFilter
        withCutoffFrequency(float cutoff_frequency, float sample_rate, float initial_value = 0.0f);

    [[nodiscard]] float process(float input) override;
    void                reset() override;

    [[nodiscard]] float getAlpha() const noexcept { return alpha_; }
    [[nodiscard]] float getPreviousOutput() const noexcept { return previous_output_; }
    [[nodiscard]] float getPreviousInput() const noexcept { return previous_input_; }
    [[nodiscard]] float getInitialValue() const noexcept { return initial_value_; }

  private:
    ExponentialFilter(float alpha, float initial_value);

    float alpha_;
    float previous_output_;
    float previous_input_;
    float initial_value_;
};

/**
 * First-order Butterworth low-pass IIR filter.
 *
 * Coefficients are designed via the bilinear transform with frequency
 * pre-warping. Difference equation (with stored a1):
 *   y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
 *
 * References:
 *  https://www.electronics-tutorials.ws/filter/filter_8.html
 *  https://tttapa.github.io/Arduino-Filters/Doxygen/d4/d40/Butterworth_8hpp_source.html
 *  https://en.wikipedia.org/wiki/Bilinear_transform
 */
class ButterworthFilter final : public SensorFilter
{
  public:
    /**
     * @param cutoff_frequency -3dB cutoff frequency, Hz (must be > 0).
     * @param sample_rate Sample rate, Hz (must be > 0).
     * @param initial_value Initial value for the delay line.
     */
    ButterworthFilter(float cutoff_frequency, float sample_rate, float initial_value = 0.0f);

    [[nodiscard]] float process(float input) override;
    void                reset() override;

    [[nodiscard]] float getB0() const noexcept { return b0_; }
    [[nodiscard]] float getB1() const noexcept { return b1_; }
    [[nodiscard]] float getA1() const noexcept { return a1_; }
    [[nodiscard]] float getPreviousInput() const noexcept { return previous_input_; }
    [[nodiscard]] float getPreviousOutput() const noexcept { return previous_output_; }
    [[nodiscard]] float getInitialValue() const noexcept { return initial_value_; }

  private:
    float b0_;
    float b1_;
    float a1_;
    float previous_input_;
    float previous_output_;
    float initial_value_;
};

/**
 * Second-order Butterworth low-pass biquad filter (Direct Form I).
 *
 * The cutoff must satisfy the Nyquist-Shannon criterion:
 *   cutoff_frequency < sample_rate / 2.
 *
 * Coefficient layout (matches the C version exactly):
 *   coeffs = { b0, b1, b2, a1, a2 }
 *   state  = { x[n-1], x[n-2], y[n-1], y[n-2] }
 *
 * References:
 *  https://www.earlevel.com/main/2011/01/02/biquad-formulas/
 *  https://www.dsprelated.com/freebooks/filters/Example_Second_Order_Butterworth_Lowpass.html
 */
class ButterworthBiquadFilter final : public SensorFilter
{
  public:
    /**
     * @param cutoff_frequency -3dB cutoff frequency, Hz (must be > 0 and < sample_rate / 2).
     * @param sample_rate Sample rate, Hz (must be > 0).
     * @param initial_value Initial value used to prime all delay-line slots.
     */
    ButterworthBiquadFilter(float cutoff_frequency, float sample_rate, float initial_value = 0.0f);

    [[nodiscard]] float process(float input) override;
    void                reset() override;

    [[nodiscard]] const std::array<float, 5> &getCoeffs() const noexcept { return coeffs_; }
    [[nodiscard]] const std::array<float, 4> &getState() const noexcept { return state_; }
    [[nodiscard]] float                       getPreviousInput() const noexcept { return previous_input_; }
    [[nodiscard]] float                       getInitialValue() const noexcept { return initial_value_; }

  private:
    // 1/Q for a 2nd order Butterworth: Q = 1/sqrt(2), so 1/Q = sqrt(2). DON'T CHANGE.
    static constexpr float kQInv = 1.41421356237f;

    std::array<float, 5> coeffs_{}; // {b0, b1, b2, a1, a2}
    std::array<float, 4> state_{};  // {x[n-1], x[n-2], y[n-1], y[n-2]}
    float                previous_input_;
    float                initial_value_;
};
} // namespace app
