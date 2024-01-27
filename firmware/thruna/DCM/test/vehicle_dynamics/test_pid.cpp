

#include <gtest/gtest.h>

extern "C"
{
#include "app_pid.h"
#include "app_torqueVectoring.h"
}

class PIDTest : public testing::Test
{
  protected:
    void SetUp() override { return; }
    void TearDown() override { return; }
};

TEST(PIDTest, values_are_as_expected_after_initialization)
{
    PID        test_pid;
    PID_Config test_pid_config = { 1.0, 1.0, 1.0, 0.0, 0.0 };
    app_pid_init(&test_pid, &test_pid_config);
    app_pid_compute(&test_pid, 0.0, 0.0);
    ASSERT_NEAR(test_pid.Kp, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Kp, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Ki, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.Kd, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.error, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.derivative, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.integral, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.prev_input, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.out_min, 0.0, 0.000001);
    ASSERT_NEAR(test_pid.out_max, 0.0, 0.000001);
}

TEST(PIDTest, values_are_as_expected_after_computing_pid_values)
{
    PID        test_pid;
    PID_Config test_pid_config = { 1.0, 1.0, 1.0, 0.0, 10.0 };
    app_pid_init(&test_pid, &test_pid_config);
    float setpoint        = 10.0;
    float input           = 5.0;
    float expected_output = 5.0;
    float actual_output   = app_pid_compute(&test_pid, setpoint, input);
    ASSERT_NEAR(actual_output, expected_output, 0.000001);
}

TEST(PIDTest, values_are_as_expected_after_initialization_with_non_zero_error)
{
    PID        test_pid;
    PID_Config test_pid_config = { 2.0, 3.0, 1.0, -10.0, 10.0 };
    app_pid_init(&test_pid, &test_pid_config);
    app_pid_compute(&test_pid, 2.0, 4.0);
    ASSERT_NEAR(test_pid.Kp, 2.0, 0.000001);
    ASSERT_NEAR(test_pid.Ki, 3.0, 0.000001);
    ASSERT_NEAR(test_pid.Kd, 1.0, 0.000001);
    ASSERT_NEAR(test_pid.derivative, 4.0, 0.000001);
    ASSERT_NEAR(test_pid.integral, -2.0, 0.000001);
    ASSERT_NEAR(test_pid.prev_input, 4.0, 0.000001);
    ASSERT_NEAR(test_pid.out_min, -10.0, 0.000001);
    ASSERT_NEAR(test_pid.out_max, 10.0, 0.000001);
    ASSERT_NEAR(test_pid.error, -2.0, 0.000001);
}