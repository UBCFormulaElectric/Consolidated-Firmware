#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cmath>
#include "app_sensor_filter.h"

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
    IIRMovingAverageFilter filter;
    
    // Initialize with window size 5
    app_sensor_filter_iir_moving_average_init(&filter, 5, 0.0f);
    
    // Test processing samples
    // For window size 5, alpha = 2/(5+1) = 0.333
    // First sample: output = 0.333 * 10 + 0.667 * 0 = 3.33
    float output1 = app_sensor_filter_iir_moving_average_process(&filter, 10.0f);
    EXPECT_NEAR(output1, 3.33f, 0.1f);
    
    // Second sample: output = 0.333 * 20 + 0.667 * 3.33 = 8.89
    float output2 = app_sensor_filter_iir_moving_average_process(&filter, 20.0f);
    EXPECT_NEAR(output2, 8.89f, 0.1f);
    
    // Vfy filter state
    EXPECT_TRUE(filter.is_initialized);
    EXPECT_NEAR(filter.alpha, 0.333f, 0.01f);
}

TEST_F(SensorFilterTest, IIRMovingAverage_Reset)
{
    IIRMovingAverageFilter filter;
    
    app_sensor_filter_iir_moving_average_init(&filter, 5, 10.0f);
    
    // Process some samples
    app_sensor_filter_iir_moving_average_process(&filter, 20.0f);
    app_sensor_filter_iir_moving_average_process(&filter, 30.0f);
    
    // Reset
    app_sensor_filter_iir_moving_average_reset(&filter);
    
    // After reset, should behave like initial state
    float output = app_sensor_filter_iir_moving_average_process(&filter, 5.0f);
    EXPECT_NEAR(output, 6.67f, 0.1f); // 0.333 * 5 + 0.667 * 10
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
    app_sensor_filter_exponential_init(&filter, 1.0f, 10.0f, 0.0f);
    
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
    
    app_sensor_filter_exponential_init(&filter, 1.0f, 10.0f, 5.0f);
    
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
    app_sensor_filter_exponential_init(&fast_filter, 5.0f, 10.0f, 0.0f);
    
    // Slow filter (lower cutoff frequency)
    app_sensor_filter_exponential_init(&slow_filter, 0.5f, 10.0f, 0.0f);
    
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
    app_sensor_filter_exponential_init(&filter, 1.0f, 10.0f, 0.0f);
    float alpha1 = filter.alpha;
    
    app_sensor_filter_exponential_init(&filter, 5.0f, 10.0f, 0.0f);
    float alpha2 = filter.alpha;
    
    // Higher cutoff frequency should result in higher alpha
    EXPECT_GT(alpha2, alpha1);
    EXPECT_GT(alpha1, 0.0f);
    EXPECT_LT(alpha1, 1.0f);
    EXPECT_GT(alpha2, 0.0f);
    EXPECT_LT(alpha2, 1.0f);
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