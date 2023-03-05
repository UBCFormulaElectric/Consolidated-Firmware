#include <cmath>
#include "Test_Fsm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
// shared
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_Timer.h"
// timing
#include "configs/App_AcceleratorSignalTimings.h"
#include "configs/App_FlowRateThresholds.h"
#include "configs/App_CoolantSignalTimings.h"
#include "configs/App_WheelSpeedThresholds.h"
#include "configs/App_SteeringAngleThresholds.h"
#include "configs/App_BrakePressureThresholds.h"
// states
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_STARTUP, const struct CanMsgs_fsm_startup_t *)
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_AIR_SHUTDOWN, const struct CanMsgs_fsm_air_shutdown_t *)
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT, const struct CanMsgs_fsm_watchdog_timeout_t *)
FAKE_VALUE_FUNC(uint32_t, get_current_ms)
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot)

// coolant
FAKE_VALUE_FUNC(float, coolant_get_flow_rate)
FAKE_VALUE_FUNC(float, coolant_get_temp_a)
FAKE_VALUE_FUNC(float, coolant_get_temp_b)
FAKE_VALUE_FUNC(float, coolant_get_pressure_a)
FAKE_VALUE_FUNC(float, coolant_get_pressure_b)

// wheel
FAKE_VALUE_FUNC(float, wheel_get_left_speed)
FAKE_VALUE_FUNC(float, wheel_get_right_speed)
// steer
FAKE_VALUE_FUNC(float, get_steering_angle)
FAKE_VALUE_FUNC(bool, steering_OCSC)
// brake
FAKE_VALUE_FUNC(float, brake_get_front_pressure)
FAKE_VALUE_FUNC(float, brake_get_rear_pressure)
FAKE_VALUE_FUNC(float, brake_get_pedal_travel)
FAKE_VALUE_FUNC(bool, brake_front_pressure_OCSC)
FAKE_VALUE_FUNC(bool, brake_rear_pressure_OCSC)
FAKE_VALUE_FUNC(bool, brake_pedal_OCSC)
FAKE_VALUE_FUNC(bool, is_brake_actuated)

// accelerators
FAKE_VALUE_FUNC(float, get_papps)
FAKE_VALUE_FUNC(bool, papps_OCSC)
FAKE_VALUE_FUNC(float, get_sapps)
FAKE_VALUE_FUNC(bool, sapps_OCSC)

class FsmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        papps_and_sapps = App_AcceleratorPedals_Create(get_papps, papps_OCSC, get_sapps, sapps_OCSC);
        brake           = App_Brake_Create(
            brake_get_front_pressure, brake_front_pressure_OCSC, brake_get_rear_pressure, brake_rear_pressure_OCSC,
            brake_get_pedal_travel, brake_pedal_OCSC, is_brake_actuated);
        coolant = App_Coolant_Create(
            coolant_get_flow_rate, coolant_get_temp_a, coolant_get_temp_b, coolant_get_pressure_a,
            coolant_get_pressure_b);
        steering = App_Steering_Create(get_steering_angle, steering_OCSC);
        wheels   = App_Wheels_Create(wheel_get_left_speed, wheel_get_right_speed);
        world    = App_FsmWorld_Create(heartbeat_monitor, papps_and_sapps, brake, coolant, steering, wheels);

        // Default to starting the state machine in the Drive state
        fsm_state_machine = App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_FSM_STARTUP)
        RESET_FAKE(send_non_periodic_msg_FSM_AIR_SHUTDOWN)
        RESET_FAKE(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT)
        RESET_FAKE(get_current_ms)
        RESET_FAKE(heartbeat_timeout_callback)
        RESET_FAKE(coolant_get_flow_rate)
        RESET_FAKE(coolant_get_temp_a)
        RESET_FAKE(coolant_get_temp_b)
        RESET_FAKE(coolant_get_pressure_a)
        RESET_FAKE(coolant_get_pressure_b)
        RESET_FAKE(wheel_get_left_speed)
        RESET_FAKE(wheel_get_right_speed)
        RESET_FAKE(get_steering_angle)
        RESET_FAKE(steering_OCSC)
        RESET_FAKE(brake_get_front_pressure)
        RESET_FAKE(brake_get_rear_pressure)
        RESET_FAKE(brake_get_pedal_travel)
        RESET_FAKE(brake_front_pressure_OCSC)
        RESET_FAKE(brake_rear_pressure_OCSC)
        RESET_FAKE(brake_pedal_OCSC)
        RESET_FAKE(is_brake_actuated)
        RESET_FAKE(get_papps)
        RESET_FAKE(papps_OCSC)
        RESET_FAKE(get_sapps)
        RESET_FAKE(sapps_OCSC)
    }

    void TearDown() override
    {
        TearDownObject(world, App_FsmWorld_Destroy);
        TearDownObject(fsm_state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(papps_and_sapps, App_AcceleratorPedals_Destroy);
        TearDownObject(brake, App_Brake_Destroy);
        TearDownObject(coolant, App_Coolant_Destroy);
        TearDownObject(steering, App_Steering_Destroy);
        TearDownObject(wheels, App_Wheels_Destroy);
    }

    // overrides
    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        UNUSED(state_machine);
        App_Timer_SetCurrentTimeMS(current_time_ms);
    }
    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    };
    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(fsm_state_machine, App_SharedStateMachine_Destroy);
        fsm_state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(fsm_state_machine));
    }
    static std::vector<const struct State *> GetAllStates()
    {
        return std::vector<const struct State *>{ App_GetDriveState(), App_GetFaultState() };
    }

    template <typename T>
    void CheckInRangeCanSignalsInAllStates(
        float  min_value,
        float  max_value,
        float &fake_value,
        T (*value_can_signal_getter)(),
        uint8_t (*out_of_range_can_signal_getter)())
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal range
            fake_value = (min_value + max_value) / 2;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(fake_value, value_can_signal_getter());
            ASSERT_EQ(RANGE_CHECK_OK, out_of_range_can_signal_getter());

            // Underflow range
            fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(RANGE_CHECK_UNDERFLOW, out_of_range_can_signal_getter());

            // Overflow range
            fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(RANGE_CHECK_OVERFLOW, out_of_range_can_signal_getter());
        }
    }

    void CheckBinaryStatusCanSignalInAllStates(bool &fake_value, uint8_t (*can_signal_getter)())
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            fake_value = true;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_TRUE(can_signal_getter());

            fake_value = false;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_FALSE(can_signal_getter());
        }
    }

    // USER CODE
    struct World *            world{};
    struct StateMachine *     fsm_state_machine{};
    struct FsmCanTxInterface *can_tx_interface{};
    struct FsmCanRxInterface *can_rx_interface{};
    struct HeartbeatMonitor * heartbeat_monitor{};

    struct AcceleratorPedals *papps_and_sapps{};
    struct Brake *            brake{};
    struct Coolant *          coolant{};
    struct Steering *         steering{};
    struct Wheels *           wheels{};
};

//========================TESTS FOR ERRORS========================
// FSM-16
TEST_F(
    FsmStateMachineTest,
    papps_alarm_error_sets_mapped_pedal_percentage_to_zero_and_sets_motor_shutdown_can_tx_signal)
{
    // For the following test we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal positions to prevent false positive
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 50;
    LetTimePass(fsm_state_machine, 10);

    papps_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 10);

    // check before it is still safe
    LetTimePass(fsm_state_machine, PAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_Warning_PappsOCSCIsActive_Get());

    // check after it has changed
    LetTimePass(fsm_state_machine, 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_Warning_PappsOCSCIsActive_Get());
}
// FSM-5, FSM-16
TEST_F(
    FsmStateMachineTest,
    sapps_alarm_error_sets_mapped_pedal_percentage_to_zero_and_sets_motor_shutdown_can_tx_signal)
{
    // For the following test we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal position to avoid false positives
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 50;
    LetTimePass(fsm_state_machine, 10);

    sapps_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 10);

    // before
    LetTimePass(fsm_state_machine, SAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanTx_FSM_Warning_SappsOCSCIsActive_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);

    // after
    LetTimePass(fsm_state_machine, 1);
    ASSERT_TRUE(App_CanTx_FSM_Warning_SappsOCSCIsActive_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
}

