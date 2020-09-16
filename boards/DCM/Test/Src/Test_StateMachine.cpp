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
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_STARTUP,
    const struct CanMsgs_dcm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_dcm_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(turn_on_brake_light);
FAKE_VOID_FUNC(turn_off_brake_light);
FAKE_VOID_FUNC(turn_on_buzzer);
FAKE_VOID_FUNC(turn_off_buzzer);

class DcmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        brake_light =
            App_BrakeLight_Create(turn_on_brake_light, turn_off_brake_light);

        buzzer = App_Buzzer_Create(turn_on_buzzer, turn_off_buzzer);

        error_table = App_SharedErrorTable_Create();

        clock = App_SharedClock_Create();

        world = App_DcmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            rgb_led_sequence, brake_light, buzzer, error_table, clock,

            App_BuzzerSignals_IsOn, App_BuzzerSignals_Callback);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        RESET_FAKE(send_non_periodic_msg_DCM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(turn_on_brake_light);
        RESET_FAKE(turn_off_brake_light);
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
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
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
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetDriveState(),
            App_GetFaultState(),
        };
    }

    void UpdateClock(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_DcmWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) override
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
    struct ErrorTable *       error_table;
    struct Clock *            clock;
};

TEST_F(
    DcmStateMachineTest,
    check_init_immediately_transitions_to_run_on_first_tick)
{
    LetTimePass(state_machine, 10);

    EXPECT_EQ(
        App_GetDriveState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

// DCM-21
TEST_F(DcmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, brake_light_control_in_all_states)
{
    for (const auto &state : GetAllStates())
    {
        SetInitialState(state);

        // Brake = Not actuated
        // Regen = Not active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(
            can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), false);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 0);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Not actuated
        // Regen = Active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(
            can_rx_interface, false);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(
            can_tx_interface,
            std::nextafter(0.0f, std::numeric_limits<float>::lowest()));
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 1);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Not active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_BrakeLight_IsTurnedOn(brake_light), true);
        ASSERT_EQ(turn_on_brake_light_fake.call_count, 2);
        ASSERT_EQ(turn_off_brake_light_fake.call_count, 1);

        // Brake = Actuated
        // Regen = Active
        App_CanRx_FSM_BRAKE_SetSignal_BRAKE_IS_ACTUATED(can_rx_interface, true);
        App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(
            can_tx_interface,
            std::nextafter(0.0f, std::numeric_limits<float>::lowest()));
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
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
        }
    }
}

// DCM-4
TEST_F(DcmStateMachineTest, zero_torque_request_in_init_state)
{
    // Start with a non-zero torque request to prevent false positive
    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 1.0f);
    ASSERT_EQ(
        1.0f, App_CanTx_GetPeriodicSignal_TORQUE_REQUEST(can_tx_interface));

    // Now tick the state machine and check torque request gets zeroed
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        0.0f, App_CanTx_GetPeriodicSignal_TORQUE_REQUEST(can_tx_interface));
}

// DCM-16
TEST_F(DcmStateMachineTest, zero_torque_request_in_fault_state)
{
    SetInitialState(App_GetFaultState());

    // Start with a non-zero torque request to prevent false positive
    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 1.0f);

    // Now tick the state machine and check torque request gets zeroed
    LetTimePass(state_machine, 10);
    ASSERT_EQ(
        0.0f, App_CanTx_GetPeriodicSignal_TORQUE_REQUEST(can_tx_interface));
}

// DCM-14
TEST_F(
    DcmStateMachineTest,
    start_switch_off_transitions_drive_state_to_init_state)
{
    SetInitialState(App_GetDriveState());

    App_CanRx_DIM_SWITCHES_SetSignal_START_SWITCH(
        can_rx_interface, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(
        App_GetInitState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(DcmStateMachineTest, exit_fault_state_if_there_is_no_error)
{
    SetInitialState(App_GetFaultState());

    LetTimePass(state_machine, 10);

    ASSERT_EQ(
        App_GetInitState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(
    DcmStateMachineTest,
    exit_fault_state_if_there_is_only_non_critical_error)
{
    SetInitialState(App_GetFaultState());

    // Choose any non-critical fault, it doesn't have to come from DCM
    App_SharedErrorTable_SetError(
        error_table, DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        true);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(
        App_GetInitState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-17
TEST_F(DcmStateMachineTest, stay_in_fault_state_if_there_is_any_critical_errors)
{
    SetInitialState(App_GetFaultState());

    // Choose any critical fault, it doesn't have to come from DCM
    App_SharedErrorTable_SetError(
        error_table, BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE,
        true);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(
        App_GetFaultState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

// DCM-12
TEST_F(
    DcmStateMachineTest,
    check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state)
{
    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        if (CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE ==
            App_CanTx_GetPeriodicSignal_STATE(can_tx_interface))
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

} // namespace StateMachineTest
