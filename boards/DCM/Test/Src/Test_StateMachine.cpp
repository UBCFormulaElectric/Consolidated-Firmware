#include <math.h>
#include "Test_Dcm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_WaitSignalDuration.h"
#include "configs/App_AccelerationThresholds.h"
#include "configs/App_TorqueRequestThresholds.h"
#include "configs/App_RegenThresholds.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_DCM_STARTUP, const struct CanMsgs_dcm_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT, const struct CanMsgs_dcm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(turn_on_brake_light);
FAKE_VOID_FUNC(turn_off_brake_light);
FAKE_VOID_FUNC(turn_on_buzzer);
FAKE_VOID_FUNC(turn_off_buzzer);
FAKE_VALUE_FUNC(float, get_acceleration_x);
FAKE_VALUE_FUNC(float, get_acceleration_y);
FAKE_VALUE_FUNC(float, get_acceleration_z);
FAKE_VOID_FUNC(turn_on_right_inverter);
FAKE_VOID_FUNC(turn_off_right_inverter);
FAKE_VOID_FUNC(turn_on_left_inverter);
FAKE_VOID_FUNC(turn_off_left_inverter);
FAKE_VALUE_FUNC(bool, is_right_inverter_on);
FAKE_VALUE_FUNC(bool, is_left_inverter_on);

class DcmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        can_tx_interface =
            App_CanTx_Create(send_non_periodic_msg_DCM_STARTUP, send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK,
            heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        brake_light = App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        buzzer = App_Buzzer_Create(turn_on_buzzer, turn_off_buzzer);

        imu = App_Imu_Create(
            get_acceleration_x, get_acceleration_y, get_acceleration_z, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);

        error_table = App_SharedErrorTable_Create();

        clock = App_SharedClock_Create();

        inverter_switches = App_InverterSwitches_Create(
            turn_on_right_inverter, turn_off_right_inverter, turn_on_left_inverter, turn_off_left_inverter,
            is_right_inverter_on, is_left_inverter_on);

        world = App_DcmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor, rgb_led_sequence, brake_light, buzzer, imu,
            error_table, clock, inverter_switches, App_BuzzerSignals_IsOn, App_BuzzerSignals_Callback);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_DCM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(get_acceleration_x);
        RESET_FAKE(get_acceleration_y);
        RESET_FAKE(get_acceleration_z);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
        RESET_FAKE(turn_on_right_inverter);
        RESET_FAKE(turn_off_right_inverter);
        RESET_FAKE(turn_on_left_inverter);
        RESET_FAKE(turn_off_left_inverter);
    }

    void TearDown() override
    {
        TearDownObject(world, App_DcmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(brake_light, App_BrakeLight_Destroy);
        TearDownObject(buzzer, App_Buzzer_Destroy);
        TearDownObject(imu, App_Imu_Destroy);
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(inverter_switches, App_InverterSwitches_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
            App_GetFaultState(),
        };
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_DcmWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        App_DcmWorld_UpdateWaitSignal(world, current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
    struct Imu *              imu;
    struct ErrorTable *       error_table;
    struct Clock *            clock;
    struct InverterSwitches * inverter_switches;
};

// DCM-5
TEST_F(DcmStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up)
{
    // Pull start switch down and back up, expect no transition
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);
    LetTimePass(state_machine, 10);
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Transition BMS to drive state, expect no transition
    App_CanRx_BMS_STATE_MACHINE_SetSignal_STATE(can_rx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Actuate brake pedal
    App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Pull start switch down and back up, expect init->drive transition
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);
    LetTimePass(state_machine, 10);
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, brake_light_control_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Brake = Not actuated
        // Regen = Not active
        // Expect brake light to be off
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
            can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), false);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 0);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Not actuated
        // Regen = Active (negative torque)
        // Expect brake light to be on
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
            can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(-1.0f));
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Not active
        // Expect brake light to be on
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
            can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 2);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Active (negative torque)
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
            can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(-1.0f));
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 3);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Manually reset the call count for the fake functions
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
    }
}

TEST_F(DcmStateMachineTest, rgb_led_sequence_in_all_states)
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

// DCM-16
/*TEST_F(DcmStateMachineTest, zero_torque_request_in_fault_state)
{
    SetInitialState(App_GetFaultState());

    // Start with a non-zero torque request to prevent false positive
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx_interface,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(1.0f));
    EXPECT_FLOAT_EQ(
        1.0f,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        1.0f,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));

    // Now tick the state machine and check torque request gets zeroed
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        0.0f,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        0.0f,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));
}*/

