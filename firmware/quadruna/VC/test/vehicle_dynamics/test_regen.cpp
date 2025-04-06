#include <gtest/gtest.h>
#include <assert.h>

#include "test_vcBaseStateMachineTest.h"

extern "C"
{
#include "app_units.h"
#include "app_regen.h"
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_vehicleDynamics.h"
}

class TestRegen : public VCBaseTest
{
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

    app_regen_computeActiveDifferentialTorque(&inputs, &regenAttributes);

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

    float expected_left_torque_request  = torque_lim_Nm * cl;
    float expected_right_torque_request = torque_lim_Nm * cr;

    float torque_negative_max = fminf(expected_left_torque_request, expected_right_torque_request);

    expected_left_torque_request *= MAX_REGEN_Nm / torque_negative_max;
    expected_right_torque_request *= MAX_REGEN_Nm / torque_negative_max;

    app_regen_computeActiveDifferentialTorque(&inputs, &regenAttributes);

    ASSERT_FLOAT_EQ(expected_left_torque_request, regenAttributes.left_inverter_torque_Nm);
    ASSERT_FLOAT_EQ(expected_right_torque_request, regenAttributes.right_inverter_torque_Nm);
}

TEST_F(TestRegen, battery_over_temp_torque_request)
{
    TearDown();
    SetUp();

    app_canRx_CRIT_RegenSwitch_update(SWITCH_ON);
    SetStateToDrive();
    LetTimePass(1);

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    ASSERT_FALSE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_TRUE(app_canTx_VC_RegenEnabled_get());

    float pedal_percentage = 0.0f;
    app_canRx_FSM_PappsMappedPedalPercentage_update(pedal_percentage);
    app_canRx_FSM_SappsMappedPedalPercentage_update(pedal_percentage);
    app_canRx_FSM_SteeringAngle_update(0.0f);

    EXPECT_EQ(SWITCH_ON, app_canRx_CRIT_RegenSwitch_get());

    // make battery over temp > 45 degrees
    app_canRx_BMS_MaxCellTemperature_update(47);

    // make motor speed in range
    app_canRx_INVR_MotorSpeed_update(-MOTOR_KMH_TO_RPM(15.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(15.0f));

    // make battery not full
    app_canRx_BMS_MaxCellVoltage_update(3.8f);

    LetTimePass(10);

    float left_torque_request  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float right_torque_request = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_FLOAT_EQ(0.0, left_torque_request);
    ASSERT_FLOAT_EQ(0.0, right_torque_request);
    ASSERT_TRUE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_FALSE(app_canTx_VC_RegenEnabled_get());
}

// same test as battery_over_temp_torque_request just different safety failure
TEST_F(TestRegen, vehicle_under_speed_torque_request)
{
    TearDown();
    SetUp();

    app_canRx_CRIT_RegenSwitch_update(SWITCH_ON);
    SetStateToDrive();
    LetTimePass(1);
    ASSERT_TRUE(app_canTx_VC_RegenEnabled_get());
    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    ASSERT_FALSE(app_canAlerts_VC_Warning_RegenNotAvailable_get());

    float pedal_percentage = 0.0f;
    app_canRx_FSM_PappsMappedPedalPercentage_update(pedal_percentage);
    app_canRx_FSM_SappsMappedPedalPercentage_update(pedal_percentage);
    app_canRx_FSM_SteeringAngle_update(0.0f);

    // make motor speed < 5.0kmh
    app_canRx_INVR_MotorSpeed_update(MOTOR_KMH_TO_RPM(4.0f));
    app_canRx_INVL_MotorSpeed_update(MOTOR_KMH_TO_RPM(4.0f));

    // make battery in temp range
    app_canRx_BMS_MaxCellTemperature_update(40);

    // make battery not full
    app_canRx_BMS_MaxCellVoltage_update(3.8f);

    LetTimePass(10);

    // make sure RegenNotAvailable alert occurs
    ASSERT_FLOAT_EQ(0.0, app_canTx_VC_LeftInverterTorqueCommand_get());
    ASSERT_FLOAT_EQ(0.0, app_canTx_VC_RightInverterTorqueCommand_get());
    ASSERT_TRUE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_FALSE(app_canTx_VC_RegenEnabled_get());
}

TEST_F(TestRegen, battery_full_torque_request)
{
    static RegenBraking_Inputs       regenAttributes = { .enable_active_differential = true };
    static ActiveDifferential_Inputs inputs;

    float expected_motor_speed_right_rpm = MOTOR_KMH_TO_RPM(20.0f);
    float expected_motor_speed_left_rpm  = MOTOR_KMH_TO_RPM(20.0f);

    // make motor speed in range
    app_canRx_INVR_MotorSpeed_update(-expected_motor_speed_right_rpm);
    app_canRx_INVL_MotorSpeed_update(expected_motor_speed_left_rpm);

    // make battery in temp range
    app_canRx_BMS_MaxCellTemperature_update(40);

    // make battery full
    app_canRx_BMS_MaxCellVoltage_update(4.1f);

    bool is_safe = app_regen_safetyCheck(&regenAttributes, &inputs);

    ASSERT_TRUE(is_safe == false);
    ASSERT_FLOAT_EQ(inputs.motor_speed_left_rpm, expected_motor_speed_left_rpm);
    ASSERT_FLOAT_EQ(inputs.motor_speed_right_rpm, expected_motor_speed_right_rpm);
}

TEST_F(TestRegen, regular_run_regen_and_switch_disable_during_drive_state)
{
    TearDown();
    SetUp();

    app_canRx_CRIT_RegenSwitch_update(SWITCH_ON);
    SetStateToDrive();
    LetTimePass(1);

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    ASSERT_FALSE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_TRUE(app_canTx_VC_RegenEnabled_get());

    float pedal_percentage      = -1.0f;
    float steering_angle        = 21.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(30.0f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(30.0f);
    float power_max_kW =
        -pedal_percentage * POWER_LIMIT_REGEN_kW; // power related functions require this to be positive

    static ActiveDifferential_Inputs inputs = { steering_angle, left_motor_speed_rpm, right_motor_speed_rpm,
                                                power_max_kW };

    app_canRx_FSM_PappsMappedPedalPercentage_update(0);
    app_canRx_FSM_SappsMappedPedalPercentage_update(0);

    // set steering wheel angle
    app_canRx_FSM_SteeringAngle_update(steering_angle);

    // make motor speed in range
    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    // make battery in temp range
    app_canRx_BMS_MaxCellTemperature_update(40);

    // make battery not full
    app_canRx_BMS_MaxCellVoltage_update(3.8f);

    app_canRx_INVL_MotorTemperature_update(30.0f);
    app_canRx_INVR_MotorTemperature_update(30.0f);

    // regular active differential
    float delta =
        TRACK_WIDTH_mm * tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
    float cl = (1 + delta);
    float cr = (1 - delta);

    float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * inputs.power_max_kW) /
                          (inputs.motor_speed_left_rpm * cl + inputs.motor_speed_right_rpm * cr + SMALL_EPSILON);

    float torque_left_Nm  = torque_lim_Nm * cl;
    float torque_right_Nm = torque_lim_Nm * cr;

    float torque_negative_max_Nm = fminf(torque_left_Nm, torque_right_Nm);

    float scale = 1;
    if (torque_negative_max_Nm < MAX_REGEN_Nm)
    {
        scale *= MAX_REGEN_Nm / torque_negative_max_Nm;
    }

    float expected_left_torque_request  = torque_left_Nm * scale;
    float expected_right_torque_request = torque_right_Nm * scale;

    LetTimePass(10);

    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);

    ASSERT_FALSE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_TRUE(app_canTx_VC_RegenEnabled_get());
    ASSERT_TRUE(app_canTx_VC_MappedPedalPercentage_get() == -100.0f);

    app_canRx_CRIT_RegenSwitch_update(SWITCH_OFF);

    LetTimePass(10);

    ASSERT_TRUE(app_canAlerts_VC_Warning_RegenNotAvailable_get());
    ASSERT_FALSE(app_canTx_VC_RegenEnabled_get());
    ASSERT_TRUE(app_canTx_VC_MappedPedalPercentage_get() == 0.0f);
}

