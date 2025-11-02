#include <gtest/gtest.h>

extern "C"
{
#include "app_pid.h"
#include "app_torqueVectoring.h"
}
#include <cmath>
#include <random>
#include <limits>
class PIDTest : public testing::Test
{
};

TEST(PIDTest, initialization_check)
{
    PID        test_pid;
<<<<<<< HEAD
    PID_Config test_cfg = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, true, true, true, true, 1.0f };
    // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?,
    // clamp_integral?, back_calculation?, feed_forward?, sample_time

    app_pid_init(&test_pid, &test_cfg);
    float setpoint        = 0.0f;
    float input           = 0.0f;
    float disturbance     = 0.0f;
    float expected_output = 0.0f;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input, disturbance);
=======
    PID_Config test_cfg = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, true, true, true, true, 1.0f};
    // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?, clamp_integral?, back_calculation?, feed_forward?, sample_time

    app_pid_init(&test_pid, &test_cfg);
    float setpoint          = 0.0f;
    float input             = 0.0f;
    float disturbance       = 0.0f;
    float expected_output   = 0.0f;
    float actual_output = app_pid_compute(&test_pid, setpoint, input, disturbance);

>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453

    ASSERT_NEAR(test_pid.Kp, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.Ki, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.Kd, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.Kb, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.Kff, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.smoothing_coeff, 1.0f, 0.000001f);

    ASSERT_NEAR(test_pid.out_max, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.out_min, 0.0f, 0.000001f);
    ASSERT_NEAR(test_pid.max_integral, 1.0f, 0.000001f);
    ASSERT_NEAR(test_pid.min_integral, 0.0f, 0.000001f);

    ASSERT_TRUE(test_pid.clamp_output);
    ASSERT_TRUE(test_pid.clamp_integral);
    ASSERT_TRUE(test_pid.back_calculation);
    ASSERT_TRUE(test_pid.feed_forward);
    ASSERT_EQ(test_pid.sample_time, 1.0f);

    ASSERT_NEAR(test_pid.error, 0.0f, 0.000001f);
    ASSERT_NEAR(test_pid.prev_error, 0.0f, 0.000001f);
    ASSERT_NEAR(test_pid.prev_derivative, 0.0f, 0.000001f);
    ASSERT_NEAR(test_pid.prev_disturbance, 0.0f, 0.000001f);
    ASSERT_NEAR(test_pid.integral, 0.0f, 0.000001f);

<<<<<<< HEAD
    ASSERT_NEAR(actual_output, 0.0f, 0.000001f);
=======
    ASSERT_NEAR(actual_output,0.0f,0.000001f);
    
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
}

TEST(PIDTest, zero_error_zero_output)
{
    PID        test_pid;
<<<<<<< HEAD
    PID_Config test_cfg = { 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                            1.0f, 0.0f, false, false, false, false, 100.0f };
=======
    PID_Config test_cfg = {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, false, false, false, false, 100.0f};
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
    app_pid_init(&test_pid, &test_cfg);
    float setpoint        = 1.0f;
    float input           = 1.0f;
    float expected_output = 0.0f;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input, 0.0f);
    ASSERT_NEAR(actual_output, expected_output, 0.000001f);
}

<<<<<<< HEAD
TEST(PIDTest, one_time_step_classic)
{
    PID        test_pid;
    PID_Config test_cfg = { 0.5f, 0.0025f, 25.0f, 0.0f,  0.0f,  1.0f,  1.0,   0.0f,
                            1.0f, 0.0f,    false, false, false, false, 100.0f };
=======

TEST(PIDTest, one_time_step_classic)
{
    PID        test_pid;
    PID_Config test_cfg = {0.5f, 0.0025f, 25.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0f, 1.0f, 0.0f, false, false, false, false, 100.0f};
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
    app_pid_init(&test_pid, &test_cfg);
    float setpoint        = 2.0f;
    float input           = 1.0f;
    float expected_output = 1.0f;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input, 0.0);
    ASSERT_NEAR(actual_output, expected_output, 0.000001f);
}

