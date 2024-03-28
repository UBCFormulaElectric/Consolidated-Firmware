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

#include <gtest/gtest.h>

extern "C"
{
#include "app_powerLimiting.h"
#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "math.h"
#include "app_canRx.h"
#include "app_canTx.h"
}

class TorqueVectoringTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        app_canTx_init();
        app_canRx_init();
        app_torqueVectoring_init();
    }
};

// TEST_F(TorqueVectoringTest, torques_are_zero_when_power_limit_is_zero)
// {
//     app_canRx_FSM_PappsMappedPedalPercentage_update(100.0);
//     app_canRx_FSM_LeftWheelSpeed_update(50.0);
//     app_canRx_FSM_RightWheelSpeed_update(50.0);
//     app_canRx_INVL_MotorSpeed_update(135);
//     app_canRx_INVR_MotorSpeed_update(135);
//     app_canRx_BMS_TractiveSystemCurrent_update(100);
//     app_canRx_BMS_TractiveSystemVoltage_update(390);
//     app_canRx_INVL_MotorTemperature_update(50);
//     app_canRx_INVR_MotorTemperature_update(50);
//     app_canRx_BMS_AvailablePower_update(0);
//     app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
//     app_torqueVectoring_run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = app_canTx_VC_LeftInverterTorqueCommand_get();
//     float actual_torque_right_nM   = app_canTx_VC_RightInverterTorqueCommand_get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
TEST_F(TorqueVectoringTest, torques_are_zero_when_pedal_is_not_pressed)
{
    app_canRx_FSM_LeftWheelSpeed_update(50.0);
    app_canRx_FSM_RightWheelSpeed_update(50.0);
    app_canRx_INVL_MotorSpeed_update(135);
    app_canRx_INVR_MotorSpeed_update(135);
    app_canRx_BMS_TractiveSystemCurrent_update(100);
    app_canRx_BMS_TractiveSystemVoltage_update(390);
    app_canRx_INVL_MotorTemperature_update(50);
    app_canRx_INVR_MotorTemperature_update(50);
    app_canRx_BMS_AvailablePower_update(50);
    app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
    app_torqueVectoring_run(0.0f);
    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;
    float actual_torque_left_nM    = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM   = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
}

