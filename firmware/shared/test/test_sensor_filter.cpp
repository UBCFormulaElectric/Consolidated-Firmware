#include <gtest/gtest.h>
#include <cmath>

extern "C"
{
#include "app_sensor_filter.h"
}

class SensorFilterTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // No setup needed bc filters are statically allocated
    }

    void TearDown() override
    {
        // No cleanup needed bc no dynamic allocation
    }
};

// Test IIR Moving Average Filter
TEST_F(SensorFilterTest, IIRMovingAverage_BasicFunctionality)
{
    IIRMovingAverageFilter filter{};

    // Initialize with window size 5
    app_sensor_filter_iir_moving_average_init(&filter, 5, 0.0f);

    // Test processing samples
    // For window size 5, alpha = 2/(5+1) = 0.333333...
    // First sample: output = 0.333333 * 10 + 0.666667 * 0 = 3.33333
    float output1 = app_sensor_filter_iir_moving_average_process(&filter, 10.0f);
    EXPECT_NEAR(output1, 3.333f, 0.002f);

    // Second sample: output = 0.333333 * 20 + 0.666667 * 3.33333 = 6.66667 + 2.22222 = 8.88889
    float output2 = app_sensor_filter_iir_moving_average_process(&filter, 20.0f);
    EXPECT_NEAR(output2, 8.889f, 0.002f);

    // Vfy filter state
    EXPECT_TRUE(filter.is_initialized);
    EXPECT_NEAR(filter.alpha, 0.333f, 0.001f);
}

TEST_F(SensorFilterTest, IIRMovingAverage_Reset)
{
    IIRMovingAverageFilter filter{};

    app_sensor_filter_iir_moving_average_init(&filter, 5, 10.0f);

    // Process some samples
    app_sensor_filter_iir_moving_average_process(&filter, 20.0f);
    app_sensor_filter_iir_moving_average_process(&filter, 30.0f);

    // Reset to initial state
    app_sensor_filter_iir_moving_average_reset(&filter);

    // After reset, should behave like initial state
    // After reset, previous_output = 10.0 (initial value)
    // Processing 5.0: output = 0.333 * 5 + 0.667 * 10 = 1.665 + 6.67 = 8.335
    float output = app_sensor_filter_iir_moving_average_process(&filter, 5.0f);
    EXPECT_NEAR(output, 8.335f, 0.002f); // 0.333 * 5 + 0.667 * 10
}

TEST_F(SensorFilterTest, IIRMovingAverage_DifferentWindowSizes)
{
    IIRMovingAverageFilter fast_filter, slow_filter;

    // Fast filter (window size 3, alpha = 0.5)
    app_sensor_filter_iir_moving_average_init(&fast_filter, 3, 0.0f);

    // Slow filter (window size 10, alpha = 0.182)
    app_sensor_filter_iir_moving_average_init(&slow_filter, 10, 0.0f);

    // Test with step input
    float input = 10.0f;

    // Process several samples
    for (int i = 0; i < 10; i++)
    {
        float fast_output = app_sensor_filter_iir_moving_average_process(&fast_filter, input);
        float slow_output = app_sensor_filter_iir_moving_average_process(&slow_filter, input);

        // Fast filter should respond faster
        if (i > 0)
        {
            EXPECT_GT(fast_output, slow_output);
        }
    }
}

TEST_F(SensorFilterTest, IIRMovingAverage_AlphaCalculation)
{
    IIRMovingAverageFilter filter;

    // Test alpha calculation for different window sizes
    app_sensor_filter_iir_moving_average_init(&filter, 1, 0.0f);
    EXPECT_NEAR(filter.alpha, 1.0f, 0.01f); // alpha = 2/(1+1) = 1.0

    app_sensor_filter_iir_moving_average_init(&filter, 2, 0.0f);
    EXPECT_NEAR(filter.alpha, 0.667f, 0.01f); // alpha = 2/(2+1) = 0.667

    app_sensor_filter_iir_moving_average_init(&filter, 10, 0.0f);
    EXPECT_NEAR(filter.alpha, 0.182f, 0.01f); // alpha = 2/(10+1) = 0.182
}

