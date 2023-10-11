#include <math.h>
#include "Test_Dcm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_WaitSignalDuration.h"
#include "configs/App_AccelerationThresholds.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"
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
FAKE_VOID_FUNC(handle_imu_logs);
FAKE_VALUE_FUNC(uint32_t, get_imu_timestamp);
FAKE_VALUE_FUNC(uint16_t, get_imu_general_status);
FAKE_VALUE_FUNC(uint32_t, get_imu_com_status);
FAKE_VALUE_FUNC(float, get_imu_output, EllipseImuOutput);

class DcmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();

        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        brake_light = App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        buzzer = App_Buzzer_Create(turn_on_buzzer, turn_off_buzzer);

        imu = App_Imu_Create(
            get_acceleration_x, get_acceleration_y, get_acceleration_z, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);

        App_EllipseImu_Init(
            handle_imu_logs, get_imu_timestamp, get_imu_general_status, get_imu_com_status, get_imu_output);

        clock = App_SharedClock_Create();

        world = App_DcmWorld_Create(
            heartbeat_monitor, brake_light, buzzer, imu, clock, App_BuzzerSignals_IsOn, App_BuzzerSignals_Callback);

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
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(brake_light, App_BrakeLight_Destroy);
        TearDownObject(buzzer, App_Buzzer_Destroy);
        TearDownObject(imu, App_Imu_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
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
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
    struct Imu *              imu;
    struct Clock *            clock;
};

// DCM-5
TEST_F(DcmStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up)
{
    // Pull start switch down and back up, expect no transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Transition BMS to drive state, expect no transition
    App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Actuate brake pedal
    App_CanRx_FSM_IsActuated_Update(true);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // Pull start switch down and back up, expect init->drive transition
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-21
TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());
    EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-21
TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-21
TEST_F(DcmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-16
TEST_F(DcmStateMachineTest, zero_torque_request_in_fault_state)
{
    SetInitialState(App_GetFaultState());

    // Start with a non-zero torque request to prevent false positive
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(1.0f);

    EXPECT_FLOAT_EQ(1.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());

    // Now tick the state machine and check torque request gets zeroed
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}

// DCM-14
TEST_F(DcmStateMachineTest, start_switch_off_transitions_drive_state_to_init_state)
{
    SetInitialState(App_GetDriveState());
    App_CanRx_DIM_StartSwitch_Update(SWITCH_OFF);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_if_there_is_no_error)
{
    SetInitialState(App_GetFaultState());

    LetTimePass(state_machine, 10);

    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_if_there_is_only_warning)
{
    SetInitialState(App_GetFaultState());

    // Choose any warning, it doesn't have to come from DCM

    LetTimePass(state_machine, 10);

    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_once_critical_errors_and_inverter_faults_cleared)
{
    SetInitialState(App_GetFaultState());

    // Set any critical error and introduce inverter faults, expect no
    // transition

    App_CanRx_INVR_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    App_CanRx_INVL_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());

    // Clear critical error, expect no transition
    LetTimePass(state_machine, 10);
    ASSERT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());

    // Clear right inverter fault, expect no transition
    App_CanRx_INVR_VsmState_Update(INVERTER_VSM_MOTOR_RUNNING_STATE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());

    // Clear left inverter fault, expect fault->init transition
    App_CanRx_INVL_VsmState_Update(INVERTER_VSM_MOTOR_RUNNING_STATE);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
}

// DCM-12
TEST_F(DcmStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        if (DCM_DRIVE_STATE == App_CanTx_DCM_CurrentState_Get())
        {
            // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
            // the drive state.
            App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
            App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);

            EXPECT_TRUE(App_BuzzerSignals_IsOn(world));

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
    App_CanRx_DIM_SetSignal_START_SWITCH(
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
    App_CanRx_DIM_SetSignal_START_SWITCH(
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
    App_CanRx_DIM_SetSignal_START_SWITCH(
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

    // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);

    // Check that no torque requests are sent when the accelerator pedal is not
    // pressed
    LetTimePass(state_machine, 10);
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
    EXPECT_FLOAT_EQ(0.0f, App_CanTx_DCM_RightInverterTorqueCommand_Get());
}

TEST_F(DcmStateMachineTest, init_to_fault_state_on_left_inverter_fault)
{
    // Introduce left inverter fault, expect init->fault transition on next
    // 100 Hz tick
    App_CanRx_INVL_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(state_machine, 9);
    EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());
    LetTimePass(state_machine, 1);
    EXPECT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());
}

TEST_F(DcmStateMachineTest, drive_to_fault_state_on_left_inverter_fault)
{
    SetInitialState(App_GetDriveState());

    // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_CurrentState_Get());

    // Introduce left inverter fault, expect transition to fault state
    App_CanRx_INVL_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());
}

