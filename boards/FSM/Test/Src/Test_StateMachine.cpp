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
FAKE_VALUE_FUNC(bool, coolant_temp_a_OCSC)
FAKE_VALUE_FUNC(bool, coolant_temp_b_OCSC)
FAKE_VALUE_FUNC(bool, coolant_pressure_a_OCSC)
FAKE_VALUE_FUNC(bool, coolant_pressure_b_OCSC)

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

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_FSM_STARTUP, send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
            send_non_periodic_msg_FSM_AIR_SHUTDOWN);
        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        papps_and_sapps = App_AcceleratorPedals_Create(get_papps, papps_OCSC, get_sapps, sapps_OCSC);
        brake           = App_Brake_Create(
            brake_get_front_pressure, brake_front_pressure_OCSC, brake_get_rear_pressure, brake_rear_pressure_OCSC,
            brake_get_pedal_travel, brake_pedal_OCSC, is_brake_actuated);
        coolant = App_Coolant_Create(
            coolant_get_flow_rate, coolant_get_temp_a, coolant_temp_a_OCSC, coolant_get_temp_b, coolant_temp_b_OCSC, coolant_get_pressure_a,
            coolant_pressure_a_OCSC, coolant_get_pressure_b, coolant_pressure_b_OCSC);
        steering = App_Steering_Create(get_steering_angle, steering_OCSC);
        wheels   = App_Wheels_Create(wheel_get_left_speed, wheel_get_right_speed);
        world    = App_FsmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor, papps_and_sapps, brake, coolant, steering, wheels);

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
        RESET_FAKE(coolant_temp_a_OCSC)
        RESET_FAKE(coolant_temp_b_OCSC)
        RESET_FAKE(coolant_pressure_a_OCSC)
        RESET_FAKE(coolant_pressure_b_OCSC)
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
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
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

    void CheckInRangeCanSignalsInAllStates(
        float  min_value,
        float  max_value,
        float &fake_value,
        float (*value_can_signal_getter)(const struct FsmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct FsmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal range
            fake_value = (min_value + max_value) / 2;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
            ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Underflow range
            fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Overflow range
            fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    void CheckBinaryStatusCanSignalInAllStates(
        bool &fake_value,
        uint8_t (*can_signal_getter)(const struct FsmCanTxInterface *),
        uint8_t on_choice,
        uint8_t off_choice)
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            fake_value = true;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(on_choice, can_signal_getter(can_tx_interface));

            fake_value = false;
            LetTimePass(fsm_state_machine, 10);
            ASSERT_EQ(off_choice, can_signal_getter(can_tx_interface));
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

    // check before it is still safe
    LetTimePass(fsm_state_machine, PAPPS_OCSC_TIME_TO_FAULT);
    ASSERT_NEAR(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    // check after it has changed
    LetTimePass(fsm_state_machine, 10);
    ASSERT_FLOAT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));
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

    // before
    LetTimePass(fsm_state_machine, SAPPS_OCSC_TIME_TO_FAULT);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_NEAR(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);

    // after
    LetTimePass(fsm_state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_FLOAT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
}

TEST_F(FsmStateMachineTest, papp_and_sapp_disagreement_error_on_can_and_torque_0)
{
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 39;
    LetTimePass(fsm_state_machine, 10 + AGREEMENT_TIME_TO_FAULT - 1);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface),
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);

    LetTimePass(fsm_state_machine, 1);
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface),
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE);
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
    ASSERT_NEAR(10, std::round(App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface)), 0.5f);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_BRAKE_ACC_DISAGREEMENT(can_tx_interface),
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_BRAKE_ACC_DISAGREEMENT_FALSE_CHOICE);

    get_papps_fake.return_val         = 30;
    get_sapps_fake.return_val         = 30;
    is_brake_actuated_fake.return_val = true;
    LetTimePass(fsm_state_machine, 9);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_BRAKE_ACC_DISAGREEMENT(can_tx_interface),
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_BRAKE_ACC_DISAGREEMENT_FALSE_CHOICE);
    ASSERT_NEAR(10, std::round(App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface)), 0.5f);
    LetTimePass(fsm_state_machine, 11);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_BRAKE_ACC_DISAGREEMENT(can_tx_interface),
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_BRAKE_ACC_DISAGREEMENT_TRUE_CHOICE);
    ASSERT_NEAR(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);
}

TEST_F(FsmStateMachineTest, brake_pedal_OCSC)
{
    brake_get_pedal_travel_fake.return_val = 30;
    brake_pedal_OCSC_fake.return_val       = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_FALSE(App_CanTx_GetPeriodicSignal_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT(can_tx_interface));
    ASSERT_NEAR(30, App_CanTx_GetPeriodicSignal_BRAKE_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);

    brake_pedal_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 9);
    ASSERT_FALSE(App_CanTx_GetPeriodicSignal_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT(can_tx_interface));
    ASSERT_NEAR(30, App_CanTx_GetPeriodicSignal_BRAKE_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);
    LetTimePass(fsm_state_machine, 1);
    ASSERT_TRUE(App_CanTx_GetPeriodicSignal_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT(can_tx_interface));
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_BRAKE_PEDAL_PERCENTAGE(can_tx_interface));
}

TEST_F(FsmStateMachineTest, primary_flow_rate_underflow_sets_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(fsm_state_machine, FLOW_METER_TIME_TO_FAULT - 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(can_tx_interface));

    // TODO reconsider timing
    LetTimePass(fsm_state_machine, 20);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(fsm_state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(can_tx_interface));
}

TEST_F(FsmStateMachineTest, primary_flow_rate_in_range_clears_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(fsm_state_machine, 1000);

    coolant_get_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::max());
    LetTimePass(fsm_state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(fsm_state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(can_tx_interface));
}

