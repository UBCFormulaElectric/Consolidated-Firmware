#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedDcmConstants.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "App_Regen.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"
#include "App_PowerLimiting.h"
#include "App_VehicleDynamicsControl.h"
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

TEST_F(TestRegen, active_differential_regen)
{
    float steering_angle = 20.0f * APPROX_STEERING_TO_WHEEL_ANGLE;
    float expected_delta = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle)) / (2 * WHEELBASE_mm);
    float cl             = (1 + expected_delta);
    float cr             = (1 - expected_delta);

    static RegenBraking_Inputs       regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs          = { steering_angle, 100, 105, 0.2f };

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * (1 + expected_delta);
    float expected_right_torque_request = torque_lim_Nm * (1 - expected_delta);

    App_ActiveDifferential_ComputeNegativeTorque(&inputs, &regenAttributes);

    ASSERT_FLOAT_EQ(expected_left_torque_request, regenAttributes.left_inverter_torque_Nm);
    ASSERT_FLOAT_EQ(expected_right_torque_request, regenAttributes.right_inverter_torque_Nm);
}

TEST_F(TestRegen, active_differential_exceeds_max)
{
    float steering_angle = 20.0f * APPROX_STEERING_TO_WHEEL_ANGLE;
    float expected_delta = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle)) / (2 * WHEELBASE_mm);
    float cl             = (1 + expected_delta);
    float cr             = (1 - expected_delta);

    static RegenBraking_Inputs       regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs          = { steering_angle, 1000, 1005, 8.0f };

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * cl * MAX_REGEN_Nm / (torque_lim_Nm * cl);
    float expected_right_torque_request = torque_lim_Nm * cr * MAX_REGEN_Nm / (torque_lim_Nm * cl);

    App_ActiveDifferential_ComputeNegativeTorque(&inputs, &regenAttributes);

    ASSERT_FLOAT_EQ(expected_left_torque_request, regenAttributes.left_inverter_torque_Nm);
    ASSERT_FLOAT_EQ(expected_right_torque_request, regenAttributes.right_inverter_torque_Nm);
}

TEST_F(TestRegen, battery_over_temp_torque_request)
{
    float pedal_percentage = -0.3f;

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
    float pedal_percentage = -0.3f;

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
    static RegenBraking_Inputs       regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs;

    float expected_motor_speed_right_rpm = MOTOR_KMH_TO_RPM(20.0f);
    float expected_motor_speed_left_rpm  = MOTOR_KMH_TO_RPM(20.0f);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-expected_motor_speed_right_rpm);
    App_CanRx_INVL_MotorSpeed_Update(expected_motor_speed_left_rpm);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery full
    App_CanRx_BMS_MaxCellVoltage_Update(4.1f);

    bool isSafe = App_Regen_SafetyCheck(&regenAttributes, &inputs);

    ASSERT_TRUE(isSafe == false);
    ASSERT_FLOAT_EQ(inputs.motor_speed_left_rpm, expected_motor_speed_left_rpm);
    ASSERT_FLOAT_EQ(inputs.motor_speed_right_rpm, expected_motor_speed_right_rpm);
}

TEST_F(TestRegen, regular_run_regen)
{
    float pedal_percentage      = -0.5f;
    float steering_angle        = 21.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(30.0f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(30.0f);
    float power_max_kW =
        -pedal_percentage * POWER_LIMIT_REGEN_kW; // power related functions require this to be positive

    static ActiveDifferential_Inputs inputs = { steering_angle, left_motor_speed_rpm, right_motor_speed_rpm,
                                                power_max_kW };

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-right_motor_speed_rpm);
    App_CanRx_INVL_MotorSpeed_Update(left_motor_speed_rpm);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery not full
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_CanRx_INVL_MotorTemperature_Update(30.0f);
    App_CanRx_INVR_MotorTemperature_Update(30.0f);

    // regular active differential
    float delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float cl = (1 + delta);
    float cr = (1 - delta);

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * cl;
    float expected_right_torque_request = torque_lim_Nm * cr;

    App_Run_Regen(pedal_percentage);

    bool  alert                  = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}