// DCM-14
TEST_F(DcmStateMachineTest, start_switch_off_transitions_drive_state_to_init_state)
{
    SetInitialState(App_GetDriveState());

    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_if_there_is_no_error)
{
    SetInitialState(App_GetFaultState());

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_if_there_is_only_non_critical_error)
{
    SetInitialState(App_GetFaultState());

    // Choose any non-critical fault, it doesn't have to come from DCM
    App_SharedErrorTable_SetError(error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, true);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_once_critical_errors_and_inverter_faults_cleared)
{
    SetInitialState(App_GetFaultState());

    // Set any critical error and introduce inverter faults, expect no
    // transition
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, true);
    App_CanRx_INVR_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVR(
        can_rx_interface, CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE);
    App_CanRx_INVL_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVL(
        can_rx_interface, CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Clear critical error, expect no transition
    App_SharedErrorTable_SetError(error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Clear right inverter fault, expect no transition
    App_CanRx_INVR_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVR(
        can_rx_interface, CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_MOTOR__RUNNING__STATE_CHOICE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Clear left inverter fault, expect fault->init transition
    App_CanRx_INVL_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVL(
        can_rx_interface, CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_MOTOR__RUNNING__STATE_CHOICE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-12
TEST_F(DcmStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        if (CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE == App_CanTx_GetPeriodicSignal_STATE(can_tx_interface))
        {
            // Turn the DIM start switch on to prevent state transitions in
            // the drive state.
            EXPECT_TRUE(App_BuzzerSignals_IsOn(world));
            App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(
                can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);

            LetTimePass(state_machine, BUZZER_ON_DURATION_MS - 1);
            EXPECT_TRUE(App_BuzzerSignals_IsOn(world));

            // Check that the buzzer has been turned off after waiting for
            // BUZZER_ON_DURATION_MS.
            LetTimePass(state_machine, 1);
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));

            // Check that the buzzer stays off for the next 100ms. It should
            // be enough time to give us confidence in the test without making
            // the test too slow.
            LetTimePass(state_machine, 100);
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));
        }
        else
        {
            // Check that the buzzer is not turned on in other states.
            EXPECT_FALSE(App_BuzzerSignals_IsOn(world));
        }
    }
}
// DCM-8
/*TEST_F(DcmStateMachineTest, regen_not_allowed_when_no_airs_closed)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(
        can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);

    App_CanRx_FSM_PEDAL_POSITION_SetSignal_MAPPED_PEDAL_PERCENTAGE(
        can_rx_interface, 60.0f);
    App_CanRx_DIM_REGEN_PADDLE_SetSignal_MAPPED_PADDLE_POSITION(
        can_rx_interface, 50.0f);

    // Wheel speed = 0 km/h, expect pedal percentage to map to torque request
    float expected_torque_request_value = 60.0f * 0.01f * MAX_TORQUE_REQUEST_NM;
    float value_over_threshold_wheel_speed = std::nextafter(
        REGEN_WHEEL_SPEED_THRESHOLD_KPH, std::numeric_limits<float>::max());

    // Set both wheel speeds over regen threshold
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_LEFT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_RIGHT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    LetTimePass(state_machine, 10);

    // Check that regen doesn't turn on when both AIRs are open
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_OPEN_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE);
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));
}*/

/*TEST_F(DcmStateMachineTest, regen_not_allowed_when_one_air_closed)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(
        can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);

    App_CanRx_FSM_PEDAL_POSITION_SetSignal_MAPPED_PEDAL_PERCENTAGE(
        can_rx_interface, 60.0f);
    App_CanRx_DIM_REGEN_PADDLE_SetSignal_MAPPED_PADDLE_POSITION(
        can_rx_interface, 50.0f);

    // Wheel speed = 0 km/h, expect pedal percentage to map to torque request
    float expected_torque_request_value = 60.0f * 0.01f * MAX_TORQUE_REQUEST_NM;
    float value_over_threshold_wheel_speed = std::nextafter(
        REGEN_WHEEL_SPEED_THRESHOLD_KPH, std::numeric_limits<float>::max());

    // Set both wheel speeds over regen threshold
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_LEFT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_RIGHT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);

    // Check that regen doesn't turn when negative AIR is open and positive AIR
    // is closed
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_OPEN_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));

    // Check that regen doesn't turn when positive AIR is open and negative AIR
    // is closed
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_OPEN_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));
}*/

