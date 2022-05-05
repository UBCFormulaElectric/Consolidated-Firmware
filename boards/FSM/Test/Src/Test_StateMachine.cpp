#include <math.h>
#include "Test_Fsm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedBinaryStatus.h"
#include "App_InRangeCheck.h"
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_AcceleratorPedalSignals.h"
#include "App_FlowMeterSignals.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_FlowRateThresholds.h"
#include "configs/App_WheelSpeedThresholds.h"
#include "configs/App_SteeringAngleThresholds.h"
#include "configs/App_BrakePressureThresholds.h"
#include "configs/App_SignalCallbackDurations.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_STARTUP, const struct CanMsgs_fsm_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_AIR_SHUTDOWN, const struct CanMsgs_fsm_air_shutdown_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT, const struct CanMsgs_fsm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);

FAKE_VALUE_FUNC(float, get_primary_flow_rate);
FAKE_VALUE_FUNC(float, get_secondary_flow_rate);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VALUE_FUNC(float, get_left_wheel_speed);
FAKE_VALUE_FUNC(float, get_right_wheel_speed);
FAKE_VALUE_FUNC(float, get_steering_angle);
FAKE_VALUE_FUNC(float, get_brake_pressure);
FAKE_VALUE_FUNC(bool, is_brake_actuated);
FAKE_VALUE_FUNC(bool, is_pressure_sensor_open_or_short_circuit);
FAKE_VALUE_FUNC(bool, is_papps_encoder_alarm_active);
FAKE_VALUE_FUNC(bool, is_sapps_encoder_alarm_active);
FAKE_VOID_FUNC(reset_papps_encoder_counter);
FAKE_VALUE_FUNC(uint32_t, get_papps_encoder_counter);
FAKE_VOID_FUNC(reset_sapps_encoder_counter);
FAKE_VALUE_FUNC(uint32_t, get_sapps_encoder_counter);

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
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK,
            heartbeat_timeout_callback);

        primary_flow_rate_in_range_check = App_InRangeCheck_Create(
            get_primary_flow_rate, MIN_PRIMARY_FLOW_RATE_L_PER_MIN, MAX_PRIMARY_FLOW_RATE_L_PER_MIN);

        secondary_flow_rate_in_range_check = App_InRangeCheck_Create(
            get_secondary_flow_rate, MIN_SECONDARY_FLOW_RATE_L_PER_MIN, MAX_SECONDARY_FLOW_RATE_L_PER_MIN);

        left_wheel_speed_in_range_check =
            App_InRangeCheck_Create(get_left_wheel_speed, MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH);

        right_wheel_speed_in_range_check =
            App_InRangeCheck_Create(get_right_wheel_speed, MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH);

        steering_angle_in_range_check =
            App_InRangeCheck_Create(get_steering_angle, MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);

        brake = App_Brake_Create(
            get_brake_pressure, is_pressure_sensor_open_or_short_circuit, is_brake_actuated, MIN_BRAKE_PRESSURE_PSI,
            MAX_BRAKE_PRESSURE_PSI);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        clock = App_SharedClock_Create();

        papps_and_sapps = App_AcceleratorPedals_Create(
            is_papps_encoder_alarm_active, is_sapps_encoder_alarm_active, get_papps_encoder_counter,
            get_sapps_encoder_counter, reset_papps_encoder_counter, reset_sapps_encoder_counter);

        world = App_FsmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor, primary_flow_rate_in_range_check,
            secondary_flow_rate_in_range_check, left_wheel_speed_in_range_check, right_wheel_speed_in_range_check,
            steering_angle_in_range_check, brake, rgb_led_sequence, clock, papps_and_sapps,

            App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure,
            App_AcceleratorPedalSignals_IsAppsAndBrakePlausibilityOk,
            App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback,
            App_AcceleratorPedalSignals_HasAppsDisagreement, App_AcceleratorPedalSignals_HasAppsAgreement,
            App_AcceleratorPedalSignals_AppsDisagreementCallback, App_AcceleratorPedalSignals_IsPappsAlarmActive,
            App_AcceleratorPedalSignals_PappsAlarmCallback, App_AcceleratorPedalSignals_IsSappsAlarmActive,
            App_AcceleratorPedalSignals_SappsAlarmCallback, App_AcceleratorPedalSignals_IsPappsAndSappsAlarmInactive,

            App_FlowMetersSignals_IsPrimaryFlowRateBelowThreshold, App_FlowMetersSignals_IsPrimaryFlowRateInRange,
            App_FlowMetersSignals_PrimaryFlowRateBelowThresholdCallback,
            App_FlowMetersSignals_IsSecondaryFlowRateBelowThreshold, App_FlowMetersSignals_IsSecondaryFlowRateInRange,
            App_FlowMetersSignals_SecondaryFlowRateBelowThresholdCallback);

        // Default to starting the state machine in the `AIR_OPEN` state
        state_machine = App_SharedStateMachine_Create(world, App_GetAirOpenState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_FSM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_FSM_AIR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(get_primary_flow_rate);
        RESET_FAKE(get_secondary_flow_rate);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(get_left_wheel_speed);
        RESET_FAKE(get_right_wheel_speed);
        RESET_FAKE(get_steering_angle);
        RESET_FAKE(get_brake_pressure);
        RESET_FAKE(is_brake_actuated);
        RESET_FAKE(is_pressure_sensor_open_or_short_circuit);
        RESET_FAKE(is_papps_encoder_alarm_active);
        RESET_FAKE(is_sapps_encoder_alarm_active);
        RESET_FAKE(reset_papps_encoder_counter);
        RESET_FAKE(get_papps_encoder_counter);
        RESET_FAKE(reset_sapps_encoder_counter);
        RESET_FAKE(get_sapps_encoder_counter);
    }

    void TearDown() override
    {
        TearDownObject(world, App_FsmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(left_wheel_speed_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(right_wheel_speed_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(primary_flow_rate_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(secondary_flow_rate_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(steering_angle_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(brake, App_Brake_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(papps_and_sapps, App_AcceleratorPedals_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{ App_GetAirOpenState(), App_GetAirClosedState() };
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
            LetTimePass(state_machine, 10);
            ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
            ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Underflow range
            fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
            LetTimePass(state_machine, 10);
            ASSERT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Overflow range
            fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
            LetTimePass(state_machine, 10);
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
            LetTimePass(state_machine, 10);
            ASSERT_EQ(on_choice, can_signal_getter(can_tx_interface));

            fake_value = false;
            LetTimePass(state_machine, 10);
            ASSERT_EQ(off_choice, can_signal_getter(can_tx_interface));
        }
    }

    uint32_t GetEncoderCounterFromPedalPercentage(float pedal_percentage, uint32_t encoder_fully_pressed_value)
    {
        // Calculate the encoder counter value from the given mapped pedal
        // percentage
        return round(
            encoder_fully_pressed_value * ((1 - 2 * PERCENT_DEFLECTION) * pedal_percentage / 100 + PERCENT_DEFLECTION));
    }

    void CheckIfAppsDisagreementFaultIsSetWhenAppsHasDisagreement(
        uint32_t &fake_larger_pedal_percentage_encoder_value,
        uint32_t &fake_smaller_pedal_percentage_encoder_value,
        uint32_t  larger_pedal_percentage_max_pressed_value,
        uint32_t  smaller_pedal_percentage_max_pressed_value,
        uint32_t  entry_high_ms,
        uint32_t  exit_high_ms,
        uint8_t   true_choice,
        uint8_t   false_choice)
    {
        const size_t starting_pedal_percentage = 11;

        for (size_t pedal_percentage = starting_pedal_percentage;
             pedal_percentage <= (size_t)MAX_ACCELERATOR_PEDAL_PRESS; pedal_percentage++)
        {
            // Remove pedal disagreement to avoid false positives on the next
            // cycle.
            fake_larger_pedal_percentage_encoder_value  = 0;
            fake_smaller_pedal_percentage_encoder_value = 0;
            LetTimePass(state_machine, exit_high_ms);

            fake_larger_pedal_percentage_encoder_value =
                GetEncoderCounterFromPedalPercentage(pedal_percentage, larger_pedal_percentage_max_pressed_value);

            // Decrement the value of the smaller encoder by 11% to ensure
            // that at least a difference of 10% is maintained.
            fake_smaller_pedal_percentage_encoder_value =
                GetEncoderCounterFromPedalPercentage(pedal_percentage - 11, smaller_pedal_percentage_max_pressed_value);

            LetTimePass(state_machine, entry_high_ms - 1);
            ASSERT_EQ(false_choice, App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface));

            LetTimePass(state_machine, 1);
            ASSERT_EQ(true_choice, App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface));
        }
    }

    void CheckIfAppsDisagreementFaultIsClearedWhenAppsHasAgreement(
        uint32_t &fake_larger_pedal_percentage_encoder_value,
        uint32_t &fake_smaller_pedal_percentage_encoder_value,
        uint32_t  larger_pedal_percentage_max_pressed_value,
        uint32_t  smaller_pedal_percentage_max_pressed_value,
        uint32_t  entry_high_ms,
        uint32_t  exit_high_ms,
        uint8_t   true_choice,
        uint8_t   false_choice)
    {
        for (size_t pedal_percentage = 5; pedal_percentage <= 100; pedal_percentage++)
        {
            // Ensure that the motor shutdown fault is initially set by
            // configuring an APPS disagreement greater than 10%
            fake_larger_pedal_percentage_encoder_value  = larger_pedal_percentage_max_pressed_value / 2.0;
            fake_smaller_pedal_percentage_encoder_value = 0;
            LetTimePass(state_machine, entry_high_ms);
            ASSERT_EQ(true_choice, App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface));

            // Stop the callback function from being triggered by setting the
            // difference between the mapped pedal percentages under 5%
            fake_larger_pedal_percentage_encoder_value =
                GetEncoderCounterFromPedalPercentage(pedal_percentage, larger_pedal_percentage_max_pressed_value);
            fake_smaller_pedal_percentage_encoder_value =
                GetEncoderCounterFromPedalPercentage(pedal_percentage - 5, smaller_pedal_percentage_max_pressed_value);
            LetTimePass(state_machine, exit_high_ms - 1);
            ASSERT_EQ(true_choice, App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface));

            LetTimePass(state_machine, 1);
            ASSERT_EQ(false_choice, App_CanTx_GetPeriodicSignal_APPS_HAS_DISAGREEMENT(can_tx_interface));
        }
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_FsmWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        App_FsmWorld_UpdateSignals(world, current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct InRangeCheck *     primary_flow_rate_in_range_check;
    struct InRangeCheck *     secondary_flow_rate_in_range_check;
    struct InRangeCheck *     left_wheel_speed_in_range_check;
    struct InRangeCheck *     right_wheel_speed_in_range_check;
    struct InRangeCheck *     steering_angle_in_range_check;
    struct Brake *            brake;
    struct RgbLedSequence *   rgb_led_sequence;
    struct Clock *            clock;
    struct AcceleratorPedals *papps_and_sapps;
};

// FSM-10
TEST_F(FsmStateMachineTest, check_air_open_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirOpenState());

    EXPECT_EQ(CANMSGS_FSM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// FSM-10
TEST_F(FsmStateMachineTest, check_air_closed_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirClosedState());

    EXPECT_EQ(CANMSGS_FSM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// FSM-9
TEST_F(FsmStateMachineTest, check_left_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH, get_left_wheel_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED, App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-9
TEST_F(FsmStateMachineTest, check_right_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH, get_right_wheel_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED, App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-14
TEST_F(FsmStateMachineTest, check_primary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_PRIMARY_FLOW_RATE_L_PER_MIN, MAX_PRIMARY_FLOW_RATE_L_PER_MIN, get_primary_flow_rate_fake.return_val,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE, App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-14
TEST_F(FsmStateMachineTest, check_secondary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_SECONDARY_FLOW_RATE_L_PER_MIN, MAX_SECONDARY_FLOW_RATE_L_PER_MIN, get_secondary_flow_rate_fake.return_val,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE, App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-8
TEST_F(FsmStateMachineTest, check_steering_angle_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG, get_steering_angle_fake.return_val,
        App_CanTx_GetPeriodicSignal_STEERING_ANGLE, App_CanTx_GetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-18
TEST_F(FsmStateMachineTest, check_brake_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI, get_brake_pressure_fake.return_val,
        App_CanTx_GetPeriodicSignal_BRAKE_PRESSURE, App_CanTx_GetPeriodicSignal_BRAKE_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    CheckBinaryStatusCanSignalInAllStates(
        is_brake_actuated_fake.return_val, App_CanTx_GetPeriodicSignal_BRAKE_IS_ACTUATED,
        CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_FALSE_CHOICE);

    CheckBinaryStatusCanSignalInAllStates(
        is_pressure_sensor_open_or_short_circuit_fake.return_val,
        App_CanTx_GetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT,
        CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE,
        CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE);
}

TEST_F(FsmStateMachineTest, rgb_led_sequence_in_all_states)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count, &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);

        // Verify that we cycle through red, green, blue, red, etc for 99 times.
        // The number 99 can be changed to any other number that is a multiple
        // of 3. The significance of 3 is because we have 3 colors (Red, green,
        // and blue).
        for (size_t i = 0; i < 99; i++)
        {
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
        }
    }
}

// FSM-12
TEST_F(FsmStateMachineTest, exit_air_open_state_when_air_positive_and_air_negative_are_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetAirClosedState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-12
TEST_F(FsmStateMachineTest, stay_in_air_open_state_if_only_air_positive_is_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetAirOpenState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-12
TEST_F(FsmStateMachineTest, stay_in_air_open_state_if_only_air_negative_is_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetAirOpenState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-15
TEST_F(FsmStateMachineTest, exit_air_closed_state_when_air_positive_is_opened)
{
    SetInitialState(App_GetAirClosedState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetAirOpenState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-15
TEST_F(FsmStateMachineTest, exit_air_closed_state_when_air_negative_is_opened)
{
    SetInitialState(App_GetAirClosedState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_OPEN_CHOICE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetAirOpenState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-3, FSM-4
TEST_F(FsmStateMachineTest, check_mapped_pedal_percentage_can_signals_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // For the following tests we will select a secondary APPS encoder
        // value such that the difference between the primary and secondary APPS
        // is within 10%. This prevents the APPS has disagreement callback from
        // being triggered

        // Normal range
        get_papps_encoder_counter_fake.return_val = PAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
        get_sapps_encoder_counter_fake.return_val = SAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));

        // Underflow range
        // Decrement fake_value by 1 to ensure that the encoder counter value is
        // within the lower deadzone
        get_papps_encoder_counter_fake.return_val = PAPPS_ENCODER_FULLY_PRESSED_VALUE * PERCENT_DEFLECTION - 1;
        get_sapps_encoder_counter_fake.return_val = SAPPS_ENCODER_FULLY_PRESSED_VALUE * PERCENT_DEFLECTION - 1;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));

        // Overflow range
        // Increment fake_value by 1 to ensure that the encoder counter value is
        // within the upper deadzone
        get_papps_encoder_counter_fake.return_val = PAPPS_ENCODER_FULLY_PRESSED_VALUE * (1 - PERCENT_DEFLECTION) + 1;
        get_sapps_encoder_counter_fake.return_val = SAPPS_ENCODER_FULLY_PRESSED_VALUE * (1 - PERCENT_DEFLECTION) + 1;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(100, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    }
}

// FSM-3
TEST_F(FsmStateMachineTest, brake_is_actuated_sets_mapped_pedal_percentage_to_zero_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        RESET_FAKE(is_brake_actuated);

        // Start with a non-zero pedal position to avoid false positives. In
        // addition, the chosen primary brake pedal percentage will not trigger
        // the APPS and brake plausibility callback function
        get_papps_encoder_counter_fake.return_val =
            GetEncoderCounterFromPedalPercentage(5, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
        LetTimePass(state_machine, 10);
        ASSERT_EQ(5, round(App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface)));

        is_brake_actuated_fake.return_val = true;

        LetTimePass(state_machine, 9);
        ASSERT_EQ(5, round(App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface)));
        LetTimePass(state_machine, 1);
        ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    }
}

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
    get_papps_encoder_counter_fake.return_val = PAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    get_sapps_encoder_counter_fake.return_val = SAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    LetTimePass(state_machine, 10);

    is_papps_encoder_alarm_active_fake.return_val = true;
    LetTimePass(state_machine, 9);
    ASSERT_EQ(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
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
    get_papps_encoder_counter_fake.return_val = PAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    get_sapps_encoder_counter_fake.return_val = SAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    LetTimePass(state_machine, 10);

    is_sapps_encoder_alarm_active_fake.return_val = true;
    LetTimePass(state_machine, SAPPS_ENTRY_HIGH_MS - 1);
    ASSERT_EQ(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));
}

// FSM-16
TEST_F(FsmStateMachineTest, check_inactive_papps_and_sapps_alarm_clears_papps_motor_shutdown_can_tx_signal)
{
    // For the following test we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered
    get_papps_encoder_counter_fake.return_val     = PAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    get_sapps_encoder_counter_fake.return_val     = SAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    is_papps_encoder_alarm_active_fake.return_val = true;
    LetTimePass(state_machine, PAPPS_ENTRY_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    is_papps_encoder_alarm_active_fake.return_val = false;
    is_sapps_encoder_alarm_active_fake.return_val = true;
    LetTimePass(state_machine, PAPPS_EXIT_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    // Check the the motor shutdown fault is cleared when both primary and
    // secondary APPS alarms are inactive. In addition, check that mapped pedal
    //    // percentage is reset to 50% when the callback is no longer
    //    triggered.
    is_sapps_encoder_alarm_active_fake.return_val = false;
    LetTimePass(state_machine, PAPPS_EXIT_HIGH_MS - 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_EQ(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
}

// FSM_16
TEST_F(FsmStateMachineTest, check_inactive_papps_and_sapps_alarm_clears_sapps_motor_shutdown_can_tx_signal)
{
    // For the following test we will select a secondary APPS encoder
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered
    get_papps_encoder_counter_fake.return_val     = PAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    get_sapps_encoder_counter_fake.return_val     = SAPPS_ENCODER_FULLY_PRESSED_VALUE / 2.0;
    is_papps_encoder_alarm_active_fake.return_val = true;
    is_sapps_encoder_alarm_active_fake.return_val = true;
    LetTimePass(state_machine, SAPPS_ENTRY_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    is_papps_encoder_alarm_active_fake.return_val = true;
    is_sapps_encoder_alarm_active_fake.return_val = false;
    LetTimePass(state_machine, SAPPS_EXIT_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));

    // Check the the motor shutdown fault is cleared when both primary and
    // secondary APPS alarms are inactive. In addition, check that mapped pedal
    // percentage is reset to 50% when the callback is no longer triggered.
    is_papps_encoder_alarm_active_fake.return_val = false;
    LetTimePass(state_machine, SAPPS_EXIT_HIGH_MS - 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SAPPS_ALARM_IS_ACTIVE(can_tx_interface));
    ASSERT_EQ(50, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
}

// FSM-6
TEST_F(FsmStateMachineTest, papps_greater_than_apps_by_ten_percent_sets_motor_shutdown_can_tx_signal)
{
    CheckIfAppsDisagreementFaultIsSetWhenAppsHasDisagreement(
        get_papps_encoder_counter_fake.return_val, get_sapps_encoder_counter_fake.return_val,
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, SAPPS_ENCODER_FULLY_PRESSED_VALUE, APPS_ENTRY_HIGH_MS, APPS_EXIT_HIGH_MS,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
}

// FSM-6
TEST_F(FsmStateMachineTest, papps_greater_than_apps_by_five_percent_or_less_clears_motor_shutdown_can_tx_signal)
{
    CheckIfAppsDisagreementFaultIsClearedWhenAppsHasAgreement(
        get_papps_encoder_counter_fake.return_val, get_sapps_encoder_counter_fake.return_val,
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, SAPPS_ENCODER_FULLY_PRESSED_VALUE, APPS_ENTRY_HIGH_MS, APPS_EXIT_HIGH_MS,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
}

// FSM-6
TEST_F(FsmStateMachineTest, sapps_greater_than_sapps_by_ten_percent_sets_motor_shutdown_can_tx_signal)
{
    CheckIfAppsDisagreementFaultIsSetWhenAppsHasDisagreement(
        get_sapps_encoder_counter_fake.return_val, get_papps_encoder_counter_fake.return_val,
        SAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_FULLY_PRESSED_VALUE, APPS_ENTRY_HIGH_MS, APPS_EXIT_HIGH_MS,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
}

// FSM-6
TEST_F(FsmStateMachineTest, sapps_greater_than_papps_by_five_percent_or_less_clears_motor_shutdown_can_tx_signal)
{
    CheckIfAppsDisagreementFaultIsClearedWhenAppsHasAgreement(
        get_sapps_encoder_counter_fake.return_val, get_papps_encoder_counter_fake.return_val,
        SAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_FULLY_PRESSED_VALUE, APPS_ENTRY_HIGH_MS, APPS_EXIT_HIGH_MS,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_FALSE_CHOICE);
}

// FSM-7
TEST_F(
    FsmStateMachineTest,
    apps_is_greater_than_25_percent_and_brake_is_not_actuated_does_not_set_motor_shutdown_can_tx_signal)
{
    const float fake_encoder_value_threshold =
        GetEncoderCounterFromPedalPercentage(25, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
    is_brake_actuated_fake.return_val = false;

    // Check CAN signals when the mapped pedal percentage is less than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold - 1;
    get_sapps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(25, SAPPS_ENCODER_FULLY_PRESSED_VALUE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check CAN signals when the mapped pedal percentage is greater than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold + 1;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));
}

// FSM-7
TEST_F(FsmStateMachineTest, apps_is_greater_than_25_percent_and_brake_is_actuated_sets_motor_shutdown_can_tx_signal)
{
    const float fake_encoder_value_threshold =
        GetEncoderCounterFromPedalPercentage(25, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
    is_brake_actuated_fake.return_val = true;

    // Check CAN signals when the mapped pedal percentage is less than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold - 1;
    get_sapps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(25, SAPPS_ENCODER_FULLY_PRESSED_VALUE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check CAN signals when the mapped pedal percentage is greater than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold + 1;
    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    is_brake_actuated_fake.return_val = false;
    LetTimePass(state_machine, 100);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));
}

// FSM-7
TEST_F(FsmStateMachineTest, apps_is_less_than_5_percent_and_brake_is_actuated_clears_motor_shutdown_can_tx_signal)
{
    const float fake_encoder_value_threshold =
        GetEncoderCounterFromPedalPercentage(25, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
    is_brake_actuated_fake.return_val = true;

    // Check that the callback has not been triggered when the mapped pedal
    // percentage is less than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold - 1;
    get_sapps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(25, SAPPS_ENCODER_FULLY_PRESSED_VALUE);
    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check that the callback has been triggered when the mapped pedal
    // percentage is greater than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold + 1;
    LetTimePass(state_machine, APPS_AND_BRAKE_ENTRY_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check if the callback function is still active regardless of the brake
    // state
    is_brake_actuated_fake.return_val = true;
    LetTimePass(state_machine, 100);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Set the mapped pedal percentage to a value less than 5% to stop
    // triggering the callback function
    get_papps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(5, PAPPS_ENCODER_FULLY_PRESSED_VALUE) - 1;
    get_sapps_encoder_counter_fake.return_val = 0;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));
}

// FSM-7
TEST_F(FsmStateMachineTest, apps_is_less_than_5_percent_and_brake_is_not_actuated_clears_motor_shutdown_can_tx_signal)
{
    const float fake_encoder_value_threshold =
        GetEncoderCounterFromPedalPercentage(25, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
    is_brake_actuated_fake.return_val = true;

    // Check that the callback has not been triggered when the mapped pedal
    // percentage is less than 25%. We also select a secondary APPS encoder
    // value such the difference between the primary and secondary APPS is
    // within 10% to avoid triggering the APPS has disagreement callback
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold - 1;
    get_sapps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(25, SAPPS_ENCODER_FULLY_PRESSED_VALUE);
    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check that the callback has been triggered when the mapped pedal
    // percentage is greater than 25%
    get_papps_encoder_counter_fake.return_val = fake_encoder_value_threshold + 1;
    LetTimePass(state_machine, APPS_AND_BRAKE_ENTRY_HIGH_MS);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Check if the callback function is still active regardless of brake state
    is_brake_actuated_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface));
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    // Set the mapped pedal percentage to a value less than 5% to stop
    // triggering the callback function
    get_papps_encoder_counter_fake.return_val =
        GetEncoderCounterFromPedalPercentage(4, PAPPS_ENCODER_FULLY_PRESSED_VALUE);
    get_sapps_encoder_counter_fake.return_val = 0;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PLAUSIBILITY_CHECK_HAS_FAILED(can_tx_interface));

    ASSERT_EQ(4, round(App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx_interface)));
}

TEST_F(FsmStateMachineTest, primary_flow_rate_underflow_sets_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    get_primary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 999);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));
}

TEST_F(FsmStateMachineTest, primary_flow_rate_in_range_clears_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    get_primary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 1000);

    get_primary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));
}

TEST_F(FsmStateMachineTest, secondary_flow_rate_underflow_sets_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    get_secondary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 999);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));
}

TEST_F(FsmStateMachineTest, secondary_flow_rate_in_range_clears_motor_shutdown_can_tx_signal)
{
    // Flow rate lower threshold (L/min)
    const float flow_rate_threshold = 1.0f;

    get_secondary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::lowest());
    LetTimePass(state_machine, 1000);

    get_secondary_flow_rate_fake.return_val = std::nextafter(flow_rate_threshold, std::numeric_limits<float>::max());
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));

    LetTimePass(state_machine, 1000);
    ASSERT_EQ(
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_FALSE_CHOICE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW(can_tx_interface));
}

} // namespace StateMachineTest
