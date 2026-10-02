#include <gtest/gtest.h>
#include <cmath>

#include "app_sensor_filter.hpp"

using app::ButterworthBiquadFilter;
using app::ButterworthFilter;
using app::ExponentialFilter;
using app::SensorFilter;

class SensorFilterCppTest : public ::testing::Test
{
};

// =============================================================================
// ExponentialFilter
// =============================================================================

TEST_F(SensorFilterCppTest, Exponential_BasicFunctionality)
{
    auto filter = ExponentialFilter::withCutoffFrequency(1.0f, 10.0f, 0.0f);

    const float output1 = filter.process(10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f);

    const float output2 = filter.process(20.0f);
    EXPECT_GT(output2, output1);
}

TEST_F(SensorFilterCppTest, Exponential_Reset)
{
    auto filter = ExponentialFilter::withCutoffFrequency(1.0f, 10.0f, 5.0f);

    (void)filter.process(10.0f);
    (void)filter.process(20.0f);

    // Reset to current unfiltered output (last input = 20.0).
    filter.reset();

    // After reset, previous_output_ == 20.0 (the last input, not the initial value).
    const float output = filter.process(5.0f);
    EXPECT_GT(output, 5.0f);
    EXPECT_LT(output, 20.0f);
    EXPECT_GT(output, 10.0f); // closer to 20.0 than to 5.0
}

TEST_F(SensorFilterCppTest, Exponential_DifferentCutoffFrequencies)
{
    auto fast_filter = ExponentialFilter::withCutoffFrequency(5.0f, 10.0f, 0.0f);
    auto slow_filter = ExponentialFilter::withCutoffFrequency(0.5f, 10.0f, 0.0f);

    const float input = 10.0f;

    const float fast_output = fast_filter.process(input);
    const float slow_output = slow_filter.process(input);

    // Higher cutoff should track input more aggressively.
    EXPECT_GT(fast_output, slow_output);
}

TEST_F(SensorFilterCppTest, Exponential_AlphaCalculation)
{
    const auto filter1 = ExponentialFilter::withCutoffFrequency(1.0f, 10.0f, 0.0f);
    const auto filter2 = ExponentialFilter::withCutoffFrequency(5.0f, 10.0f, 0.0f);

    const float alpha1 = filter1.getAlpha();
    const float alpha2 = filter2.getAlpha();

    EXPECT_GT(alpha2, alpha1);
    EXPECT_GT(alpha1, 0.0f);
    EXPECT_LT(alpha1, 1.0f);
    EXPECT_GT(alpha2, 0.0f);
    EXPECT_LT(alpha2, 1.0f);
}

TEST_F(SensorFilterCppTest, Exponential_DirectAlpha)
{
    const auto direct = ExponentialFilter::withAlpha(0.5f, 0.0f);
    EXPECT_NEAR(direct.getAlpha(), 0.5f, 0.01f);

    // Direct alpha and the equivalent computed cutoff should match.
    const auto  direct_03 = ExponentialFilter::withAlpha(0.3f, 0.0f);
    const float fc        = -10.0f * std::log(0.7f) / (2.0f * M_PI_F);
    const auto  cutoff_03 = ExponentialFilter::withCutoffFrequency(fc, 10.0f, 0.0f);

    EXPECT_NEAR(direct_03.getAlpha(), cutoff_03.getAlpha(), 0.001f);
}

// =============================================================================
// ButterworthFilter (1st order)
// =============================================================================

TEST_F(SensorFilterCppTest, Butterworth_BasicFunctionality)
{
    ButterworthFilter filter(1.0f, 10.0f, 0.0f);

    const float output1 = filter.process(10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f);

    const float output2 = filter.process(20.0f);
    EXPECT_GT(output2, output1);
}

TEST_F(SensorFilterCppTest, Butterworth_Reset)
{
    ButterworthFilter filter(1.0f, 10.0f, 5.0f);

    (void)filter.process(10.0f);
    (void)filter.process(20.0f);

    filter.reset();

    const float output = filter.process(5.0f);
    EXPECT_GT(output, 5.0f);
    EXPECT_LT(output, 20.0f);
    EXPECT_GT(output, 10.0f);
}

TEST_F(SensorFilterCppTest, Butterworth_DifferentCutoffFrequencies)
{
    ButterworthFilter fast_filter(5.0f, 10.0f, 0.0f);
    ButterworthFilter slow_filter(0.5f, 10.0f, 0.0f);

    const float input = 10.0f;

    const float fast_output = fast_filter.process(input);
    const float slow_output = slow_filter.process(input);

    EXPECT_GT(fast_output, slow_output);
}

