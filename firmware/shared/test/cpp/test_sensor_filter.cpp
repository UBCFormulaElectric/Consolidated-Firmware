#include <gtest/gtest.h>
#include <cmath>

#include "app_sensor_filter.hpp"

using namespace app;

TEST(ExponentialFilterTest, alpha_direct_construction)
{
    ExponentialFilter filter(AlphaParameterType::ALPHA_DIRECT, 0.2f, 0.0f, 0.0f);
    float             out = filter.process(1.0f);
    ASSERT_NEAR(out, 0.2f, 1e-5f);
}

TEST(ExponentialFilterTest, alpha_from_cutoff_construction)
{
    ExponentialFilter filter(AlphaParameterType::ALPHA_FROM_CUTOFF_FREQUENCY, 10.0f, 100.0f, 0.0f);
    float             out = filter.process(1.0f);
    EXPECT_GT(out, 0.0f);
    EXPECT_LE(out, 1.0f);
}

TEST(ExponentialFilterTest, steady_state_tracks_constant_input)
{
    ExponentialFilter filter(AlphaParameterType::ALPHA_DIRECT, 0.3f, 0.0f, 0.0f);
    for (int i = 0; i < 50; ++i)
    {
        (void)filter.process(5.0f);
    }
    ASSERT_NEAR(filter.process(5.0f), 5.0f, 1e-4f);
}

TEST(ExponentialFilterTest, reset_restores_to_previous_input)
{
    ExponentialFilter filter(AlphaParameterType::ALPHA_DIRECT, 0.5f, 0.0f, 0.0f);
    (void)filter.process(1.0f);
    (void)filter.process(2.0f);
    filter.reset();
    float out = filter.process(2.0f);
    ASSERT_NEAR(out, 2.0f, 1e-4f);
}

TEST(ButterworthFilterTest, construction_and_process)
{
    ButterworthFilter filter(10.0f, 100.0f, 0.0f);
    float             out = filter.process(1.0f);
    EXPECT_GT(out, 0.0f);
    EXPECT_LE(out, 1.0f);
}

TEST(ButterworthFilterTest, steady_state_dc_gain_one)
{
    ButterworthFilter filter(10.0f, 100.0f, 0.0f);
    for (int i = 0; i < 100; ++i)
    {
        (void)filter.process(3.0f);
    }
    ASSERT_NEAR(filter.process(3.0f), 3.0f, 1e-4f);
}

TEST(ButterworthFilterTest, reset)
{
    ButterworthFilter filter(10.0f, 100.0f, 0.0f);
    (void)filter.process(1.0f);
    (void)filter.process(2.0f);
    filter.reset();
    ASSERT_NEAR(filter.process(2.0f), 2.0f, 1e-3f);
}

TEST(ButterworthBiquadFilterTest, construction_and_process)
{
    ButterworthBiquadFilter filter(10.0f, 100.0f, 0.0f);
    float                   out = filter.process(1.0f);
    EXPECT_GT(out, 0.0f);
    EXPECT_LE(out, 1.0f);
}

TEST(ButterworthBiquadFilterTest, steady_state_dc_gain_one)
{
    ButterworthBiquadFilter filter(10.0f, 100.0f, 0.0f);
    for (int i = 0; i < 100; ++i)
    {
        (void)filter.process(4.0f);
    }
    ASSERT_NEAR(filter.process(4.0f), 4.0f, 1e-4f);
}

TEST(ButterworthBiquadFilterTest, reset)
{
    ButterworthBiquadFilter filter(10.0f, 100.0f, 0.0f);
    (void)filter.process(1.0f);
    (void)filter.process(2.0f);
    filter.reset();
    ASSERT_NEAR(filter.process(2.0f), 2.0f, 1e-3f);
}