// Test Exponential Filter
TEST_F(SensorFilterTest, Exponential_BasicFunctionality)
{
    ExponentialFilter filter;

    // Initialize with cutoff frequency 1Hz and sample rate 10Hz
    app_sensor_filter_exponential_init(&filter, ALPHA_FROM_CUTOFF_FREQUENCY, 1.0f, 10.0f, 0.0f);

    // Test processing samples
    float output1 = app_sensor_filter_exponential_process(&filter, 10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f); // Should be filtered

    float output2 = app_sensor_filter_exponential_process(&filter, 20.0f);
    EXPECT_GT(output2, output1); // Should be increasing towards input
}

TEST_F(SensorFilterTest, Exponential_Reset)
{
    ExponentialFilter filter;

    app_sensor_filter_exponential_init(&filter, ALPHA_FROM_CUTOFF_FREQUENCY, 1.0f, 10.0f, 5.0f);

    // Process some samples
    app_sensor_filter_exponential_process(&filter, 10.0f);
    app_sensor_filter_exponential_process(&filter, 20.0f);

    // Reset
    app_sensor_filter_exponential_reset(&filter);

    // After reset, should behave like initial state
    float output = app_sensor_filter_exponential_process(&filter, 5.0f);
    EXPECT_NEAR(output, 5.0f, 0.1f); // Should be close to initial value
}

TEST_F(SensorFilterTest, Exponential_DifferentCutoffFrequencies)
{
    ExponentialFilter fast_filter, slow_filter;

    // Fast filter (higher cutoff frequency)
    app_sensor_filter_exponential_init(&fast_filter, ALPHA_FROM_CUTOFF_FREQUENCY, 5.0f, 10.0f, 0.0f);

    // Slow filter (lower cutoff frequency)
    app_sensor_filter_exponential_init(&slow_filter, ALPHA_FROM_CUTOFF_FREQUENCY, 0.5f, 10.0f, 0.0f);

    // Test with step input
    float input = 10.0f;

    float fast_output = app_sensor_filter_exponential_process(&fast_filter, input);
    float slow_output = app_sensor_filter_exponential_process(&slow_filter, input);

    // Fast filter should be closer to the input
    EXPECT_GT(fast_output, slow_output);
}

TEST_F(SensorFilterTest, Exponential_AlphaCalculation)
{
    ExponentialFilter filter;

    // Test alpha calculation for different frequencies
    app_sensor_filter_exponential_init(&filter, ALPHA_FROM_CUTOFF_FREQUENCY, 1.0f, 10.0f, 0.0f);
    float alpha1 = filter.alpha;

    app_sensor_filter_exponential_init(&filter, ALPHA_FROM_CUTOFF_FREQUENCY, 5.0f, 10.0f, 0.0f);
    float alpha2 = filter.alpha;

    // Higher cutoff frequency should result in higher alpha
    EXPECT_GT(alpha2, alpha1);
    EXPECT_GT(alpha1, 0.0f);
    EXPECT_LT(alpha1, 1.0f);
    EXPECT_GT(alpha2, 0.0f);
    EXPECT_LT(alpha2, 1.0f);
}

TEST_F(SensorFilterTest, Exponential_DirectAlpha)
{
    ExponentialFilter filter;

    // Test with direct alpha value
    app_sensor_filter_exponential_init(&filter, ALPHA_DIRECT, 0.5f, 0.0f, 0.0f);
    EXPECT_NEAR(filter.alpha, 0.5f, 0.01f);

    // Test that direct alpha and calculated alpha from cutoff frequency produce same result
    ExponentialFilter filter_direct, filter_cutoff;
    app_sensor_filter_exponential_init(&filter_direct, ALPHA_DIRECT, 0.3f, 0.0f, 0.0f);

    // Calculate what cutoff frequency would give alpha = 0.3 with sample rate 10Hz
    // alpha = 1 - exp(-2π * fc / fs)
    // 0.3 = 1 - exp(-2π * fc / 10)
    // exp(-2π * fc / 10) = 0.7
    // -2π * fc / 10 = ln(0.7)
    // fc = -10 * ln(0.7) / (2π) ≈ 0.568 Hz
    const float M_PI_F            = static_cast<float>(M_PI);
    float       calculated_cutoff = -10.0f * logf(0.7f) / (2.0f * M_PI_F);
    app_sensor_filter_exponential_init(&filter_cutoff, ALPHA_FROM_CUTOFF_FREQUENCY, calculated_cutoff, 10.0f, 0.0f);
    EXPECT_NEAR(filter_direct.alpha, filter_cutoff.alpha, 0.001f);
}

