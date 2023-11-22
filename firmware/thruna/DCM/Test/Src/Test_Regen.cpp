#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "App_Regen.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"
#include "math.h"
#include <assert.h>
}

class TestRegen : public testing::Test
{
  protected:
    void SetUp() override
    {
        App_CanTx_Init();
        App_CanRx_Init();
    }
    void TearDown() override { return; }
};

TEST_F(TestRegen, active_differential)
{
    static RegenBraking              regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs;

    float pedal_percentage    = -30.0f;
    float torque_request      = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION;
    float steering_angle      = 20.0f;
    inputs.steering_angle_deg = steering_angle;

    float expected_delta                = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle)) / (2 * WHEELBASE_mm);
    float expected_left_torque_request  = torque_request * (1 + expected_delta);
    float expected_right_torque_request = torque_request * (1 - expected_delta);

    App_ActiveDifferential_ComputeNegativeTorque(&inputs, &regenAttributes, torque_request);

    ASSERT_FLOAT_EQ(expected_left_torque_request, regenAttributes.left_inverter_torque_Nm);
    ASSERT_FLOAT_EQ(expected_right_torque_request, regenAttributes.right_inverter_torque_Nm);
}

TEST_F(TestRegen, battery_over_temp_torque_request)
{
    static float prev_torque_request = 10.0f;
    float        pedal_percentage    = -30.0f;

    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // make battery over temp > 45 degrees
    App_CanRx_BMS_MaxCellTemperature_Update(47);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-MOTOR_KMH_TO_RPM(15.0f));
    App_CanRx_INVL_MotorSpeed_Update(MOTOR_KMH_TO_RPM(15.0f));

    // make battery not full
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_Run_Regen(pedal_percentage);

    float left_torque_request  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float right_torque_request = App_CanTx_DCM_RightInverterTorqueCommand_Get();
    bool  alert                = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();

    ASSERT_FLOAT_EQ(0.0, left_torque_request);
    ASSERT_FLOAT_EQ(0.0, right_torque_request);
    ASSERT_TRUE(alert == true);
}

// same test as battery_over_temp_torque_request just different safety failure
TEST_F(TestRegen, vehicle_under_speed_torque_request)
{
    static float prev_torque_request = 10.0f;
    float        pedal_percentage    = -30.0f;

    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // make motor speed < 5.0kmh
    App_CanRx_INVR_MotorSpeed_Update(MOTOR_KMH_TO_RPM(4.0f));
    App_CanRx_INVL_MotorSpeed_Update(MOTOR_KMH_TO_RPM(4.0f));

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery not full
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_Run_Regen(pedal_percentage);

    // make sure RegenNotAvailable alert occurs
    bool alert = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();

    ASSERT_FLOAT_EQ(0.0, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    ASSERT_FLOAT_EQ(0.0, App_CanTx_DCM_RightInverterTorqueCommand_Get());
    ASSERT_TRUE(alert == true);
}

TEST_F(TestRegen, battery_full_torque_request)
{
    static RegenBraking regenAttributes = { .enable_active_differential = true };

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(MOTOR_KMH_TO_RPM(20.0f));
    App_CanRx_INVL_MotorSpeed_Update(MOTOR_KMH_TO_RPM(20.0f));

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery full
    App_CanRx_BMS_MaxCellVoltage_Update(4.1f);

    bool isSafe = App_Regen_SafetyCheck(&regenAttributes);

    ASSERT_TRUE(isSafe == false);
}

TEST_F(TestRegen, regular_active_differential_torque_request)
{
    static float prev_torque_request = -10.0f;
    float        pedal_percentage    = -50.0f;
    float        steering_angle      = 21.0f;

    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-MOTOR_KMH_TO_RPM(30.0f));
    App_CanRx_INVL_MotorSpeed_Update(MOTOR_KMH_TO_RPM(30.0f));

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery not full
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_Run_Regen(pedal_percentage);

    // make sure RegenNotAvailable alert occurs
    bool  alert = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float expected_delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float expected_torque_request = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION;
    float expected_left_torque    = expected_torque_request * (1 + expected_delta);
    float expected_right_torque   = expected_torque_request * (1 - expected_delta);

    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque, actual_torque_right_nM);
}

