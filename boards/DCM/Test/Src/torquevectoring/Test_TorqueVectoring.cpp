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
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "math.h"
#include "App_SharedMacros.h"
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

TEST_F(TorqueVectoringTest, no_BMS_available_power)
{
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(0);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);
    App_TorqueVectoring_Run();
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM   = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // check the IO functions are being called the proper amount of times
}
TEST_F(TorqueVectoringTest, accelerator_pedal_not_pressed)
{
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(0.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(50);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);
    App_TorqueVectoring_Run();
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM   = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // check the IO functions are being called the proper amount of times
}
TEST_F(TorqueVectoringTest, left_motor_too_hot)
{
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(130);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(100);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(50);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);
    App_TorqueVectoring_Run();
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM   = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // check the IO functions are being called the proper amount of times
}
TEST_F(TorqueVectoringTest, right_motor_too_hot)
{
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(100);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(130);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(50);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);
    App_TorqueVectoring_Run();
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM   = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // check the IO functions are being called the proper amount of times
}
TEST_F(TorqueVectoringTest, torque_lessthan_90_nM)
{
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(60);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(60);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(80);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);
    App_TorqueVectoring_Run();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    ASSERT_TRUE(actual_torque_left_nM <= MOTOR_TORQUE_LIMIT_Nm);
    ASSERT_TRUE(actual_torque_right_nM <= MOTOR_TORQUE_LIMIT_Nm);
    ASSERT_TRUE(actual_torque_right_nM > 0);
    ASSERT_TRUE(actual_torque_left_nM > 0);
    // check the IO functions are being called the proper amount of times
}
TEST_F(TorqueVectoringTest, torque_ratio_preserved)
{
    float steering_angle = 30.0;
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(100.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(60);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(60);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(50);
    App_CanRx_FSM_Steering_SteeringAngle_Update(steering_angle);
    App_TorqueVectoring_Run();
    float wheel_angle            = steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE;
    float delta                  = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle)) / (2 * WHEELBASE_mm);
    float cl                     = 1 + delta;
    float cr                     = 1 - delta;
    float expected_torque_ratio  = cl / cr;
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get();
    float actual_torque_ratio    = actual_torque_left_nM / actual_torque_right_nM;
    ASSERT_FLOAT_EQ(expected_torque_ratio, actual_torque_ratio);
    // check the IO functions are being called the proper amount of times
}