TEST(PIDTest, clamped_output_stability)
{
    PID        test_pid;
<<<<<<< HEAD
    PID_Config test_cfg = { 1.0f,   0.05f,  0.1f, 0.0f, 0.0f,  0.9f,  10.0f, -10.0f,
                            100.0f, -50.0f, true, true, false, false, 1.0f };

    app_pid_init(&test_pid, &test_cfg);

    float setpoint = 5.0f;
    float input    = 0.0f;

    const int steps = 10000;
    for (int i = 0; i < steps; ++i)
    {
=======
    PID_Config test_cfg = {1.0f, 0.05f, 0.1f, 0.0f, 0.0f, 0.9f, 10.0f, -10.0f, 100.0f, -50.0f, true, true, false, false, 1.0f};
    
    app_pid_init(&test_pid, &test_cfg);

    float setpoint = 5.0f;
    float input = 0.0f;

    const int steps = 10000;
    for (int i = 0; i < steps; ++i) {
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
        float out = app_pid_compute(&test_pid, setpoint, input, 0.0f);
        // output must always be finite
        ASSERT_TRUE(std::isfinite(out));
        // and when clamped must respect configured bounds
        ASSERT_LE(out, test_cfg.out_max + 1e-6f);
        ASSERT_GE(out, test_cfg.out_min - 1e-6f);
        // feed the loop with previous output as measurement to simulate closed-loop
<<<<<<< HEAD
        input += 0.001f * out;
=======
        input += 0.001f * out; 
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
    }
}

TEST(PIDTest, long_run_unclamped_boundedness)
{
<<<<<<< HEAD
    PID test_pid;
    // unclamped output, but integral clamping enabled to keep integrator in check
    PID_Config test_cfg = { 1.0f,   0.01f,   0.1f,  0.0f, 0.0f,  0.8f,  1e6f, -1e6f,
                            100.0f, -100.0f, false, true, false, false, 1.0f };

    app_pid_init(&test_pid, &test_cfg);

    float setpoint = 100.0f;
    float input    = 0.0f;

    const int   steps       = 10000;
    const float max_allowed = 1e6f; // sanity threshold for explosion
    for (int i = 0; i < steps; ++i)
    {
=======
    PID        test_pid;
    // unclamped output, but integral clamping enabled to keep integrator in check
    PID_Config test_cfg = {1.0f, 0.01f, 0.1f, 0.0f, 0.0f, 0.8f, 1e6f, -1e6f, 100.0f, -100.0f, false, true, false, false, 1.0f};
    
    app_pid_init(&test_pid, &test_cfg);

    float setpoint = 100.0f;
    float input = 0.0f;

    const int steps = 10000;
    const float max_allowed = 1e6f; // sanity threshold for explosion
    for (int i = 0; i < steps; ++i) {
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
        float out = app_pid_compute(&test_pid, setpoint, input, 0.0f);
        ASSERT_TRUE(std::isfinite(out));
        ASSERT_LT(std::fabs(out), max_allowed);

        // keep input evolving so controller sees changing error
        input += 0.001f * (out - input);
    }
}

TEST(PIDTest, back_calculation_antiwindup_test)
{
    PID        test_pid;
<<<<<<< HEAD
    PID_Config test_cfg = { 2.0f, 2.0f,  0.0f, 0.5f,  0.0f, 0.9f,  10.0f, -10.0f,
                            1e6f, -1e6f, true, false, true, false, 1.0f };
    // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?,
    // clamp_integral?, back_calculation?, feed_forward?, sample_time
    app_pid_init(&test_pid, &test_cfg);

    float setpoint = 11.0f;
    float input    = 0.0f;

    const int   steps       = 100;
    const float max_allowed = 7.0f;

    // Setpoint is outside of maximum output to simulate saturation
    for (int i = 0; i < steps; i++)
    {
        float out = app_pid_compute(&test_pid, setpoint, input, 0.0f);

        ASSERT_LT(out, test_pid.out_max + 1e-6f);
        ASSERT_LT(test_pid.integral, max_allowed + 1e-6f);
        ASSERT_GT(test_pid.integral, -1 * max_allowed - 1e-6f);
        input += 0.1f * out;
=======
    PID_Config test_cfg = {2.0f, 2.0f, 0.0f, 0.5f, 0.0f, 0.9f, 10.0f, -10.0f, 1e6f, -1e6f, true, false, true, false, 1.0f};
    // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?, clamp_integral?, back_calculation?, feed_forward?, sample_time
    app_pid_init(&test_pid, &test_cfg);
    
    float setpoint = 11.0f;
    float input = 0.0f;

    const int steps = 100;
    const float max_allowed = 7.0f;

    //Setpoint is outside of maximum output to simulate saturation
    for (int i = 0; i < steps; i++) {
        float out = app_pid_compute(&test_pid, setpoint, input, 0.0f);
        
        ASSERT_LT(out, test_pid.out_max + 1e-6f);
        ASSERT_LT(test_pid.integral, max_allowed + 1e-6f);
        ASSERT_GT(test_pid.integral, -1*max_allowed - 1e-6f);
        input += 0.1f *out;
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
    }
}

TEST(PIDTest, random_disturbance_stress_test)
{
    PID        test_pid;
<<<<<<< HEAD
    PID_Config test_cfg = { 0.8f,  0.005f, 0.5f, 0.1f, 0.2f, 0.9f, 50.0f, -50.0f,
                            20.0f, -20.0f, true, true, true, true, 1.0f };
    // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?,
    // clamp_integral?, back_calculation?, feed_forward?, sample_time

    app_pid_init(&test_pid, &test_cfg);

    std::mt19937                          rng(10);
    std::uniform_real_distribution<float> dist(-30.0f, 30.0f);

    float setpoint = 10.0f;
    float input    = 0.0f;

    const int steps = 10000;
    for (int i = 0; i < steps; ++i)
    {
        float disturbance = dist(rng);
        float out         = app_pid_compute(&test_pid, setpoint, input, disturbance);
        ASSERT_TRUE(std::isfinite(out));

        // when integral is clamped it should remain within configured limits
        if (test_pid.clamp_integral)
        {
=======
    PID_Config test_cfg = {0.8f, 0.005f, 0.5f, 0.1f, 0.2f, 0.9f, 50.0f, -50.0f, 20.0f, -20.0f, true, true, true, true, 1.0f};
     // kp, ki, kd, kb, kff, smoothing_coeff, out_max, out_min, max_integral, min_integral, clamp_output?, clamp_integral?, back_calculation?, feed_forward?, sample_time

    app_pid_init(&test_pid, &test_cfg);

    std::mt19937 rng(10);
    std::uniform_real_distribution<float> dist(-30.0f, 30.0f);

    float setpoint = 10.0f;
    float input = 0.0f;

    const int steps = 10000;
    for (int i = 0; i < steps; ++i) {
        float disturbance = dist(rng);
        float out = app_pid_compute(&test_pid, setpoint, input, disturbance);
        ASSERT_TRUE(std::isfinite(out));

        // when integral is clamped it should remain within configured limits
        if (test_pid.clamp_integral) {
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
            ASSERT_LE(test_pid.integral, test_pid.max_integral + 1e-6f);
            ASSERT_GE(test_pid.integral, test_pid.min_integral - 1e-6f);
        }
        // enforce output clamps if configured
<<<<<<< HEAD
        if (test_pid.clamp_output)
        {
=======
        if (test_pid.clamp_output) {
>>>>>>> 0cada2852f26c06c6eeb4ec530006e6711793453
            ASSERT_LE(out, test_pid.out_max + 1e-6f);
            ASSERT_GE(out, test_pid.out_min - 1e-6f);
        }
        // slightly perturb the input towards the output to simulate feedback
        input += 0.001f * out;
    }
}