// Test Butterworth Filter
TEST_F(SensorFilterTest, Butterworth_BasicFunctionality)
{
    ButterworthFilter filter;

    // Initialize with cutoff frequency 1Hz and sample rate 10Hz
    app_sensor_filter_butterworth_init(&filter, 1.0f, 10.0f, 0.0f);

    // Test processing samples
    float output1 = app_sensor_filter_butterworth_process(&filter, 10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f); // Should be filtered

    float output2 = app_sensor_filter_butterworth_process(&filter, 20.0f);
    EXPECT_GT(output2, output1); // Should be increasing towards input
}

TEST_F(SensorFilterTest, Butterworth_Reset)
{
    ButterworthFilter filter;

    app_sensor_filter_butterworth_init(&filter, 1.0f, 10.0f, 5.0f);

    // Process some samples
    app_sensor_filter_butterworth_process(&filter, 10.0f);
    app_sensor_filter_butterworth_process(&filter, 20.0f);

    // Reset
    app_sensor_filter_butterworth_reset(&filter);

    // After reset, should behave like initial state
    float output = app_sensor_filter_butterworth_process(&filter, 5.0f);
    EXPECT_NEAR(output, 5.0f, 0.1f); // Should be close to initial value
}

TEST_F(SensorFilterTest, Butterworth_DifferentCutoffFrequencies)
{
    ButterworthFilter fast_filter, slow_filter;

    // Fast filter (higher cutoff frequency)
    app_sensor_filter_butterworth_init(&fast_filter, 5.0f, 10.0f, 0.0f);

    // Slow filter (lower cutoff frequency)
    app_sensor_filter_butterworth_init(&slow_filter, 0.5f, 10.0f, 0.0f);

    // Test with step input
    float input = 10.0f;

    float fast_output = app_sensor_filter_butterworth_process(&fast_filter, input);
    float slow_output = app_sensor_filter_butterworth_process(&slow_filter, input);

    // Fast filter should be closer to the input
    EXPECT_GT(fast_output, slow_output);
}

TEST_F(SensorFilterTest, Butterworth_CoefficientCalculation)
{
    ButterworthFilter filter;

    // Test coefficient calculation
    app_sensor_filter_butterworth_init(&filter, 1.0f, 10.0f, 0.0f);

    // Coefficients should be valid
    EXPECT_GT(filter.b0, 0.0f);
    EXPECT_GT(filter.b1, 0.0f);
    EXPECT_LT(filter.a1, 1.0f);
    EXPECT_GT(filter.a1, -1.0f);

    // b0 should equal b1 for first-order Butterworth
    EXPECT_NEAR(filter.b0, filter.b1, 0.01f);
}

// Test Butterworth Biquad Filter
TEST_F(SensorFilterTest, ButterworthBiquad_BasicFunctionality)
{
    ButterworthBiquadFilter filter{};

    // Initialize with cutoff frequency 1Hz and sample rate 10Hz
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);

    // Verify initialization
    EXPECT_TRUE(filter.is_initialized);
    EXPECT_NEAR(filter.state[0], 0.0f, 0.001f); // Initial value was 0.0
    EXPECT_NEAR(filter.state[1], 0.0f, 0.001f);

    // Test processing samples
    float output1 = app_sensor_filter_butterworth_biquad_process(&filter, 10.0f);
    EXPECT_GT(output1, 0.0f);
    EXPECT_LT(output1, 10.0f); // Should be filtered

    float output2 = app_sensor_filter_butterworth_biquad_process(&filter, 20.0f);
    EXPECT_GT(output2, output1); // Should be increasing towards input
}

TEST_F(SensorFilterTest, ButterworthBiquad_Reset)
{
    ButterworthBiquadFilter filter{};

    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 5.0f);

    // Process some samples
    app_sensor_filter_butterworth_biquad_process(&filter, 10.0f);
    app_sensor_filter_butterworth_biquad_process(&filter, 20.0f);

    // Verify state changed
    EXPECT_NE(filter.state[0], 5.0f);
    EXPECT_NE(filter.state[1], 5.0f);

    // Reset
    app_sensor_filter_butterworth_biquad_reset(&filter);

    // After reset, state should be restored to initial value
    EXPECT_NEAR(filter.state[0], 5.0f, 0.001f);
    EXPECT_NEAR(filter.state[1], 5.0f, 0.001f);

    // After reset, processing the initial value multiple times should converge to it
    // (The filter still applies its equations, so it won't immediately output the initial value)
    float output = 0.0f;
    for (int i = 0; i < 20; i++)
    {
        output = app_sensor_filter_butterworth_biquad_process(&filter, 5.0f);
    }
    // After many samples of the same value, should converge to that value
    EXPECT_NEAR(output, 5.0f, 0.1f);
}

