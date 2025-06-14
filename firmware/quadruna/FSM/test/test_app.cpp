#include "test_fsmBaseStateMachineTest.h"
#include <cmath>

class FsmAppTest : public FsmBaseStateMachineTest
{
  protected:
    template <typename T>
    void CheckInRangeCanSignals(
        const float min_value,
        const float max_value,
        void (*fake_setter)(float),
        T (*value_can_signal_getter)(),
        bool (*alert_getter)())
    {
        // Normal range
        const float nominal_val = (min_value + max_value) / 2;
        fake_setter(nominal_val);
        LetTimePass(10);
        ASSERT_EQ(nominal_val, value_can_signal_getter());
        ASSERT_FALSE(alert_getter());

        // Underflow range
        fake_setter(std::nextafter(min_value, std::numeric_limits<float>::lowest()));
        LetTimePass(10);
        ASSERT_TRUE(alert_getter());

        // Overflow range
        fake_setter(std::nextafter(max_value, std::numeric_limits<float>::max()));
        LetTimePass(10);
        ASSERT_TRUE(alert_getter());
    }

    void CheckBinaryStatusCanSignal(void (*fake_setter)(bool), bool (*can_signal_getter)())
    {
        fake_setter(true);
        LetTimePass(10);
        ASSERT_TRUE(can_signal_getter());

        fake_setter(false);
        LetTimePass(10);
        ASSERT_FALSE(can_signal_getter());
    }
};

TEST_F(FsmAppTest, check_mapped_pedal_percentage_can_signals)
{
    // For the following tests we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Normal range
    fake_io_apps_getPrimary_returns(50.0);
    fake_io_apps_getSecondary_returns(50.0);
    LetTimePass(10);
    ASSERT_NEAR(50.0, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
}

TEST_F(FsmAppTest, check_brake_can_signals)
{
    // front and rear pressure in range
    CheckInRangeCanSignals(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, fake_io_brake_getFrontPressurePsi_returns,
        app_canTx_FSM_FrontBrakePressure_get, app_canAlerts_FSM_Warning_FrontBrakePressureOutOfRange_get);
    CheckInRangeCanSignals(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, fake_io_brake_getRearPressurePsi_returns,
        app_canTx_FSM_RearBrakePressure_get, app_canAlerts_FSM_Warning_RearBrakePressureOutOfRange_get);

    // actuation
    CheckBinaryStatusCanSignal(fake_io_brake_isActuated_returns, app_canTx_FSM_BrakeActuated_get);

    // front pressure and rear pressure both cause "pressure OCSC" behaviour
    CheckBinaryStatusCanSignal(
        fake_io_brake_frontPressureSensorOCSC_returns, app_canAlerts_FSM_Warning_FrontBrakePressureOcSc_get);

    CheckBinaryStatusCanSignal(
        fake_io_brake_rearPressureSensorOCSC_returns, app_canAlerts_FSM_Warning_RearBrakePressureOcSc_get);

    CheckBinaryStatusCanSignal(fake_io_brake_hwOCSC_returns, app_canAlerts_FSM_Warning_BrakeOcScNotOk_get);
}

TEST_F(FsmAppTest, check_steering_angle_can_signals)
{
    CheckInRangeCanSignals(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, fake_io_steering_getAngleDegrees_returns,
        app_canTx_FSM_SteeringAngle_get, app_canAlerts_FSM_Warning_SteeringAngleOutOfRange_get);
}

TEST_F(FsmAppTest, check_left_wheel_speed_can_signals)
{
    CheckInRangeCanSignals(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, fake_io_wheels_getLeftSpeedKph_returns,
        app_canTx_FSM_LeftWheelSpeed_get, app_canAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_get);
}

TEST_F(FsmAppTest, check_right_wheel_speed_can_signals)
{
    CheckInRangeCanSignals(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, fake_io_wheels_getRightSpeedKph_returns,
        app_canTx_FSM_RightWheelSpeed_get, app_canAlerts_FSM_Warning_RightWheelSpeedOutOfRange_get);
}

// TEST_F(FsmAppTest, check_left_suspension_can_signals)
// {
//     fake_io_suspension_getLeftTravel_returns(50.0);
//     LetTimePass(10);
//     // ASSERT_EQ(app_canTx_FSM_LeftSuspensionTravel_get(), 50.0);

//     CheckBinaryStatusCanSignal(
//         fake_io_suspension_leftSensorOCSC_returns, app_canAlerts_FSM_Warning_LeftSuspensionOCSC_get);
// }

// TEST_F(FsmAppTest, check_right_suspension_can_signals)
// {
//     fake_io_suspension_getRightTravel_returns(50.0);
//     LetTimePass(10);
//     // ASSERT_EQ(app_canTx_FSM_RightSuspensionTravel_get(), 50.0);

//     CheckBinaryStatusCanSignal(
//         fake_io_suspension_rightSensorOCSC_returns, app_canAlerts_FSM_Warning_RightSuspensionOCSC_get);
// }