TEST_F(DcmStateMachineTest, init_to_fault_state_on_right_inverter_fault)
{
    // Introduce right inverter fault, expect transition to fault state on
    // next 100 Hz tick
    App_CanRx_INVR_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(state_machine, 9);
    EXPECT_EQ(DCM_INIT_STATE, App_CanTx_DCM_CurrentState_Get());

    LetTimePass(state_machine, 1);
    EXPECT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());
}

TEST_F(DcmStateMachineTest, drive_to_fault_state_on_right_inverter_fault)
{
    SetInitialState(App_GetDriveState());

    // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
    // the drive state.
    App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
    App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_DRIVE_STATE, App_CanTx_DCM_CurrentState_Get());

    // Introduce right inverter fault, expect transition to fault state
    App_CanRx_INVR_VsmState_Update(INVERTER_VSM_BLINK_FAULT_CODE_STATE);
    LetTimePass(state_machine, 10);
    EXPECT_EQ(DCM_FAULT_STATE, App_CanTx_DCM_CurrentState_Get());
}

TEST_F(DcmStateMachineTest, minimum_torque_request_transmitted_in_drive_state)
{
    struct
    {
        float bms_available_power;
        float motor_speed;
        int   pedal_percentage;
        float fsm_max_torque_request;
        float expected_torque_request;
    } test_params[4] = {
        {
            // Limited by FSM max torque request
            .bms_available_power     = 78e3f,
            .motor_speed             = 0.0f,
            .pedal_percentage        = 100,
            .fsm_max_torque_request  = 30,
            .expected_torque_request = 30,
        },
        {
            // Limited by BMS available power and motor speed
            .bms_available_power     = 10e3f,
            .motor_speed             = 1000.0f,
            .pedal_percentage        = 100,
            .fsm_max_torque_request  = MAX_TORQUE_REQUEST_NM,
            .expected_torque_request = 10e3f * 0.8 / (2 * RPM_TO_RADS(1000.0f)),
        },
        {
            // Limited by accelerator pedal percentage
            .bms_available_power     = 78e3f,
            .motor_speed             = 0.0f,
            .pedal_percentage        = 50,
            .fsm_max_torque_request  = MAX_TORQUE_REQUEST_NM,
            .expected_torque_request = 0.5f * MAX_TORQUE_REQUEST_NM,
        },
        {
            // Limited by max torque request (90Nm)
            .bms_available_power     = 78e3f,
            .motor_speed             = 1.0f, // Small motor speed allows for very high BMS torque limit (>>90Nm)
            .pedal_percentage        = 100,
            .fsm_max_torque_request  = MAX_TORQUE_REQUEST_NM + 20, // Not allowed to happen nominally
            .expected_torque_request = MAX_TORQUE_REQUEST_NM,
        },
    };

    for (int i = 0; i < 4; i++)
    {
        TearDown();
        SetUp();
        SetInitialState(App_GetDriveState());

        // Set the DIM start switch to on, and the BMS to drive state, to prevent state transitions in
        // the drive state.
        App_CanRx_DIM_StartSwitch_Update(SWITCH_ON);
        App_CanRx_BMS_CurrentState_Update(BMS_DRIVE_STATE);

        // Set inital conditions
        App_CanRx_BMS_AvailablePower_Update(test_params[i].bms_available_power);
        App_CanRx_INVR_MotorSpeed_Update(test_params[i].motor_speed);
        App_CanRx_INVL_MotorSpeed_Update(test_params[i].motor_speed);
        App_CanRx_FSM_PappsMappedPedalPercentage_Update(test_params[i].pedal_percentage);
        App_CanRx_FSM_TorqueLimit_Update(test_params[i].fsm_max_torque_request);

        // Confirm expected torque request is set
        LetTimePass(state_machine, 10);
        EXPECT_FLOAT_EQ(test_params[i].expected_torque_request, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
        EXPECT_FLOAT_EQ(test_params[i].expected_torque_request, App_CanTx_DCM_RightInverterTorqueCommand_Get());

        LetTimePass(state_machine, 1000);
        EXPECT_FLOAT_EQ(test_params[i].expected_torque_request, App_CanTx_DCM_LeftInverterTorqueCommand_Get());
        EXPECT_FLOAT_EQ(test_params[i].expected_torque_request, App_CanTx_DCM_RightInverterTorqueCommand_Get());
    }
}

} // namespace StateMachineTest