// TEST_F(TorqueVectoringTest, torques_are_zero_when_wheels_are_not_moving)
// {
//     app_canRx_FSM_PappsMappedPedalPercentage_update(100.0);
//     app_canRx_FSM_LeftWheelSpeed_update(0.0);
//     app_canRx_FSM_RightWheelSpeed_update(0.0);
//     app_canRx_INVL_MotorSpeed_update(135);
//     app_canRx_INVR_MotorSpeed_update(135);
//     app_canRx_BMS_TractiveSystemCurrent_update(100);
//     app_canRx_BMS_TractiveSystemVoltage_update(390);
//     app_canRx_INVL_MotorTemperature_update(50);
//     app_canRx_INVR_MotorTemperature_update(50);
//     app_canRx_BMS_AvailablePower_update(50);
//     app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
//     app_torqueVectoring_run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = app_canTx_VC_LeftInverterTorqueCommand_get();
//     float actual_torque_right_nM   = app_canTx_VC_RightInverterTorqueCommand_get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
// TEST_F(TorqueVectoringTest, check_toruqes_are_zero_when_left_motor_too_hot)
// {
//     app_canRx_FSM_PappsMappedPedalPercentage_update(100.0);
//     app_canRx_FSM_LeftWheelSpeed_update(50.0);
//     app_canRx_FSM_RightWheelSpeed_update(50.0);
//     app_canRx_INVL_MotorSpeed_update(135);
//     app_canRx_INVR_MotorSpeed_update(135);
//     app_canRx_BMS_TractiveSystemCurrent_update(100);
//     app_canRx_BMS_TractiveSystemVoltage_update(390);
//     app_canRx_INVL_MotorTemperature_update(130);
//     app_canRx_INVR_MotorTemperature_update(100);
//     app_canRx_BMS_AvailablePower_update(50);
//     app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
//     app_torqueVectoring_run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = app_canTx_VC_LeftInverterTorqueCommand_get();
//     float actual_torque_right_nM   = app_canTx_VC_RightInverterTorqueCommand_get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
// TEST_F(TorqueVectoringTest, check_torques_are_zero_when_right_motor_too_hot)
// {
//     app_canRx_FSM_PappsMappedPedalPercentage_update(100.0);
//     app_canRx_FSM_LeftWheelSpeed_update(50.0);
//     app_canRx_FSM_RightWheelSpeed_update(50.0);
//     app_canRx_INVL_MotorSpeed_update(135);
//     app_canRx_INVR_MotorSpeed_update(135);
//     app_canRx_BMS_TractiveSystemCurrent_update(100);
//     app_canRx_BMS_TractiveSystemVoltage_update(390);
//     app_canRx_INVL_MotorTemperature_update(100);
//     app_canRx_INVR_MotorTemperature_update(130);
//     app_canRx_BMS_AvailablePower_update(50);
//     app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
//     app_torqueVectoring_run();
//     float expected_torque_left_nM  = 0.0;
//     float expected_torque_right_nM = 0.0;
//     float actual_torque_left_nM    = app_canTx_VC_LeftInverterTorqueCommand_get();
//     float actual_torque_right_nM   = app_canTx_VC_RightInverterTorqueCommand_get();
//     ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
//     ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);
// }
TEST_F(TorqueVectoringTest, check_torques_are_less_than_limit)
{
    app_canRx_FSM_LeftWheelSpeed_update(50.0);
    app_canRx_FSM_RightWheelSpeed_update(50.0);
    app_canRx_INVL_MotorSpeed_update(135);
    app_canRx_INVR_MotorSpeed_update(135);
    app_canRx_BMS_TractiveSystemCurrent_update(100);
    app_canRx_BMS_TractiveSystemVoltage_update(390);
    app_canRx_INVL_MotorTemperature_update(60);
    app_canRx_INVR_MotorTemperature_update(60);
    app_canRx_BMS_AvailablePower_update(80);
    app_canRx_FSM_Warning_SteeringAngleOCSC_update(30);
    app_torqueVectoring_run(100.0f);
    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_TRUE(actual_torque_left_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM > 0);
    ASSERT_TRUE(actual_torque_left_nM > 0);
}
// TEST_F(TorqueVectoringTest, check_torque_ratio_is_preserved)
// {
//     float steering_angle = 30.0;
//     app_canRx_FSM_PappsMappedPedalPercentage_update(100.0);
//     app_canRx_FSM_LeftWheelSpeed_update(50.0);
//     app_canRx_FSM_RightWheelSpeed_update(50.0);
//     app_canRx_INVL_MotorSpeed_update(135);
//     app_canRx_INVR_MotorSpeed_update(135);
//     app_canRx_BMS_TractiveSystemCurrent_update(100);
//     app_canRx_BMS_TractiveSystemVoltage_update(390);
//     app_canRx_INVL_MotorTemperature_update(60);
//     app_canRx_INVR_MotorTemperature_update(60);
//     app_canRx_BMS_AvailablePower_update(50);
//     app_canRx_FSM_Warning_SteeringAngleOCSC_update(steering_angle);
//     app_torqueVectoring_run();
//     float wheel_angle            = steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE;
//     float delta                  = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle)) / (2 * WHEELBASE_mm);
//     float cl                     = 1 + delta;
//     float cr                     = 1 - delta;
//     float expected_torque_ratio  = cl / cr;
//     float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
//     float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
//     float actual_torque_ratio    = actual_torque_left_nM / actual_torque_right_nM;
//     ASSERT_FLOAT_EQ(expected_torque_ratio, actual_torque_ratio);
// }