TEST_F(FsmStateMachineTest, papp_and_sapp_disagreement_error_on_can_and_torque_0)
{
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 39;
    LetTimePass(fsm_state_machine, 10 + AGREEMENT_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanTx_FSM_Warning_AppsDisagreementIsActive_Get());

    LetTimePass(fsm_state_machine, 1);
    ASSERT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_Warning_AppsDisagreementIsActive_Get());
}
// FSM-3
TEST_F(FsmStateMachineTest, brake_is_actuated_sets_mapped_pedal_percentage_to_zero_in_drive_state)
{
    RESET_FAKE(is_brake_actuated)

    // Start with a non-zero pedal position to avoid false positives. In
    // addition, the chosen primary brake pedal percentage will not trigger
    // the APPS and brake plausibility callback function
    get_papps_fake.return_val         = 10;
    get_sapps_fake.return_val         = 10;
    is_brake_actuated_fake.return_val = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_NEAR(10, std::round(App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get()), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_Warning_BrakeAccDisagreement_Get());

    get_papps_fake.return_val         = 30;
    get_sapps_fake.return_val         = 30;
    is_brake_actuated_fake.return_val = true;
    LetTimePass(fsm_state_machine, 9);
    ASSERT_FALSE(App_CanTx_FSM_Warning_BrakeAccDisagreement_Get());
    ASSERT_NEAR(10, std::round(App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get()), 0.5f);
    LetTimePass(fsm_state_machine, 11);
    ASSERT_TRUE(App_CanTx_FSM_Warning_BrakeAccDisagreement_Get());
    ASSERT_NEAR(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
}

TEST_F(FsmStateMachineTest, brake_pedal_OCSC)
{
    brake_get_pedal_travel_fake.return_val = 30;
    brake_pedal_OCSC_fake.return_val       = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_FALSE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());
    ASSERT_NEAR(30, App_CanTx_FSM_Brake_BrakePedalPercentage_Get(), 0.5f);

    brake_pedal_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 9);
    ASSERT_FALSE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());
    ASSERT_NEAR(30, App_CanTx_FSM_Brake_BrakePedalPercentage_Get(), 0.5f);
    LetTimePass(fsm_state_machine, 1);
    ASSERT_TRUE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());
    ASSERT_EQ(0, App_CanTx_FSM_Brake_BrakePedalPercentage_Get());
}

TEST_F(FsmStateMachineTest, primary_flow_rate_underflow_sets_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(fsm_state_machine, 10);

    LetTimePass(fsm_state_machine, FLOW_METER_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanTx_FSM_Warning_FlowMeterHasUnderflow_Get());

    LetTimePass(fsm_state_machine, 1);
    ASSERT_TRUE(App_CanTx_FSM_Warning_FlowMeterHasUnderflow_Get());

    LetTimePass(fsm_state_machine, 1000);
    ASSERT_TRUE(App_CanTx_FSM_Warning_FlowMeterHasUnderflow_Get());
}

TEST_F(FsmStateMachineTest, primary_flow_rate_in_range_clears_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(fsm_state_machine, 1000);

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::max());
    LetTimePass(fsm_state_machine, 1000);
    ASSERT_FALSE(App_CanTx_FSM_Warning_FlowMeterHasUnderflow_Get());

    LetTimePass(fsm_state_machine, 1000);
    ASSERT_FALSE(App_CanTx_FSM_Warning_FlowMeterHasUnderflow_Get());
}

TEST_F(FsmStateMachineTest, steering_sensor_OCSC)
{
    steering_OCSC_fake.return_val = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_FALSE(App_CanTx_FSM_Warning_SteeringAngleSensorOCSC_Get());

    steering_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_TRUE(App_CanTx_FSM_Warning_SteeringAngleSensorOCSC_Get());
}

