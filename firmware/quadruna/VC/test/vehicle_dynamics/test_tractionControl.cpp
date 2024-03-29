

#include <gtest/gtest.h>

extern "C"
{
#include "app_units.h"
#include "app_torqueVectoring.h"
#include "app_tractionControl.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include <math.h>
}

class TractionControlTest : public testing::Test
{
};

TEST(TractionControlTest, TractionControl_init)
{
    PID_Config tc_pid_config = { 0.0, 0.0, 0.0, 0.0, 10.0 };
    PID        tc_pid;
    app_pid_init(&tc_pid, &tc_pid_config);
    TractionControl_Inputs  test_inputs  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &tc_pid };
    TractionControl_Outputs test_outputs = { 0.0, 0.0 };
    app_tractionControl_computeTorque(&test_inputs, &test_outputs);
    ASSERT_NEAR(test_outputs.torque_left_final_Nm, 0.0, 0.000001);
    ASSERT_NEAR(test_outputs.torque_right_final_Nm, 0.0, 0.000001);
}

TEST(TractionControlTest, TractionControl_slip)
{
    float test_slip = app_tractionControl_computeSlip(100.0 * GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.0, 0.000001);
}

TEST(TractionControlTest, backwards_slip)
{
    float test_slip = app_tractionControl_computeSlip(90.0 * GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, -0.1, 0.000001);
}

TEST(TractionControlTest, forwards_slip)
{
    float test_slip = app_tractionControl_computeSlip(110.0 * GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.1, 0.000001);
}

TEST(TractionControlTest, big_forwards_slip)
{
    float test_slip = app_tractionControl_computeSlip(100.0 / 2.0 * GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, -0.5, 0.000001);
}

TEST(TractionControlTest, big_backwards_slip)
{
    float test_slip = app_tractionControl_computeSlip(150 * GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.5, 0.000001);
}

TEST(TractionControlTest, wheel_speed_to_rpm)
{
    float test_rpm = WHEEL_KMH_TO_RPM(100.0);
    ASSERT_NEAR(test_rpm, (int)(100.0 * 1000.0 / (60.0 * ((float)M_PI * WHEEL_DIAMETER_IN * IN_TO_M))), 0.00001);
}