TEST_F(SensorFilterCppTest, Butterworth_CoefficientCalculation)
{
    const ButterworthFilter filter(1.0f, 10.0f, 0.0f);

    EXPECT_GT(filter.getB0(), 0.0f);
    EXPECT_GT(filter.getB1(), 0.0f);

    // Stable first-order pole: -1 < a1 < 1.
    EXPECT_LT(filter.getA1(), 1.0f);
    EXPECT_GT(filter.getA1(), -1.0f);

    // For 1st-order Butterworth, b0 == b1.
    EXPECT_NEAR(filter.getB0(), filter.getB1(), 0.01f);
}

TEST_F(SensorFilterCppTest, Butterworth_DCGain)
{
    ButterworthFilter filter(1.0f, 10.0f, 0.0f);

    const float dc_value = 5.0f;

    float output = 0.0f;
    for (int i = 0; i < 100; i++)
    {
        output = filter.process(dc_value);
    }

    EXPECT_NEAR(output, dc_value, 0.1f);
}

TEST_F(SensorFilterCppTest, Butterworth_StepResponse)
{
    ButterworthFilter filter(1.0f, 10.0f, 0.0f);

    const float step_value = 10.0f;

    float output = 0.0f;
    for (int i = 0; i < 100; i++)
    {
        output = filter.process(step_value);
    }

    EXPECT_NEAR(output, step_value, 0.1f);
    EXPECT_GT(output, step_value * 0.9f);
    EXPECT_LT(output, step_value * 1.1f);
}

TEST_F(SensorFilterCppTest, Butterworth_Stability)
{
    const float sample_rate = 10.0f;
    const float cutoffs[]   = { 0.1f, 0.5f, 1.0f, 2.0f, 4.0f };

    for (const float cutoff : cutoffs)
    {
        ButterworthFilter filter(cutoff, sample_rate, 0.0f);

        EXPECT_LT(filter.getA1(), 1.0f);
        EXPECT_GT(filter.getA1(), -1.0f);

        const float input       = 10.0f;
        float       prev_output = 0.0f;
        bool        converged   = false;

        for (int i = 0; i < 100; i++)
        {
            const float output = filter.process(input);

            // Output must stay bounded.
            EXPECT_LT(std::abs(output), input * 10.0f);

            if (i > 50)
            {
                const float diff = std::abs(output - prev_output);
                if (diff < 0.01f)
                {
                    converged = true;
                }
            }
            prev_output = output;
        }

        EXPECT_TRUE(converged);
        EXPECT_NEAR(prev_output, input, 0.5f);
    }
}

// =============================================================================
// ButterworthBiquadFilter (2nd order)
// =============================================================================

TEST_F(SensorFilterCppTest, ButterworthBiquad_BasicFunctionality)
{
    ButterworthBiquadFilter filter(1.0f, 10.0f, 0.0f);

    EXPECT_NEAR(filter.getState()[0], 0.0f, 0.001f);
    EXPECT_NEAR(filter.getState()[1], 0.0f, 0.001f);

    const float output1 = filter.process(10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f);

    const float output2 = filter.process(20.0f);
    EXPECT_GT(output2, output1);
}

TEST_F(SensorFilterCppTest, ButterworthBiquad_Reset)
{
    ButterworthBiquadFilter filter(1.0f, 10.0f, 5.0f);

    (void)filter.process(10.0f);
    (void)filter.process(20.0f);

    EXPECT_NE(filter.getState()[0], 5.0f);
    EXPECT_NE(filter.getState()[1], 5.0f);

    filter.reset();

    // After reset, state should be re-anchored to last processed input (20.0).
    EXPECT_NEAR(filter.getState()[0], 20.0f, 0.001f);
    EXPECT_NEAR(filter.getState()[1], 20.0f, 0.001f);

    const float output = filter.process(5.0f);
    EXPECT_GT(output, 5.0f);
    EXPECT_LT(output, 20.0f);
    EXPECT_GT(output, 10.0f);
}

// =============================================================================
// Polymorphic usage via SensorFilter base class.
// =============================================================================

TEST_F(SensorFilterCppTest, Polymorphic_BaseClassUsage)
{
    auto                    ema       = ExponentialFilter::withAlpha(0.5f, 0.0f);
    ButterworthFilter       butter    = { 1.0f, 10.0f, 0.0f };
    ButterworthBiquadFilter biquad    = { 1.0f, 10.0f, 0.0f };
    SensorFilter           *filters[] = { &ema, &butter, &biquad };

    // Drive every filter to steady state with the same DC input through the base
    // class interface; each must converge near the input value.
    const float dc_value = 5.0f;
    for (SensorFilter *f : filters)
    {
        float output = 0.0f;
        for (int i = 0; i < 200; i++)
        {
            output = f->process(dc_value);
        }
        EXPECT_NEAR(output, dc_value, 0.1f);

        f->reset();
    }
}
