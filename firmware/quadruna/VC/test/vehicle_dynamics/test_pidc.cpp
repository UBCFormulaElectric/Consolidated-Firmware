#include <gtest/gtest.h>

extern "C"
{
#include "app_pid.h"
#include "app_torqueVectoring.h"
}

class PIDTest : public testing::Test
{
};

TEST(PIDTest, values_are_as_expected_after_initialization)
{
    PID        test_pid;
    PID_Config test_pid_config = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, true, true, true, true, 100};

    app_pid_init(&test_pid, &test_pid_config);
    float setpoint          = 0.0;
    float input             = 0.0;
    float disturbance       = 0.0;
    float expected_output   = 0.0;
    float actual_output = app_pid_compute(&test_pid, setpoint, input, disturbance);


    ASSERT_NEAR(test_pid.Kp, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Ki, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Kd, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Kb, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Kff, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.smoothing_coeff, 1.0, 0.000001);

    ASSERT_NEAR(test_pid.out_max, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.out_min, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.max_integral, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.min_integral, 0.0, 0.000001);

    ASSERT_TRUE(test_pid.clamp_output);
    ASSERT_TRUE(test_pid.clamp_integral);
    ASSERT_TRUE(test_pid.back_calculation);
    ASSERT_TRUE(test_pid.feed_forward);
    ASSERT_EQ(test_pid.sample_time, 100);

    ASSERT_NEAR(test_pid.error, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.prev_error, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.prev_derivative, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.prev_disturbance, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.integral, 0.0, 0.000001);

    ASSERT_NEAR(actual_output,0.0,0.000001);
    
}

TEST(PIDTest, values_are_as_expected_after_computing_pid_values_classic)
{
    PID        test_pid;
    PID_Config test_pid_config = {1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, false, false, false, false, 100};
    app_pid_init(&test_pid, &test_pid_config);
    float setpoint        = 1.0;
    float input           = 1.0;
    float expected_output = 0.0;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input, 0.0);
    ASSERT_NEAR(actual_output, expected_output, 0.000001);
}


TEST(PIDTest, values_are_as_expected_after_computing_pid_values_classic_non_zero_error)
{
    PID        test_pid;
    PID_Config test_pid_config = {0.5, 0.0025, 25.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, false, false, false, false, 100};
    app_pid_init(&test_pid, &test_pid_config);
    float setpoint        = 2.0;
    float input           = 1.0;
    float expected_output = 1.0;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input,0.0);
    ASSERT_NEAR(actual_output, expected_output, 0.000001);
}

TEST(PIDTest, stability_test_classic)
{
    PID        test_pid;
    PID_Config test_pid_config = {0.5, 0.0025, 25.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, false, false, false, false, 100};
    app_pid_init(&test_pid, &test_pid_config);
    float setpoint        = 2.0;
    float input           = 1.0;
    float expected_output = 1.0; 
    float actual_output   = app_pid_compute(&test_pid, setpoint, input,0.0);
    ASSERT_NEAR(actual_output, expected_output, 0.000001);

}