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
#include <stdio.h>
#include "test_vcBaseStateMachineTest.h"

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

class TorqueVectoringTest : public VcBaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        app_canTx_init();
        app_canRx_init();
        app_torqueVectoring_init();
    }
};

TEST_F(TorqueVectoringTest, stateMachineTest)
{
    TearDown();
    SetUp();
    app_canRx_CRIT_TorqueVecSwitch_update(SWITCH_ON);
    SetStateToDrive();

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    ASSERT_TRUE(app_canTx_VC_TorqueVectoringEnabled_get());

    // at init the TV state is set to TV_OFF
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);

    app_canRx_FSM_LeftWheelSpeed_update(50.0);
    app_canRx_FSM_RightWheelSpeed_update(50.0);
    app_canRx_INVL_MotorSpeed_update(135);
    app_canRx_INVR_MotorSpeed_update(135);
    app_canRx_BMS_TractiveSystemCurrent_update(100);
    app_canRx_BMS_TractiveSystemVoltage_update(390);
    app_canRx_INVL_MotorTemperature_update(50);
    app_canRx_INVR_MotorTemperature_update(50);
    app_canRx_BMS_AvailablePower_update(50);
    app_canRx_FSM_SteeringAngle_update(30);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);

    float expected_torque_left_nM  = 0.0;
    float expected_torque_right_nM = 0.0;

    /******************** TV_OFF -> TRACTION CONTROL OR ACTIVE DIFF VICE VERSA *****************/

    // Torques should be 0 for no pedal input
    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // Interal State should be TV_OFF regardless of steering angle if pedal input is 0
    // 0 = TV_OFF, 1 = Traction Control, 2 = Active Differential
    ASSERT_TRUE(app_canTx_VC_TorqueVectoringEnabled_get());
    LetTimePass(10);

    // when pedal input is 0 regardless of steering angle TV state should be TV_OFF
    ASSERT_TRUE(app_canTx_VC_TorqueVectoringEnabled_get());
    ASSERT_EQ(app_canTx_VC_TVState_get(), 0);
    // ensuring inverters get no torque in test case;
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // with a steering input < 10 (if coming from TV_OFF as no hystersis) we transition to ACTIVE DIFF (2)
    app_canRx_FSM_SteeringAngle_update(30);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.2);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.2);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // while in active diff and we get a pedal input of 0 transition to TV_OFF
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    // ensuring that no torque is going to inverters when in TV_OFF state
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // now testing negative steering angle in above cases (TV_OFF to active diff if angl < -10)
    app_canRx_FSM_SteeringAngle_update(-30);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.2);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.2);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // while in active diff and we get a pedal input of 0 transition to TV_OFF
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    // ensuring that no torque is going to inverters when in TV_OFF state
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // TV_OFF to SLIP edge case (in range but no pedal input) should stay in TV_OFF
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SteeringAngle_update(10.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    // ensuring that no torque is going to inverters when in TV_OFF state
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    // Now with a pedal input we should go to traction control
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.1);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.1);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TRACTION_CONTROL);

    // and back out if pedal input is 0
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    // ensuring that no torque is going to inverters when in TV_OFF state
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_FLOAT_EQ(expected_torque_left_nM, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_torque_right_nM, actual_torque_right_nM);

    /******************** ACTIVE DIFF -> TRACTION CONTROL OR VICE VERSA *****************/
    // start in TRACTION Control .. no hystersis buffer to transition to ACTIVE DIFF
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    app_canRx_FSM_SteeringAngle_update(10.0);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.9);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.9);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TRACTION_CONTROL);
    app_canRx_FSM_SteeringAngle_update(10.1);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // but we should see buffer to go back into traction control (so this should stay in active diff)
    app_canRx_FSM_SteeringAngle_update(9.9);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.9);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.9);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // Hystersis buffer is set to 5 so we should only be able to tranistion back to slip in -5<= steering angle <= 5
    app_canRx_FSM_SteeringAngle_update(5.1);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.82);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.82);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // should transition here:
    app_canRx_FSM_SteeringAngle_update(4.9);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TRACTION_CONTROL);

    // should not be able to go back into active diff until steering angle > 10 or steering angle < -10
    // will test - steering angle case now
    app_canRx_FSM_SteeringAngle_update(-6.5);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TRACTION_CONTROL);

    // should tranistion to ACTIVE DIFF below
    app_canRx_FSM_SteeringAngle_update(-10.1);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // should stay in ACTIVE DIFF below
    app_canRx_FSM_SteeringAngle_update(-90);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);

    // only transition back when <= -5
    app_canRx_FSM_SteeringAngle_update(-5.0);
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.44);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.44);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TRACTION_CONTROL);

    // And back to TV_OFF when apps and papps = 0
    app_canRx_FSM_PappsMappedPedalPercentage_update(0.0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0.0);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
}

TEST_F(TorqueVectoringTest, check_torques_are_less_than_limit)
{

    app_canRx_CRIT_TorqueVecSwitch_update(SWITCH_ON);
    SetStateToDrive();

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    ASSERT_TRUE(app_canTx_VC_TorqueVectoringEnabled_get());

    // at init the TV state is set to TV_OFF
    ASSERT_EQ(app_canTx_VC_TVState_get(), TV_OFF);
    LetTimePass(10);

    app_canRx_FSM_LeftWheelSpeed_update(50.0);
    app_canRx_FSM_RightWheelSpeed_update(50.0);
    app_canRx_INVL_MotorSpeed_update(135);
    app_canRx_INVR_MotorSpeed_update(135);
    app_canRx_BMS_TractiveSystemCurrent_update(100);
    app_canRx_BMS_TractiveSystemVoltage_update(390);
    app_canRx_INVL_MotorTemperature_update(60);
    app_canRx_INVR_MotorTemperature_update(60);
    app_canRx_BMS_AvailablePower_update(80);
    app_canRx_FSM_SteeringAngle_update(30);
    app_canRx_FSM_PappsMappedPedalPercentage_update(100.0f);
    app_canRx_FSM_SappsMappedPedalPercentage_update(100.0f);
    LetTimePass(10);

    ASSERT_EQ(app_canTx_VC_TVState_get(), ACTIVE_DIFF);
    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();
    ASSERT_TRUE(actual_torque_left_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM <= MAX_TORQUE_REQUEST_NM);
    ASSERT_TRUE(actual_torque_right_nM > 0);
    ASSERT_TRUE(actual_torque_left_nM > 0);
}