TEST_F(SensorFilterTest, ButterworthBiquad_DifferentCutoffFrequencies)
{
    ButterworthBiquadFilter fast_filter{}, slow_filter{};

    // Fast filter (higher cutoff frequency, but must be < sample_rate/2)
    app_sensor_filter_butterworth_biquad_init(&fast_filter, 4.5f, 10.0f, 0.0f);

    // Slow filter (lower cutoff frequency)
    app_sensor_filter_butterworth_biquad_init(&slow_filter, 0.5f, 10.0f, 0.0f);

    // Test with step input
    float input = 10.0f;

    float fast_output = app_sensor_filter_butterworth_biquad_process(&fast_filter, input);
    float slow_output = app_sensor_filter_butterworth_biquad_process(&slow_filter, input);

    // Fast filter should be closer to the input (responds faster)
    EXPECT_GT(fast_output, slow_output);
}

TEST_F(SensorFilterTest, ButterworthBiquad_CoefficientCalculation)
{
    ButterworthBiquadFilter filter{};

    // Test coefficient calculation for 1Hz cutoff, 10Hz sample rate
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);

    // Coefficients should be valid (b0, b1, b2, a1, a2)
    EXPECT_GT(filter.coeffs[0], 0.0f); // b0 > 0
    EXPECT_GT(filter.coeffs[1], 0.0f); // b1 > 0
    EXPECT_GT(filter.coeffs[2], 0.0f); // b2 > 0

    // For 2nd order Butterworth, b0 = b2 (symmetry)
    EXPECT_NEAR(filter.coeffs[0], filter.coeffs[2], 0.01f);

    // b1 should be 2*b0 (from the formula)
    EXPECT_NEAR(filter.coeffs[1], 2.0f * filter.coeffs[0], 0.01f);

    // a1 and a2 should be reasonable values
    EXPECT_LT(std::abs(filter.coeffs[3]), 2.0f); // |a1| < 2 for stability
    EXPECT_LT(std::abs(filter.coeffs[4]), 2.0f); // |a2| < 2 for stability
}

TEST_F(SensorFilterTest, ButterworthBiquad_StepResponse)
{
    ButterworthBiquadFilter filter{};

    // Initialize with cutoff frequency 1Hz and sample rate 10Hz
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);

    // Apply step input (constant value)
    float step_value = 10.0f;
    float max_output = 0.0f;
    float min_output_after_start = 1000.0f; // Large initial value

    // Process multiple samples to reach steady state
    for (int i = 0; i < 100; i++)
    {
        float output = app_sensor_filter_butterworth_biquad_process(&filter, step_value);
        
        // Track max and min
        if (output > max_output)
        {
            max_output = output;
        }
        if (i > 5 && output < min_output_after_start)
        {
            min_output_after_start = output;
        }
    }

    // Key properties of a 2nd order Butterworth step response:
    // 1. Should converge to the step value (DC gain = 1.0)
    // 2. Should not significantly overshoot (butterworth filters shoot to upto 5% then decay)
    
    // Check convergence: output should be close to step value
    // Allow 3-10% overshoot
    EXPECT_GT(max_output, step_value * 1.03f); 
    EXPECT_LT(max_output, step_value * 1.10f);
}

TEST_F(SensorFilterTest, ButterworthBiquad_DCGain)
{
    ButterworthBiquadFilter filter{};

    // Initialize with cutoff frequency 1Hz and sample rate 10Hz
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);

    // Apply constant DC input
    float dc_value = 5.0f;

    // Process many samples to reach steady state
    float output = 0.0f;
    for (int i = 0; i < 100; i++)
    {
        output = app_sensor_filter_butterworth_biquad_process(&filter, dc_value);
    }

    // DC gain should be 1.0 (passes DC through)
    EXPECT_NEAR(output, dc_value, 0.1f);
}

TEST_F(SensorFilterTest, ButterworthBiquad_InitialValueHandling)
{
    ButterworthBiquadFilter filter{};

    // Initialize with non-zero initial value
    float initial_value = 7.5f;
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, initial_value);

    // State should be initialized to initial value
    EXPECT_NEAR(filter.state[0], initial_value, 0.001f);
    EXPECT_NEAR(filter.state[1], initial_value, 0.001f);
    EXPECT_NEAR(filter.initial_value, initial_value, 0.001f);

    // First sample should be influenced by initial value
    float output = app_sensor_filter_butterworth_biquad_process(&filter, 10.0f);
    EXPECT_GT(output, initial_value); // Should move towards input
    EXPECT_LT(output, 10.0f);          // But not reach it immediately
}