// tapers torque request due to being too close to a full battery
TEST_F(TestRegen, taper_torque_request)
{
    float pedal_percentage      = -0.5f;
    float steering_angle        = 21.0f;
    float right_motor_speed_rpm = MOTOR_KMH_TO_RPM(30.0f);
    float left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(30.0f);
    float power_max_kW          = -pedal_percentage * POWER_LIMIT_REGEN_kW;

    static ActiveDifferential_Inputs inputs = { steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE, left_motor_speed_rpm,
                                                right_motor_speed_rpm, power_max_kW, pedal_percentage };

    static RegenBraking_Inputs regen_braking_inputs = { .enable_active_differential = true,
                                                        .derating_value             = SOC_LIMIT_DERATING_VALUE };

    // set steering wheel angle
    app_canRx_FSM_SteeringAngle_update(steering_angle);

    // make motor speed in range
    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    // make battery in temp range
    app_canRx_BMS_MaxCellTemperature_update(40);

    // make battery close to full so start tapering torque request
    app_canRx_BMS_MaxCellVoltage_update(3.95f);

    app_canRx_INVL_MotorTemperature_update(30.0f);
    app_canRx_INVR_MotorTemperature_update(30.0f);

    // regular active differential
    app_regen_computeActiveDifferentialTorque(&inputs, &regen_braking_inputs);

    float expected_left_torque_request  = regen_braking_inputs.left_inverter_torque_Nm;
    float expected_right_torque_request = regen_braking_inputs.right_inverter_torque_Nm;

    app_regen_run(pedal_percentage);

    bool  alert                  = app_canAlerts_VC_Warning_RegenNotAvailable_get();
    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_TRUE(alert == false);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}