TEST_F(FsmStateMachineTest, coolant_pressure_temperature_OCSC) {
    coolant_pressure_a_OCSC_fake.return_val = false;
    coolant_pressure_b_OCSC_fake.return_val = false;
    coolant_temp_a_OCSC_fake.return_val = false;
    coolant_temp_b_OCSC_fake.return_val = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_A_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_PRESSURE_A_OCSC_FALSE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_B_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_PRESSURE_B_OCSC_FALSE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_A_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_TEMPERATURE_A_OCSC_FALSE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_B_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_TEMPERATURE_B_OCSC_FALSE_CHOICE);
    coolant_pressure_a_OCSC_fake.return_val = true;
    coolant_pressure_b_OCSC_fake.return_val = true;
    coolant_temp_a_OCSC_fake.return_val = true;
    coolant_temp_b_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_A_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_PRESSURE_A_OCSC_TRUE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_B_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_PRESSURE_B_OCSC_TRUE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_A_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_TEMPERATURE_A_OCSC_TRUE_CHOICE);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_B_OCSC(can_tx_interface), CANMSGS_FSM_COOLANT_FLAGS_TEMPERATURE_B_OCSC_TRUE_CHOICE);
}

TEST_F(FsmStateMachineTest, steering_sensor_OCSC) {
    steering_OCSC_fake.return_val = false;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_STEERING_SENSOR_OCSC(can_tx_interface), CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC_FALSE_CHOICE);

    steering_OCSC_fake.return_val = true;
    LetTimePass(fsm_state_machine, 10);
    ASSERT_EQ(App_CanTx_GetPeriodicSignal_STEERING_SENSOR_OCSC(can_tx_interface), CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC_TRUE_CHOICE);
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
    ASSERT_NEAR(50.0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);

    /*REMOVED when switched to linear pot
    // Underflow range
    // Decrement fake_value by +/-1 to ensure that the encoder counter value
    // is within the lower deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(0) - 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(0) + 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);

    // Overflow range
    // Increment fake_value by 1 to ensure that the encoder counter value is
    // within the upper deadzone
    get_papps_encoder_counter_fake.return_val = GetPrimaryEncoderCounterFromPedalPercentage(100) + 1;
    get_sapps_encoder_counter_fake.return_val = GetSecondaryEncoderCounterFromPedalPercentage(100) - 1;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(100, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface), 0.5f);
     */
}
// FSM-18
TEST_F(FsmStateMachineTest, check_brake_can_signals_in_all_states)
{
    // front and rear pressure in range
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_front_pressure_fake.return_val,
        App_CanTx_GetPeriodicSignal_FRONT_BRAKE_PRESSURE, App_CanTx_GetPeriodicSignal_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, brake_get_rear_pressure_fake.return_val,
        App_CanTx_GetPeriodicSignal_REAR_BRAKE_PRESSURE, App_CanTx_GetPeriodicSignal_BRAKE_REAR_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    // actuation
    CheckBinaryStatusCanSignalInAllStates(
        is_brake_actuated_fake.return_val, App_CanTx_GetPeriodicSignal_BRAKE_IS_ACTUATED,
        CANMSGS_FSM_BRAKE_FLAGS_BRAKE_IS_ACTUATED_TRUE_CHOICE, CANMSGS_FSM_BRAKE_FLAGS_BRAKE_IS_ACTUATED_FALSE_CHOICE);

    // front pressure and rear pressure both cause "pressure OCSC" behaviour
    CheckBinaryStatusCanSignalInAllStates(
        brake_front_pressure_OCSC_fake.return_val, App_CanTx_GetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT,
        CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE,
        CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE);
    CheckBinaryStatusCanSignalInAllStates(
        brake_rear_pressure_OCSC_fake.return_val, App_CanTx_GetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT,
        CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE,
        CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE);
}
// FSM-14
TEST_F(FsmStateMachineTest, check_primary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN, coolant_get_flow_rate_fake.return_val,
        App_CanTx_GetPeriodicSignal_FLOW_RATE, App_CanTx_GetPeriodicSignal_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(FsmStateMachineTest, check_coolant_pressure_temperature_can_signals_in_all_states) {
    for(const State * currentState: GetAllStates()){
        SetInitialState(currentState);

        coolant_get_temp_a_fake.return_val = 30;
        coolant_get_temp_b_fake.return_val = 30;
        coolant_get_pressure_a_fake.return_val = 30;
        coolant_get_pressure_b_fake.return_val = 30;
        LetTimePass(fsm_state_machine, 10);

        ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_A(can_tx_interface), 30);
        ASSERT_EQ(App_CanTx_GetPeriodicSignal_TEMPERATURE_B(can_tx_interface), 30);
        ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_A(can_tx_interface), 30);
        ASSERT_EQ(App_CanTx_GetPeriodicSignal_PRESSURE_B(can_tx_interface), 30);
    }
}
// FSM-8
TEST_F(FsmStateMachineTest, check_steering_angle_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, get_steering_angle_fake.return_val,
        App_CanTx_GetPeriodicSignal_STEERING_ANGLE, App_CanTx_GetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
// FSM-9
TEST_F(FsmStateMachineTest, check_left_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, wheel_get_left_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED, App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
// FSM-9
TEST_F(FsmStateMachineTest, check_right_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, wheel_get_right_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED, App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

//========================OTHERS========================
// FSM-10
TEST_F(FsmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());
    EXPECT_EQ(CANMSGS_FSM_STATE_MACHINE_STATE_DRIVE_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}
// FSM-10
TEST_F(FsmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());
    EXPECT_EQ(CANMSGS_FSM_STATE_MACHINE_STATE_FAULT_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}
} // namespace StateMachineTest
