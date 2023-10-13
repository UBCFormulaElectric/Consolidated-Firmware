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
FAKE_VALUE_FUNC(uint32_t, get_current_ms)
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot)
FAKE_VALUE_FUNC(float, coolant_get_flow_rate)
FAKE_VALUE_FUNC(float, coolant_get_temp_a)
FAKE_VALUE_FUNC(float, coolant_get_temp_b)
FAKE_VALUE_FUNC(float, coolant_get_pressure_a)
FAKE_VALUE_FUNC(float, coolant_get_pressure_b)
FAKE_VALUE_FUNC(float, wheel_get_left_speed)
FAKE_VALUE_FUNC(float, wheel_get_right_speed)
FAKE_VALUE_FUNC(float, get_steering_angle)
FAKE_VALUE_FUNC(bool, steering_OCSC)
FAKE_VALUE_FUNC(float, brake_get_front_pressure)
FAKE_VALUE_FUNC(float, brake_get_rear_pressure)
FAKE_VALUE_FUNC(float, brake_get_pedal_travel)
FAKE_VALUE_FUNC(bool, front_pressure_sensor_ocsc)
FAKE_VALUE_FUNC(bool, rear_pressure_sensor_ocsc)
FAKE_VALUE_FUNC(bool, brake_pedal_OCSC)
FAKE_VALUE_FUNC(bool, is_brake_actuated)
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

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        papps_and_sapps = App_AcceleratorPedals_Create(get_papps, papps_OCSC, get_sapps, sapps_OCSC);
        brake           = App_Brake_Create(
            brake_get_front_pressure, front_pressure_sensor_ocsc, brake_get_rear_pressure, rear_pressure_sensor_ocsc,
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
        RESET_FAKE(front_pressure_sensor_ocsc)
        RESET_FAKE(rear_pressure_sensor_ocsc)
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
        bool (*alert_getter)(uint8_t),
        uint8_t alert_id)
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal range
            fake_value = (min_value + max_value) / 2;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(fake_value, value_can_signal_getter());
            ASSERT_FALSE(alert_getter(alert_id));

            // Underflow range
            fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_TRUE(alert_getter(alert_id));

            // Overflow range
            fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_TRUE(alert_getter(alert_id));
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

    struct World *            world;
    struct StateMachine *     fsm_state_machine;
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct AcceleratorPedals *papps_and_sapps;
    struct Brake *            brake;
    struct Coolant *          coolant;
    struct Steering *         steering;
    struct Wheels *           wheels;
};

//========================TESTS FOR VALUES========================

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
    ASSERT_NEAR(50.0, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);

    /*REMOVED when switched to linear pot
    // Underflow range
    // Decrement fake_value by +/-1 to ensure that the encoder counter value
    // is within the lower deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(0) - 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(0) + 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);

    // Overflow range
    // Increment fake_value by 1 to ensure that the encoder counter value is
    // within the upper deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(100) + 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(100) - 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(100, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
     */
}

TEST_F(FsmStateMachineTest, check_brake_can_signals_in_all_states)
{
    // front and rear pressure in range
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_front_pressure_fake.return_val,
        App_CanTx_FSM_FrontBrakePressure_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FrontBrakePressureOutOfRangeWarning);
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_rear_pressure_fake.return_val,
        App_CanTx_FSM_RearBrakePressure_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        RearBrakePressureOutOfRangeWarning);

    // actuation
    // TODO: Update this once potentiometer is fixed
    //    CheckBinaryStatusCanSignalInAllStates(
    //        is_brake_actuated_fake.return_val, (uint8_t(*)(void))App_CanTx_FSM_IsActuated_Get);

    // front pressure and rear pressure both cause "pressure OCSC" behaviour
    CheckBinaryStatusCanSignalInAllStates(
        front_pressure_sensor_ocsc_fake.return_val, (uint8_t(*)(void))App_CanTx_FSM_BrakePressureSensorOCSC_Get);

    CheckBinaryStatusCanSignalInAllStates(
        rear_pressure_sensor_ocsc_fake.return_val, (uint8_t(*)(void))App_CanTx_FSM_BrakePressureSensorOCSC_Get);
}

TEST_F(FsmStateMachineTest, check_primary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN, coolant_get_flow_rate_fake.return_val,
        App_CanTx_FSM_CoolantFlowRate_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        FlowRateOutOfRangeWarning);
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

        ASSERT_EQ(App_CanTx_FSM_CoolantTemperatureA_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_CoolantTemperatureB_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_CoolantPressureA_Get(), 30);
        ASSERT_EQ(App_CanTx_FSM_CoolantPressureB_Get(), 30);
    }
}

TEST_F(FsmStateMachineTest, check_steering_angle_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, get_steering_angle_fake.return_val,
        App_CanTx_FSM_SteeringAngle_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        SteeringAngleOutOfRangeWarning);
}

TEST_F(FsmStateMachineTest, check_left_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, wheel_get_left_speed_fake.return_val,
        App_CanTx_FSM_LeftWheelSpeed_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        LeftWheelSpeedOutOfRangeWarning);
}

TEST_F(FsmStateMachineTest, check_right_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, wheel_get_right_speed_fake.return_val,
        App_CanTx_FSM_RightWheelSpeed_Get, (bool (*)(uint8_t))App_CanAlerts_GetWarning,
        RightWheelSpeedOutOfRangeWarning);
}

//========================OTHERS========================

TEST_F(FsmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());
    EXPECT_EQ(FSM_STATE_DRIVE, App_CanTx_FSM_State_Get());
}

TEST_F(FsmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());
    EXPECT_EQ(FSM_STATE_FAULT, App_CanTx_FSM_State_Get());
}

} // namespace StateMachineTest