// tapers torque request due to being too close to a full battery
TEST_F(TestRegen, taper_torque_request)
{
    static float prev_torque_request = -10.0f;
    float        pedal_percentage    = -50.0f;
    float        steering_angle      = 21.0f;

    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-MOTOR_KMH_TO_RPM(30.0f));
    App_CanRx_INVL_MotorSpeed_Update(MOTOR_KMH_TO_RPM(30.0f));

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery clost to full so start tapering torque request
    App_CanRx_BMS_MaxCellVoltage_Update(3.95f);

    App_Run_Regen(pedal_percentage);

    // make sure RegenNotAvailable alert occurs
    bool  alert          = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float torque_request = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION * 0.75f;
    float expected_delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float expected_left_torque_request  = torque_request * (1 + expected_delta);
    float expected_right_torque_request = torque_request * (1 - expected_delta);

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    ASSERT_FLOAT_EQ(expected_right_torque_request, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}

// max as in max negative torque = -50.0f
TEST_F(TestRegen, active_differential_exceeds_max)
{
    static RegenBraking              regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs;

    float pedal_percentage    = -100.0f;
    float torque_request      = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION;
    float steering_angle      = -43.0f; // doesn't need to be multiplied by APPROX_STEERING_TO_WHEEL_ANGLE since handled
    inputs.steering_angle_deg = steering_angle;

    float expected_delta                = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle)) / (2 * WHEELBASE_mm);
    float expected_left_torque_request  = torque_request * (1 + expected_delta);
    float expected_right_torque_request = torque_request * (1 - expected_delta);
    float torque_negative_max_Nm        = fminf(expected_left_torque_request, expected_right_torque_request);

    float scale = MAX_REGEN_nm / torque_negative_max_Nm;

    App_ActiveDifferential_ComputeNegativeTorque(&inputs, &regenAttributes, torque_request);

    ASSERT_FLOAT_EQ(expected_left_torque_request * scale, regenAttributes.left_inverter_torque_Nm);
    ASSERT_FLOAT_EQ(expected_right_torque_request * scale, regenAttributes.right_inverter_torque_Nm);
}

// tapers torque request due in 5-10kph range from -30.0f towards 0.0f
TEST_F(TestRegen, taper_torque_request_transition_point)
{
    static float prev_torque_request = -30.0f;
    float        pedal_percentage    = -100.0f;
    float        steering_angle      = 21.0f;
    float right_speed = MOTOR_KMH_TO_RPM(-9.0f);
    float left_speed = MOTOR_KMH_TO_RPM(9.0f);


    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(right_speed);
    App_CanRx_INVL_MotorSpeed_Update(left_speed);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery clost to full so start tapering torque request
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_Run_Regen(pedal_percentage);

    // make sure RegenNotAvailable alert occurs
    bool  alert          = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float torque_request = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION; 
    torque_request = (MOTOR_RPM_TO_KMH(left_speed) - SPEED_MIN_kph) / (SPEED_MIN_kph) * torque_request; 
    float expected_delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float expected_left_torque_request  = torque_request * (1 + expected_delta);
    float expected_right_torque_request = torque_request * (1 - expected_delta);

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    ASSERT_FLOAT_EQ(expected_right_torque_request, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}

// tapers torque request due in 5-10kph range from 0.0f towards -30.0f towards
TEST_F(TestRegen, limit_change_in_torque)
{
    static float prev_torque_request = 0.0f;
    float        pedal_percentage    = -100.0f;
    float steering_angle      = -15.0f;
    float right_speed = MOTOR_KMH_TO_RPM(-6.1f);
    float left_speed = MOTOR_KMH_TO_RPM(6.1f);

    // set torque to prev_torque_request first
    App_CanTx_DCM_RightInverterTorqueCommand_Set(prev_torque_request);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(prev_torque_request);

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(right_speed);
    App_CanRx_INVL_MotorSpeed_Update(left_speed);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery not full
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_Run_Regen(pedal_percentage);

    // make sure RegenNotAvailable alert occurs
    bool  alert          = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float torque_request = MAX_REGEN_nm * pedal_percentage / MAX_PEDAL_POSITION; 
    torque_request = (MOTOR_RPM_TO_KMH(left_speed) - SPEED_MIN_kph) / (SPEED_MIN_kph) * torque_request; 
    float expected_delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float expected_left_torque_request  = torque_request * (1 + expected_delta);
    float expected_right_torque_request = torque_request * (1 - expected_delta);

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    ASSERT_FLOAT_EQ(expected_right_torque_request, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}
