#include "test_fsmBaseStateMachineTest.h"
#include <cmath>

class FsmAppTest : public FsmBaseStateMachineTest
{
  protected:
    template <typename T>
    void CheckInRangeCanSignals(
        float  min_value,
        float  max_value,
        void (*fake_setter)(float),
        T (*value_can_signal_getter)(),
        bool (*alert_getter)(uint8_t),
        uint8_t alert_id)
    {
        // Normal range
        const float nominal_val = (min_value + max_value) / 2;
        fake_setter(nominal_val);
        LetTimePass(state_machine, 10);
        ASSERT_EQ(nominal_val, value_can_signal_getter());
        ASSERT_FALSE(alert_getter(alert_id));

        // Underflow range
        fake_setter(std::nextafter(min_value, std::numeric_limits<float>::lowest()));
        LetTimePass(state_machine, 10);
        ASSERT_TRUE(alert_getter(alert_id));

        // Overflow range
        fake_setter(std::nextafter(max_value, std::numeric_limits<float>::max()));
        LetTimePass(state_machine, 10);
        ASSERT_TRUE(alert_getter(alert_id));
    }

    void CheckBinaryStatusCanSignal(void (*fake_setter)(bool), uint8_t (*can_signal_getter)())
    {
        fake_setter(true);
        LetTimePass(state_machine, 10);
        ASSERT_TRUE(can_signal_getter());

        fake_setter(false);
        LetTimePass(state_machine, 10);
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
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(50.0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
}

TEST_F(FsmAppTest, check_brake_can_signals)
{
    // front and rear pressure in range
    CheckInRangeCanSignals(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, fake_io_brake_getFrontPressurePsi_returns,
        App_CanTx_FSM_Brake_FrontBrakePressure_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE);
    CheckInRangeCanSignals(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, fake_io_brake_getRearPressurePsi_returns,
        App_CanTx_FSM_Brake_RearBrakePressure_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE);

    // actuation
    CheckBinaryStatusCanSignal(
        fake_io_brake_isActuated_returns, (uint8_t(*)(void))App_CanTx_FSM_Brake_IsActuated_Get);

    // front pressure and rear pressure both cause "pressure OCSC" behaviour
    CheckBinaryStatusCanSignal(
        fake_io_brake_frontPressureSensorOCSC_returns,
        (uint8_t(*)(void))App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Get);

    CheckBinaryStatusCanSignal(
        fake_io_brake_rearPressureSensorOCSC_returns,
        (uint8_t(*)(void))App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Get);
}

TEST_F(FsmAppTest, check_primary_flow_rate_can_signals)
{
    CheckInRangeCanSignals(
        MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN, fake_io_coolant_getFlowRate_returns,
        App_CanTx_FSM_Coolant_FlowRate_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_FLOW_RATE_OUT_OF_RANGE);
}

TEST_F(FsmAppTest, check_coolant_pressure_temperature_can_signals)
{
    fake_io_coolant_getTemperatureA_returns(30);
    fake_io_coolant_getTemperatureB_returns(30);
    fake_io_coolant_getPressureA_returns(30);
    fake_io_coolant_getPressureB_returns(30);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_CanTx_FSM_Coolant_TemperatureA_Get(), 30);
    ASSERT_EQ(App_CanTx_FSM_Coolant_TemperatureB_Get(), 30);
    ASSERT_EQ(App_CanTx_FSM_Coolant_PressureA_Get(), 30);
    ASSERT_EQ(App_CanTx_FSM_Coolant_PressureB_Get(), 30);
}

TEST_F(FsmAppTest, check_steering_angle_can_signals)
{
    CheckInRangeCanSignals(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, fake_io_steering_getAngleDegrees_returns,
        App_CanTx_FSM_Steering_SteeringAngle_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE);
}

TEST_F(FsmAppTest, check_left_wheel_speed_can_signals)
{
    CheckInRangeCanSignals(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, fake_io_wheels_getLeftSpeedKph_returns,
        App_CanTx_FSM_Wheels_LeftWheelSpeed_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE);
}

TEST_F(FsmAppTest, check_right_wheel_speed_can_signals)
{
    CheckInRangeCanSignals(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, fake_io_wheels_getRightSpeedKph_returns,
        App_CanTx_FSM_Wheels_RightWheelSpeed_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE);
}
