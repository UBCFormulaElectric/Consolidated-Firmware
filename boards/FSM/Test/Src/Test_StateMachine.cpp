#include <math.h>
#include "Test_Fsm.h"

extern "C"
{
#include "App_SharedBinaryStatus.h"
#include "App_InRangeCheck.h"
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_FlowRateThresholds.h"
#include "configs/App_WheelSpeedThresholds.h"
#include "configs/App_SteeringAngleThresholds.h"
#include "configs/App_BrakePressureThresholds.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_STARTUP,
    const struct CanMsgs_fsm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_AIR_SHUTDOWN,
    const struct CanMsgs_fsm_air_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_fsm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);

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
FAKE_VALUE_FUNC(bool, is_brake_open_or_short_circuited);

class FsmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_FSM_STARTUP,
            send_non_periodic_msg_FSM_WATCHDOG_TIMEOUT,
            send_non_periodic_msg_FSM_AIR_SHUTDOWN);

        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        primary_flow_rate_in_range_check = App_InRangeCheck_Create(
            get_primary_flow_rate, MIN_PRIMARY_FLOW_RATE_L_PER_MIN,
            MAX_PRIMARY_FLOW_RATE_L_PER_MIN);

        secondary_flow_rate_in_range_check = App_InRangeCheck_Create(
            get_secondary_flow_rate, MIN_SECONDARY_FLOW_RATE_L_PER_MIN,
            MAX_SECONDARY_FLOW_RATE_L_PER_MIN);

        left_wheel_speed_in_range_check = App_InRangeCheck_Create(
            get_left_wheel_speed, MIN_LEFT_WHEEL_SPEED_KPH,
            MAX_LEFT_WHEEL_SPEED_KPH);

        right_wheel_speed_in_range_check = App_InRangeCheck_Create(
            get_right_wheel_speed, MIN_RIGHT_WHEEL_SPEED_KPH,
            MAX_RIGHT_WHEEL_SPEED_KPH);

        steering_angle_in_range_check = App_InRangeCheck_Create(
            get_steering_angle, MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);

        brake_pressure_in_range_check = App_InRangeCheck_Create(
            get_brake_pressure, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);

        brake_actuation_status =
            App_SharedBinaryStatus_Create(is_brake_actuated);

        brake_oc_sc_status =
            App_SharedBinaryStatus_Create(is_brake_open_or_short_circuited);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        world = App_FsmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            primary_flow_rate_in_range_check,
            secondary_flow_rate_in_range_check, left_wheel_speed_in_range_check,
            right_wheel_speed_in_range_check, steering_angle_in_range_check,
            brake_pressure_in_range_check, brake_actuation_status,
            brake_oc_sc_status, rgb_led_sequence);

        // Default to starting the state machine in the `AIR_OPEN` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetAirOpenState());

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
        RESET_FAKE(is_brake_open_or_short_circuited);
    }

    void TearDown() override
    {
        TearDownObject(world, App_FsmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(
            left_wheel_speed_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            right_wheel_speed_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            primary_flow_rate_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            secondary_flow_rate_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(steering_angle_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(brake_pressure_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(brake_actuation_status, App_SharedBinaryStatus_Destroy);
        TearDownObject(brake_oc_sc_status, App_SharedBinaryStatus_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{ App_GetAirOpenState(),
                                                  App_GetAirClosedState() };
    }

    void CheckInRangeCanSignalsInAllStates(
        float  min_value,
        float  max_value,
        float &fake_value,
        float (*value_can_signal_getter)(const struct FsmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct FsmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal range
            fake_value = (min_value + max_value) / 2;
            App_SharedStateMachine_Tick100Hz(state_machine);
            ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
            ASSERT_EQ(
                ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Underflow range
            fake_value =
                std::nextafter(min_value, std::numeric_limits<float>::lowest());
            App_SharedStateMachine_Tick100Hz(state_machine);
            ASSERT_EQ(
                underflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));

            // Overflow range
            fake_value =
                std::nextafter(max_value, std::numeric_limits<float>::max());
            App_SharedStateMachine_Tick100Hz(state_machine);
            ASSERT_EQ(
                overflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    void CheckBinaryStatusCanSignalsInAllStates(
        bool &fake_value,
        uint8_t (*can_signal_getter)(const struct FsmCanTxInterface *),
        uint8_t on_choice,
        uint8_t off_choice)
    {
        for (const auto &state : GetAllStates())
        {
            SetInitialState(state);

            fake_value = true;
            App_SharedStateMachine_Tick100Hz(state_machine);
            ASSERT_EQ(on_choice, can_signal_getter(can_tx_interface));

            fake_value = false;
            App_SharedStateMachine_Tick100Hz(state_machine);
            ASSERT_EQ(off_choice, can_signal_getter(can_tx_interface));
        }
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
    struct InRangeCheck *     brake_pressure_in_range_check;
    struct BinaryStatus *     brake_actuation_status;
    struct BinaryStatus *     brake_oc_sc_status;
    struct RgbLedSequence *   rgb_led_sequence;
};

// FSM-10
TEST_F(FsmStateMachineTest, check_air_open_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirOpenState());

    EXPECT_EQ(
        CANMSGS_FSM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// FSM-10
TEST_F(FsmStateMachineTest, check_air_closed_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirClosedState());

    EXPECT_EQ(
        CANMSGS_FSM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// FSM-9
TEST_F(FsmStateMachineTest, check_left_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH,
        get_left_wheel_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED,
        App_CanTx_GetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-9
TEST_F(FsmStateMachineTest, check_right_wheel_speed_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH,
        get_right_wheel_speed_fake.return_val,
        App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED,
        App_CanTx_GetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-14
TEST_F(FsmStateMachineTest, check_primary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_PRIMARY_FLOW_RATE_L_PER_MIN, MAX_PRIMARY_FLOW_RATE_L_PER_MIN,
        get_primary_flow_rate_fake.return_val,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE,
        App_CanTx_GetPeriodicSignal_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-14
TEST_F(FsmStateMachineTest, check_secondary_flow_rate_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_SECONDARY_FLOW_RATE_L_PER_MIN, MAX_SECONDARY_FLOW_RATE_L_PER_MIN,
        get_secondary_flow_rate_fake.return_val,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE,
        App_CanTx_GetPeriodicSignal_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_SECONDARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-8
TEST_F(FsmStateMachineTest, check_steering_angle_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG,
        get_steering_angle_fake.return_val,
        App_CanTx_GetPeriodicSignal_STEERING_ANGLE,
        App_CanTx_GetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_PRIMARY_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(FsmStateMachineTest, check_brake_pressure_can_signals_in_all_states)
{
    CheckInRangeCanSignalsInAllStates(
        MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI,
        get_brake_pressure_fake.return_val,
        App_CanTx_GetPeriodicSignal_BRAKE_PRESSURE,
        App_CanTx_GetPeriodicSignal_BRAKE_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

// FSM-18
TEST_F(
    FsmStateMachineTest,
    check_brake_pressure_actuation_can_signal_in_all_states)
{
    CheckBinaryStatusCanSignalsInAllStates(
        is_brake_actuated_fake.return_val,
        App_CanTx_GetPeriodicSignal_BRAKE_IS_ACTUATED,
        CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE,
        CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_FALSE_CHOICE);
}

TEST_F(
    FsmStateMachineTest,
    check_brake_open_or_short_circuited_can_signal_in_all_states)
{
    CheckBinaryStatusCanSignalsInAllStates(
        is_brake_open_or_short_circuited_fake.return_val,
        App_CanTx_GetPeriodicSignal_BRAKE_IS_OC_SC,
        CANMSGS_FSM_BRAKE_OC_SC_BRAKE_IS_OC_SC_TRUE_CHOICE,
        CANMSGS_FSM_BRAKE_OC_SC_BRAKE_IS_OC_SC_FALSE_CHOICE);
}

TEST_F(FsmStateMachineTest, rgb_led_sequence_in_all_states)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count,
                                    &turn_on_green_led_fake.call_count,
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
            App_SharedStateMachine_Tick1Hz(state_machine);
            ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
        }
    }
}

// FSM-12
TEST_F(
    FsmStateMachineTest,
    exit_air_open_state_when_air_positive_and_air_negative_are_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    App_SharedStateMachine_Tick100Hz(state_machine);

    ASSERT_EQ(
        App_GetAirClosedState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-12
TEST_F(
    FsmStateMachineTest,
    stay_in_air_open_state_if_only_air_positive_is_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);
    App_SharedStateMachine_Tick100Hz(state_machine);

    ASSERT_EQ(
        App_GetAirOpenState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-12
TEST_F(
    FsmStateMachineTest,
    stay_in_air_open_state_if_only_air_negative_is_closed)
{
    SetInitialState(App_GetAirOpenState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    App_SharedStateMachine_Tick100Hz(state_machine);

    ASSERT_EQ(
        App_GetAirOpenState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-15
TEST_F(FsmStateMachineTest, exit_air_closed_state_when_air_positive_is_opened)
{
    SetInitialState(App_GetAirClosedState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE);
    App_SharedStateMachine_Tick100Hz(state_machine);

    ASSERT_EQ(
        App_GetAirOpenState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// FSM-15
TEST_F(FsmStateMachineTest, exit_air_closed_state_when_air_negative_is_opened)
{
    SetInitialState(App_GetAirClosedState());

    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_OPEN_CHOICE);
    App_SharedStateMachine_Tick100Hz(state_machine);

    ASSERT_EQ(
        App_GetAirOpenState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

} // namespace StateMachineTest