// DCM-8, DCM-19
/*TEST_F(
    DcmStateMachineTest,
    regen_allowed_only_when_going_faster_than_5kph_and_both_airs_closed)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(
        can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);

    App_CanRx_FSM_PEDAL_POSITION_SetSignal_MAPPED_PEDAL_PERCENTAGE(
        can_rx_interface, 60.0f);
    App_CanRx_DIM_REGEN_PADDLE_SetSignal_MAPPED_PADDLE_POSITION(
        can_rx_interface, 50.0f);

    float expected_torque_request_value = 60.0f * 0.01f * MAX_TORQUE_REQUEST_NM;
    float expected_regen_request_value = -50.0f * 0.01f * MAX_TORQUE_REQUEST_NM;
    float value_over_threshold_wheel_speed = std::nextafter(
        REGEN_WHEEL_SPEED_THRESHOLD_KPH, std::numeric_limits<float>::max());

    // Close both AIRs
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_NEGATIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE);
    App_CanRx_BMS_AIR_STATES_SetSignal_AIR_POSITIVE(
        can_rx_interface, CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE);

    // Check that regen doesn't turn on when left wheel speed is below regen
    // threshold
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_LEFT_WHEEL_SPEED(
        can_rx_interface, REGEN_WHEEL_SPEED_THRESHOLD_KPH);
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_RIGHT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));

    // Check that regen doesn't turn on when right wheel speed is below regen
    // threshold
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_LEFT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_RIGHT_WHEEL_SPEED(
        can_rx_interface, REGEN_WHEEL_SPEED_THRESHOLD_KPH);
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_torque_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));

    // Check that regen turns on when both wheels speeds are over regen
    // threshold
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_LEFT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    App_CanRx_FSM_WHEEL_SPEED_SENSOR_SetSignal_RIGHT_WHEEL_SPEED(
        can_rx_interface, value_over_threshold_wheel_speed);
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        expected_regen_request_value,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        expected_regen_request_value,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));
}*/

// DCM-19
TEST_F(DcmStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);

    // Check that no torque requests are sent when the accelerator pedal is not
    // pressed
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(
        0.0f, App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
                  App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx_interface)));
    EXPECT_FLOAT_EQ(
        0.0f, App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
                  App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVR(can_tx_interface)));
}

TEST_F(DcmStateMachineTest, init_to_fault_state_on_left_inverter_fault)
{
    // Introduce left inverter fault, expect init->fault transition on next
    // 100 Hz tick
    App_CanRx_INVL_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVL(
        can_rx_interface, CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE);
    LetTimePass(state_machine, 9);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    LetTimePass(state_machine, 1);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, drive_to_fault_state_on_left_inverter_fault)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Introduce left inverter fault, expect transition to fault state
    App_CanRx_INVL_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVL(
        can_rx_interface, CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, init_to_fault_state_on_right_inverter_fault)
{
    // Introduce right inverter fault, expect transition to fault state on
    // next 100 Hz tick
    App_CanRx_INVR_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVR(
        can_rx_interface, CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE);
    LetTimePass(state_machine, 9);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    LetTimePass(state_machine, 1);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, drive_to_fault_state_on_right_inverter_fault)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Introduce right inverter fault, expect transition to fault state
    App_CanRx_INVR_INTERNAL_STATES_SetSignal_D1_VSM_STATE_INVR(
        can_rx_interface, CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-20
TEST_F(DcmStateMachineTest, init_to_fault_state_on_motor_shutdown_error)
{
    // Introduce motor shutdown error, expect transition to fault state on next
    // 100 Hz tick
    App_SharedErrorTable_SetError(error_table, BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, true);
    LetTimePass(state_machine, 9);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    LetTimePass(state_machine, 1);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-20
TEST_F(DcmStateMachineTest, drive_to_fault_state_on_motor_shutdown_error)
{
    SetInitialState(App_GetDriveState());

    // Turn the DIM start switch on to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Introduce motor shutdown error, expect transition to fault state
    App_SharedErrorTable_SetError(error_table, BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, true);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, broadcast_inverter_switch_status_over_can)
{
    // Both inverter switches are off initially
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_SWITCH(can_tx_interface),
        CANMSGS_DCM_INVERTER_SWITCHES_RIGHT_INVERTER_SWITCH_OFF_CHOICE);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_SWITCH(can_tx_interface),
        CANMSGS_DCM_INVERTER_SWITCHES_LEFT_INVERTER_SWITCH_OFF_CHOICE);

    // Inverter switches turned on
    is_right_inverter_on_fake.return_val = true;
    is_left_inverter_on_fake.return_val  = true;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_SWITCH(can_tx_interface),
        CANMSGS_DCM_INVERTER_SWITCHES_RIGHT_INVERTER_SWITCH_ON_CHOICE);
    ASSERT_EQ(
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_SWITCH(can_tx_interface),
        CANMSGS_DCM_INVERTER_SWITCHES_LEFT_INVERTER_SWITCH_ON_CHOICE);
}
} // namespace StateMachineTest