//========================TESTS FOR VALUES========================
// FSM-3, FSM-4
TEST_F(FsmStateMachineTest, check_mapped_pedal_percentage_can_signals_in_drive_state)
{
    // For the following tests we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Normal range
    get_papps_fake.return_val = 50.0;
    get_sapps_fake.return_val = 50.0;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_NEAR(50.0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);

    /*REMOVED when switched to linear pot
    // Underflow range
    // Decrement fake_value by +/-1 to ensure that the encoder counter value
    // is within the lower deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(0) - 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(0) + 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);

    // Overflow range
    // Increment fake_value by 1 to ensure that the encoder counter value is
    // within the upper deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(100) + 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(100) - 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(100, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
     */
}
// FSM-18
TEST_F(FsmStateMachineTest, check_brake_can_signals_in_all_states)
{
    // front and rear pressure in range
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_front_pressure_fake.return_val,
        App_CanTx_FSM_Brake_FrontBrakePressure_Get,
        (uint8_t(*)())App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Get);
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_rear_pressure_fake.return_val,
        App_CanTx_FSM_Brake_RearBrakePressure_Get, (uint8_t(*)())App_CanTx_FSM_Warning_RearBrakePressureOutOfRange_Get);

    // actuation
    CheckBinaryStatusCanSignalInAllStates(
        is_brake_actuated_fake.return_val, (uint8_t(*)(void))App_CanTx_FSM_Brake_IsActuated_Get);

    // front pressure and rear pressure both cause "pressure OCSC" behaviour
    CheckBinaryStatusCanSignalInAllStates(
        brake_front_pressure_OCSC_fake.return_val,
        (uint8_t(*)(void))App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Get);
    CheckBinaryStatusCanSignalInAllStates(
        brake_rear_pressure_OCSC_fake.return_val,
        (uint8_t(*)(void))App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Get);
}
// FSM-14
TEST_F(FsmStateMachineTest, check_primary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN, coolant_get_flow_rate_fake.return_val,
        App_CanTx_FSM_Coolant_FlowRate_Get, (uint8_t(*)(void))App_CanTx_FSM_Warning_FlowRateOutOfRange_Get);
}

TEST_F(FsmStateMachineTest, check_coolant_pressure_temperature_can_signals_in_all_states)
{
    for (const State *currentState : GetAllStates())
    {
        SetInitialState(currentState);

        coolant_get_temp_a_fake.return_val     = 30;
        coolant_get_temp_b_fake.return_val     = 30;
        coolant_get_pressure_a_fake.return_val = 30;
        coolant_get_pressure_b_fake.return_val = 30;
        LetTimePass(fsm_state_machine, 10);

        ASSERT_EQ(App_CanTx_FSM_Coolant_TemperatureA_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_Coolant_TemperatureB_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_Coolant_PressureA_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_Coolant_PressureB_Get(), 30);
    }
}
// FSM-8
TEST_F(FsmStateMachineTest, check_steering_angle_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, get_steering_angle_fake.return_val,
        App_CanTx_FSM_Steering_SteeringAngle_Get, (uint8_t(*)(void))App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Get);
}
// FSM-9
TEST_F(FsmStateMachineTest, check_left_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, wheel_get_left_speed_fake.return_val,
        App_CanTx_FSM_Wheels_LeftWheelSpeed_Get,
        (uint8_t(*)(void))(App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Get));
}
// FSM-9
TEST_F(FsmStateMachineTest, check_right_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, wheel_get_right_speed_fake.return_val,
        App_CanTx_FSM_Wheels_RightWheelSpeed_Get,
        (uint8_t(*)(void))App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Get);
}

//========================OTHERS========================
// FSM-10
TEST_F(FsmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());
    EXPECT_EQ(FSM_STATE_DRIVE, App_CanTx_FSM_Vitals_State_Get());
}
// FSM-10
TEST_F(FsmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());
    EXPECT_EQ(FSM_STATE_FAULT, App_CanTx_FSM_Vitals_State_Get());
}
} // namespace StateMachineTest