TEST_F(SensorFilterTest, ButterworthBiquad_MultipleInitializations)
{
    ButterworthBiquadFilter filter{};

    // Initialize with first set of parameters
    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);
    float coeffs1[5];
    for (int i = 0; i < 5; i++)
    {
        coeffs1[i] = filter.coeffs[i];
    }

    // Process some samples
    app_sensor_filter_butterworth_biquad_process(&filter, 10.0f);
    app_sensor_filter_butterworth_biquad_process(&filter, 20.0f);

    // Re-initialize with different parameters
    app_sensor_filter_butterworth_biquad_init(&filter, 2.0f, 10.0f, 0.0f);

    // Coefficients should be different
    bool coeffs_different = false;
    for (int i = 0; i < 5; i++)
    {
        if (std::abs(filter.coeffs[i] - coeffs1[i]) > 0.001f)
        {
            coeffs_different = true;
            break;
        }
    }
    EXPECT_TRUE(coeffs_different);

    // State should be reset to initial value
    EXPECT_NEAR(filter.state[0], 0.0f, 0.001f);
    EXPECT_NEAR(filter.state[1], 0.0f, 0.001f);
}

TEST_F(SensorFilterTest, ButterworthBiquad_FilteringBehavior)
{
    ButterworthBiquadFilter filter{};

    // Initialize with low cutoff frequency (strong filtering)
    float cutoff = 0.5f;
    float sample_rate = 10.0f;
    app_sensor_filter_butterworth_biquad_init(&filter, cutoff, sample_rate, 0.0f);

    // Apply alternating high/low values (simulating high frequency noise)
    float high_value = 10.0f;
    float low_value = 0.0f;

    // Process alternating samples
    float outputs[10];
    for (int i = 0; i < 10; i++)
    {
        float input = (i % 2 == 0) ? high_value : low_value;
        outputs[i] = app_sensor_filter_butterworth_biquad_process(&filter, input);
    }

    // Filtered output should be smoother (less variation)
    // The difference between consecutive outputs should be smaller than the input variation
    float max_output_diff = 0.0f;
    for (int i = 1; i < 10; i++)
    {
        float diff = std::abs(outputs[i] - outputs[i - 1]);
        if (diff > max_output_diff)
        {
            max_output_diff = diff;
        }
    }

    // Output variation should be less than input variation (10.0)
    EXPECT_LT(max_output_diff, high_value - low_value);
}

TEST_F(SensorFilterTest, ButterworthBiquad_NyquistCriterion)
{
    ButterworthBiquadFilter filter{};

    // Test that filter rejects frequencies above Nyquist (sample_rate/2)
    // This is enforced by assertion in init, but we test valid cases

    // Valid: cutoff < sample_rate/2
    app_sensor_filter_butterworth_biquad_init(&filter, 4.0f, 10.0f, 0.0f);
    EXPECT_TRUE(filter.is_initialized);

    // Valid: cutoff close to but less than sample_rate/2
    app_sensor_filter_butterworth_biquad_init(&filter, 4.9f, 10.0f, 0.0f);
    EXPECT_TRUE(filter.is_initialized);
}

TEST_F(SensorFilterTest, ButterworthBiquad_StateConsistency)
{
    ButterworthBiquadFilter filter{};

    app_sensor_filter_butterworth_biquad_init(&filter, 1.0f, 10.0f, 0.0f);

    // Process a sequence of samples
    float inputs[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float outputs[5];

    for (int i = 0; i < 5; i++)
    {
        outputs[i] = app_sensor_filter_butterworth_biquad_process(&filter, inputs[i]);
    }

    // Reset and process same sequence again
    app_sensor_filter_butterworth_biquad_reset(&filter);
    float outputs2[5];

    for (int i = 0; i < 5; i++)
    {
        outputs2[i] = app_sensor_filter_butterworth_biquad_process(&filter, inputs[i]);
    }

    // Outputs should be identical (deterministic behavior)
    for (int i = 0; i < 5; i++)
    {
        EXPECT_NEAR(outputs[i], outputs2[i], 0.001f);
    }
}