// tapers torque request due in 5-10kph range, exceed max regen
TEST_F(TestRegen, hysterisis_derate)
{
    float                 pedal_percentage      = -1.0f;
    float                 steering_angle        = 0.0f;
    float                 right_motor_speed_rpm = MOTOR_KMH_TO_RPM(9.0f);
    float                 left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(9.0f);
    float motor_speed_rpm MIN(right_motor_speed_rpm, left_motor_speed_rpm);
    float                 power_max_kW = -pedal_percentage * POWER_LIMIT_REGEN_kW;

    static ActiveDifferential_Inputs inputs = { steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE, left_motor_speed_rpm,
                                                right_motor_speed_rpm, power_max_kW, pedal_percentage };

    static RegenBraking_Inputs regen_braking_inputs = {
        .enable_active_differential = true,
        .derating_value             = 1.0f * (MOTOR_RPM_TO_KMH(motor_speed_rpm) - SPEED_MIN_kph) / SPEED_MIN_kph
    };

    // set steering wheel angle
    app_canRx_FSM_SteeringAngle_update(steering_angle);

    // make motor speed in range
    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    // make battery in temp range
    app_canRx_BMS_MaxCellTemperature_update(40);

    // make battery in range
    app_canRx_BMS_MaxCellVoltage_update(3.8f);

    app_canRx_INVL_MotorTemperature_update(30.0f);
    app_canRx_INVR_MotorTemperature_update(30.0f);

    app_regen_computeActiveDifferentialTorque(&inputs, &regen_braking_inputs);

    float expected_left_torque_request  = regen_braking_inputs.left_inverter_torque_Nm;
    float expected_right_torque_request = regen_braking_inputs.right_inverter_torque_Nm;

    app_regen_run(pedal_percentage);

    // Speed above 5km/hr
    bool  alert                  = app_canAlerts_VC_Warning_RegenNotAvailable_get();
    bool  enabled                = app_canTx_VC_RegenEnabled_get();
    float actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    float actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_TRUE(alert == false);
    ASSERT_TRUE(enabled == true);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);

    // Speed below 5km/hr turn off
    right_motor_speed_rpm = MOTOR_KMH_TO_RPM(4.0f);
    left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(4.0f);

    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    app_regen_run(pedal_percentage);

    alert                  = app_canAlerts_VC_Warning_RegenNotAvailable_get();
    enabled                = app_canTx_VC_RegenEnabled_get();
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_TRUE(alert == true);
    ASSERT_TRUE(enabled == false);
    ASSERT_FLOAT_EQ(0.0f, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(0.0f, actual_torque_right_nM);

    // Regen not enabled again until speed above 7km/hr
    right_motor_speed_rpm = MOTOR_KMH_TO_RPM(6.0f);
    left_motor_speed_rpm  = MOTOR_KMH_TO_RPM(6.0f);

    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    app_regen_run(pedal_percentage);

    alert                  = app_canAlerts_VC_Warning_RegenNotAvailable_get();
    enabled                = app_canTx_VC_RegenEnabled_get();
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_TRUE(alert == true);
    ASSERT_TRUE(enabled == false);
    ASSERT_FLOAT_EQ(0.0f, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(0.0f, actual_torque_right_nM);

    // Speed above 7km/hr, turn on regen again
    right_motor_speed_rpm               = MOTOR_KMH_TO_RPM(7.5f);
    left_motor_speed_rpm                = MOTOR_KMH_TO_RPM(7.5f);
    motor_speed_rpm                     = MIN(right_motor_speed_rpm, left_motor_speed_rpm);
    regen_braking_inputs.derating_value = 1 * (MOTOR_RPM_TO_KMH(motor_speed_rpm) - SPEED_MIN_kph) / SPEED_MIN_kph;

    app_canRx_INVR_MotorSpeed_update(-right_motor_speed_rpm);
    app_canRx_INVL_MotorSpeed_update(left_motor_speed_rpm);

    app_regen_computeActiveDifferentialTorque(&inputs, &regen_braking_inputs);

    expected_left_torque_request  = regen_braking_inputs.left_inverter_torque_Nm;
    expected_right_torque_request = regen_braking_inputs.right_inverter_torque_Nm;

    app_regen_run(pedal_percentage);

    alert                  = app_canAlerts_VC_Warning_RegenNotAvailable_get();
    enabled                = app_canTx_VC_RegenEnabled_get();
    actual_torque_left_nM  = app_canTx_VC_LeftInverterTorqueCommand_get();
    actual_torque_right_nM = app_canTx_VC_RightInverterTorqueCommand_get();

    ASSERT_TRUE(alert == false);
    ASSERT_TRUE(enabled == true);
    ASSERT_FLOAT_EQ(expected_left_torque_request, actual_torque_left_nM);
    ASSERT_FLOAT_EQ(expected_right_torque_request, actual_torque_right_nM);
}

TEST_F(TestRegen, pedal_remap)
{
    // accelerating range
    float pedal_remap  = app_regen_pedalRemapping(1);
    float actual_pedal = 1.0f;
    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    pedal_remap  = app_regen_pedalRemapping(0.80);
    actual_pedal = (0.8f - 0.3f) / 0.7f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    pedal_remap  = app_regen_pedalRemapping(0.50);
    actual_pedal = (0.5f - 0.3f) / 0.7f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    // cruising range
    pedal_remap  = app_regen_pedalRemapping(0.2999);
    actual_pedal = 0.0f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    pedal_remap  = app_regen_pedalRemapping(0.25);
    actual_pedal = 0.0f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    // regen range

    pedal_remap  = app_regen_pedalRemapping(0.19);
    actual_pedal = (0.19f - 0.2f) / 0.2f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    pedal_remap  = app_regen_pedalRemapping(0.05);
    actual_pedal = (0.05f - 0.2f) / 0.2f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);

    pedal_remap  = app_regen_pedalRemapping(0.0);
    actual_pedal = -1.0f;

    ASSERT_FLOAT_EQ(actual_pedal, pedal_remap);
}