// tapers torque request due to being too close to a full battery
TEST_F(TestRegen, taper_torque_request)
{
    float pedal_percentage      = -0.5f;
    float steering_angle        = 21.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(30.0f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(30.0f);
    float power_max_kW          = -pedal_percentage * POWER_LIMIT_REGEN_kW * 0.75; // taper

    static ActiveDifferential_Inputs inputs = { steering_angle, left_motor_speed_rpm, right_motor_speed_rpm,
                                                power_max_kW };

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-right_motor_speed_rpm);
    App_CanRx_INVL_MotorSpeed_Update(left_motor_speed_rpm);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery close to full so start tapering torque request
    App_CanRx_BMS_MaxCellVoltage_Update(3.95f);

    App_CanRx_INVL_MotorTemperature_Update(30.0f);
    App_CanRx_INVR_MotorTemperature_Update(30.0f);

    // regular active differential
    float delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float cl = (1 + delta);
    float cr = (1 - delta);

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * cl;
    float expected_right_torque_request = torque_lim_Nm * cr;

    App_Run_Regen(pedal_percentage);

    bool  alert                  = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}

// tapers torque request due in 5-10kph range, exceed max regen
TEST_F(TestRegen, taper_torque_request_max_regen_exceed)
{
    float pedal_percentage      = -1.0f;
    float steering_angle        = 0.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(9.0f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(9.0f);
    float power_max_kW = -pedal_percentage * (MOTOR_RPM_TO_KMH(right_motor_speed_rpm) - SPEED_MIN_kph) / SPEED_MIN_kph *
                         POWER_LIMIT_REGEN_kW;

    static ActiveDifferential_Inputs inputs = { steering_angle, left_motor_speed_rpm, right_motor_speed_rpm,
                                                power_max_kW };

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-right_motor_speed_rpm);
    App_CanRx_INVL_MotorSpeed_Update(left_motor_speed_rpm);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery in range
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_CanRx_INVL_MotorTemperature_Update(30.0f);
    App_CanRx_INVR_MotorTemperature_Update(30.0f);

    float delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float cl = (1 + delta);
    float cr = (1 - delta);

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * cl * MAX_REGEN_Nm / (torque_lim_Nm * cl);
    float expected_right_torque_request = torque_lim_Nm * cr * MAX_REGEN_Nm / (torque_lim_Nm * cl);

    App_Run_Regen(pedal_percentage);

    bool  alert                  = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}

// tapers torque request due in 5-10kph, in max regen range
TEST_F(TestRegen, taper_torque_request_transition_point)
{
    float pedal_percentage      = -1.0f;
    float steering_angle        = -15.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(5.5f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(5.5f);
    float power_max_kW = -pedal_percentage * (MOTOR_RPM_TO_KMH(right_motor_speed_rpm) - SPEED_MIN_kph) / SPEED_MIN_kph *
                         POWER_LIMIT_REGEN_kW;

    static ActiveDifferential_Inputs inputs = { steering_angle, left_motor_speed_rpm, right_motor_speed_rpm,
                                                power_max_kW };

    // set steering wheel angle
    App_CanRx_FSM_SteeringAngle_Update(steering_angle);

    // make motor speed in range
    App_CanRx_INVR_MotorSpeed_Update(-right_motor_speed_rpm);
    App_CanRx_INVL_MotorSpeed_Update(left_motor_speed_rpm);

    // make battery in temp range
    App_CanRx_BMS_MaxCellTemperature_Update(40);

    // make battery in range
    App_CanRx_BMS_MaxCellVoltage_Update(3.8f);

    App_CanRx_INVL_MotorTemperature_Update(30.0f);
    App_CanRx_INVR_MotorTemperature_Update(30.0f);

    float delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float cl = (1 + delta);
    float cr = (1 - delta);

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float expected_left_torque_request  = torque_lim_Nm * cl;
    float expected_right_torque_request = torque_lim_Nm * cr;

    App_Run_Regen(pedal_percentage);

    bool  alert                  = App_CanAlerts_DCM_Warning_RegenNotAvailable_Get();
    float actual_torque_left_nM  = App_CanTx_DCM_LeftInverterTorqueCommand_Get();
    float actual_torque_right_nM = App_CanTx_DCM_RightInverterTorqueCommand_Get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}
