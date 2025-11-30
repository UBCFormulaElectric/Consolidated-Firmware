#include <gtest/gtest.h>
#include "app_pid.hpp"
#include <random>
#include <cmath>

// Put tests in same namespace for readability
using app::PID;

class PIDFixture : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cfg.Kp               = 1.0f;
        cfg.Ki               = 0.0f;
        cfg.Kd               = 0.0f;
        cfg.Kb               = 0.0f;
        cfg.Kff              = 0.0f;
        cfg.smoothing_coeff  = 0.8f;
        cfg.out_max          = 1e6f;
        cfg.out_min          = -1e6f;
        cfg.max_integral     = 100.0f;
        cfg.min_integral     = -100.0f;
        cfg.clamp_output     = false;
        cfg.clamp_integral   = false;
        cfg.back_calculation = false;
        cfg.feed_forward     = false;
        cfg.sample_time      = 1; // discrete step unit
    }
    app::PID::Config cfg;
};

TEST_F(PIDFixture, zero_error_zero_output)
{
    PID pid(cfg);

    float setpoint = 0.0f;
    float input    = 0.0f;
    float out      = pid.compute(setpoint, input, 0.0f);

    EXPECT_NEAR(out, 0.0f, 1e-6f);
}

TEST_F(PIDFixture, one_time_step_classic)
{
    cfg.Kp              = 0.5f;
    cfg.Ki              = 0.1f;
    cfg.Kd              = 0.1f;
    cfg.smoothing_coeff = 1.0f;
    PID pid(cfg);

    float setpoint = 3.0f;
    float input    = 1.0f;
    float out      = pid.compute(setpoint, input, 0.0f);
    EXPECT_NEAR(out, 1.4, 1e-6f);
}

TEST_F(PIDFixture, clamped_output_stability)
{
    cfg.Kp             = 0.25f;
    cfg.Ki             = 0.5f;
    cfg.Kd             = 0.25f;
    cfg.out_max        = 100.0f;
    cfg.out_min        = -100.0f;
    cfg.clamp_integral = true;
    cfg.clamp_output   = true;

    PID pid(cfg);

    float setpoint = 5.0f;
    float input    = 0.0f;

    const int steps = 10000;
    for (int i = 0; i < steps; ++i)
    {
        float out = pid.compute(setpoint, input, 0.0f);

        // output must always be finite
        ASSERT_TRUE(std::isfinite(out));
        // and when clamped must respect configured bounds
        ASSERT_LE(out, cfg.out_max + 1e-6f);
        ASSERT_GE(out, cfg.out_min - 1e-6f);
        // Simulate closed loop response
        input = input + 0.001f * out;
    }
}

TEST_F(PIDFixture, clamped_integral_test)
{
    cfg.Ki             = 0.5f;
    cfg.clamp_integral = true;
    cfg.max_integral   = 100.0f;
    cfg.min_integral   = -50.0f;
    PID pid(cfg);

    float setpoint = 5.0f;
    float input    = 0.0f;

    for (int i = 0; i < 10000; ++i)
    {
        float out = pid.compute(setpoint, input, 0.0f);
        ASSERT_TRUE(std::isfinite(out));
        ASSERT_LE(pid.getIntegral(), cfg.max_integral + 1e-6f);
        ASSERT_GE(pid.getIntegral(), cfg.min_integral - 1e-6f);
        input += 0.001f * out;
    }
}

TEST_F(PIDFixture, back_calculation_antiwindup_test)
{
    cfg.Kp               = 2.0f;
    cfg.Ki               = 2.0f;
    cfg.Kd               = 0.0f;
    cfg.Kb               = 0.5f;
    cfg.smoothing_coeff  = 0.0;
    cfg.out_max          = 10.0f;
    cfg.out_min          = -10.0f;
    cfg.max_integral     = 14.0f;
    cfg.min_integral     = -14.0f;
    cfg.clamp_output     = true;
    cfg.clamp_integral   = false;
    cfg.back_calculation = true;

    PID pid(cfg);
    // Set point outside of max output simulating saturation
    float setpoint = 11.0f;
    float input    = 0.0f;

    for (int i = 0; i < 100; ++i)
    {
        float out = pid.compute(setpoint, input, 0.0f);
        ASSERT_TRUE(std::isfinite(out));
        ASSERT_LE(pid.getIntegral(), cfg.max_integral + 1e-6f);
        ASSERT_GE(pid.getIntegral(), cfg.min_integral - 1e-6f);
        input += 0.1f * out;
    }
}

TEST_F(PIDFixture, random_disturbance_stress_test)
{
    cfg.Kp              = 0.8f;
    cfg.Ki              = 0.005f;
    cfg.Kd              = 0.5f;
    cfg.Kb              = 0.1f;
    cfg.Kff             = 0.2f;
    cfg.smoothing_coeff = 0.9f;
    cfg.clamp_output    = true;
    cfg.out_max         = 50.0f;
    cfg.out_min         = -50.0f;
    cfg.clamp_integral  = true;
    cfg.max_integral    = 20.0f;
    cfg.min_integral    = -20.0f;
    cfg.feed_forward    = true;
    cfg.sample_time     = 1;

    PID pid(cfg);

    std::mt19937                          rng(10);
    std::uniform_real_distribution<float> dist(-30.0f, 30.0f);

    float setpoint = 10.0f;
    float input    = 0.0f;

    for (int i = 0; i < 10000; ++i)
    {
        float disturbance = dist(rng);
        float out         = pid.compute(setpoint, input, disturbance);
        EXPECT_TRUE(std::isfinite(out));

        // If output clamping is active, output must lie within bounds
        if (cfg.clamp_output)
        {
            ASSERT_LE(out, cfg.out_max + 1e-6f);
            ASSERT_GE(out, cfg.out_min - 1e-6f);
        }

        if (cfg.clamp_integral)
        {
            ASSERT_LE(pid.getIntegral(), cfg.max_integral + 1e-6f);
            ASSERT_GE(pid.getIntegral(), cfg.min_integral + 1e-6f);
        }

        // small closed-loop update
        input += 0.001f * out;
    }
}