//
// Created by formulae on 25/03/23.
//
/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 *
 *
 *
 */

#include "Test_Dcm.h"

extern "C"
{
#include "App_PowerLimiting.h"
#include "App_ActiveDifferential.h"
#include "App_SharedDcmConstants.h"
#include "App_TorqueVectoring.h"
#include "math.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
}
/**
 * Define SetUp and TearDown to be called before every TEST_F call
 */
class TorqueVectoringTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        App_CanTx_Init();
        App_CanRx_Init();
        App_TorqueVectoring_Setup();
    }
    void TearDown() override { return; }
};

// TEST_F(TorqueVectoringTest, torques_are_zero_when_power_limit_is_zero)
// {
//     App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
//     App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
//     App_CanRx_FSM_RightWheelSpeed_Update(50.0);
//     App_CanRx_INVL_MotorSpeed_Update(135);
//     App_CanRx_INVR_MotorSpeed_Update(135);
//     App_CanRx_BMS_TractiveSystemCurrent_Update(100);
//     App_CanRx_BMS_TractiveSystemVoltage_Update(390);
//     App_CanRx_INVL_MotorTemperature_Update(50);
//     App_CanRx_INVR_MotorTemperature_Update(50);
//     App_CanRx_BMS_AvailablePower_Update(0);
//     App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
//     App_TorqueVectoring_Run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
//     float actual_torque_right_nM   = App_CanTx_DCM_RightInverterTorqueCommand_Get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
TEST_F(TorqueVectoringTest, torques_are_zero_when_pedal_is_not_pressed)
{
    App_CanRx_FSM_PappsMappedPedalPercentage_Update(0.0);
    App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystemCurrent_Update(100);
    App_CanRx_BMS_TractiveSystemVoltage_Update(390);
    App_CanRx_INVL_MotorTemperature_Update(50);
    App_CanRx_INVR_MotorTemperature_Update(50);
    App_CanRx_BMS_AvailablePower_Update(50);
    App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
    App_TorqueVectoring_Run();
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM   = App_CanTx_DCM_RightInverterTorqueCommand_Get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
}

// TEST_F(TorqueVectoringTest, torques_are_zero_when_wheels_are_not_moving)
// {
//     App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
//     App_CanRx_FSM_LeftWheelSpeed_Update(0.0);
//     App_CanRx_FSM_RightWheelSpeed_Update(0.0);
//     App_CanRx_INVL_MotorSpeed_Update(135);
//     App_CanRx_INVR_MotorSpeed_Update(135);
//     App_CanRx_BMS_TractiveSystemCurrent_Update(100);
//     App_CanRx_BMS_TractiveSystemVoltage_Update(390);
//     App_CanRx_INVL_MotorTemperature_Update(50);
//     App_CanRx_INVR_MotorTemperature_Update(50);
//     App_CanRx_BMS_AvailablePower_Update(50);
//     App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
//     App_TorqueVectoring_Run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
//     float actual_torque_right_nM   = App_CanTx_DCM_RightInverterTorqueCommand_Get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
// TEST_F(TorqueVectoringTest, check_toruqes_are_zero_when_left_motor_too_hot)
// {
//     App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
//     App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
//     App_CanRx_FSM_RightWheelSpeed_Update(50.0);
//     App_CanRx_INVL_MotorSpeed_Update(135);
//     App_CanRx_INVR_MotorSpeed_Update(135);
//     App_CanRx_BMS_TractiveSystemCurrent_Update(100);
//     App_CanRx_BMS_TractiveSystemVoltage_Update(390);
//     App_CanRx_INVL_MotorTemperature_Update(130);
//     App_CanRx_INVR_MotorTemperature_Update(100);
//     App_CanRx_BMS_AvailablePower_Update(50);
//     App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
//     App_TorqueVectoring_Run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
//     float actual_torque_right_nM   = App_CanTx_DCM_RightInverterTorqueCommand_Get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
// TEST_F(TorqueVectoringTest, check_torques_are_zero_when_right_motor_too_hot)
// {
//     App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
//     App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
//     App_CanRx_FSM_RightWheelSpeed_Update(50.0);
//     App_CanRx_INVL_MotorSpeed_Update(135);
//     App_CanRx_INVR_MotorSpeed_Update(135);
//     App_CanRx_BMS_TractiveSystemCurrent_Update(100);
//     App_CanRx_BMS_TractiveSystemVoltage_Update(390);
//     App_CanRx_INVL_MotorTemperature_Update(100);
//     App_CanRx_INVR_MotorTemperature_Update(130);
//     App_CanRx_BMS_AvailablePower_Update(50);
//     App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
//     App_TorqueVectoring_Run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
//     float actual_torque_right_nM   = App_CanTx_DCM_RightInverterTorqueCommand_Get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
TEST_F(TorqueVectoringTest, check_torques_are_less_than_limit)
{
    App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystemCurrent_Update(100);
    App_CanRx_BMS_TractiveSystemVoltage_Update(390);
    App_CanRx_INVL_MotorTemperature_Update(60);
    App_CanRx_INVR_MotorTemperature_Update(60);
    App_CanRx_BMS_AvailablePower_Update(80);
    App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(30);
    App_TorqueVectoring_Run();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();
    ASSERT_TRUE(actual_torque_left_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM > 0);
    ASSERT_TRUE(actual_torque_left_nM > 0);
}
// TEST_F(TorqueVectoringTest, check_torque_ratio_is_preserved)
// {
//     float steering_angle = 30.0;
//     App_CanRx_FSM_PappsMappedPedalPercentage_Update(100.0);
//     App_CanRx_FSM_LeftWheelSpeed_Update(50.0);
//     App_CanRx_FSM_RightWheelSpeed_Update(50.0);
//     App_CanRx_INVL_MotorSpeed_Update(135);
//     App_CanRx_INVR_MotorSpeed_Update(135);
//     App_CanRx_BMS_TractiveSystemCurrent_Update(100);
//     App_CanRx_BMS_TractiveSystemVoltage_Update(390);
//     App_CanRx_INVL_MotorTemperature_Update(60);
//     App_CanRx_INVR_MotorTemperature_Update(60);
//     App_CanRx_BMS_AvailablePower_Update(50);
//     App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(steering_angle);
//     App_TorqueVectoring_Run();
//     float wheel_angle            = steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE;
//     float delta                  = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle)) / (2 * WHEELBASE_mm);
//     float cl                     = 1 + delta;
//     float cr                     = 1 - delta;
//     float expected_torque_ratio  = cl / cr;
//     float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
//     float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();
//     float actual_torque_ratio    = actual_torque_left_nM / actual_torque_right_nM;
//     ASSERT_FLOAT_EQ(expected_torque_ratio, actual_torque_ratio);
// }