

#include "Test_Dcm.h"

extern "C"
{
#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TractionControl.h"
#include "App_SharedDcmConstants.h"
#include <math.h>
}

class TractionControlTest : public testing::Test
{
  protected:
    void SetUp() override { return; }
    void TearDown() override { return; }
};

TEST(TractionControlTest, TractionControl_init)
{
    PID_Config tc_pid_config = { 0.0, 0.0, 0.0, 0.0, 10.0 };
    PID        tc_pid;
    App_PID_Init(&tc_pid, &tc_pid_config);
    TractionControl_Inputs  test_inputs  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &tc_pid };
    TractionControl_Outputs test_outputs = { 0.0, 0.0 };
    App_TractionControl_ComputeTorque(&test_inputs, &test_outputs);
    ASSERT_NEAR(test_outputs.torque_left_final_Nm, 0.0, 0.000001);
    ASSERT_NEAR(test_outputs.torque_right_final_Nm, 0.0, 0.000001);
}

TEST(TractionControlTest, TractionControl_slip)
{
    float test_slip = App_TractionControl_ComputeSlip(100.0 / PLANETARY_GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.0, 0.000001);
}

TEST(TractionControlTest, backwards_slip)
{
    float test_slip = App_TractionControl_ComputeSlip(90.0 / PLANETARY_GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, -0.1, 0.000001);
}

TEST(TractionControlTest, forwards_slip)
{
    float test_slip = App_TractionControl_ComputeSlip(110.0 / PLANETARY_GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.1, 0.000001);
}

TEST(TractionControlTest, big_forwards_slip)
{
    float test_slip = App_TractionControl_ComputeSlip(100.0 / 2.0 / PLANETARY_GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, -0.5, 0.000001);
}

TEST(TractionControlTest, big_backwards_slip)
{
    float test_slip = App_TractionControl_ComputeSlip(150 / PLANETARY_GEAR_RATIO, 100.0);
    ASSERT_NEAR(test_slip, 0.5, 0.000001);
}

TEST(TractionControlTest, wheel_speed_to_rpm)
{
    float test_rpm = App_TractionControl_WheelSpeedKPHToRPM(100.0);
    ASSERT_NEAR(test_rpm, 100.0 * 1000.0 / (60.0 * ((float)M_PI * TIRE_DIAMETER_in * IN_TO_M)), 0.00001);
}