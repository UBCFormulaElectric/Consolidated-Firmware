#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
}

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

class DcmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        constexpr uint32_t DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_HEARTBEAT_BOARDS_TO_CHECK =
            BMS_HEARTBEAT_ONE_HOT;

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DCM_STARTUP,
            send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT);
        can_rx_interface  = App_CanRx_Create();
        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS,
            DEFAULT_HEARTBEAT_BOARDS_TO_CHECK, heartbeat_timeout_callback);
        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);
        world = App_DcmWorld_Create(
            can_tx_interface, can_rx_interface, heartbeat_monitor,
            rgb_led_sequence);

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
    }

    void TearDown() override
    {
        ASSERT_TRUE(world != NULL);
        App_DcmWorld_Destroy(world);
        world = NULL;

        ASSERT_TRUE(can_tx_interface != NULL);
        App_CanTx_Destroy(can_tx_interface);
        can_tx_interface = NULL;

        ASSERT_TRUE(can_rx_interface != NULL);
        App_CanRx_Destroy(can_rx_interface);
        can_rx_interface = NULL;

        ASSERT_TRUE(heartbeat_monitor != NULL);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);
        heartbeat_monitor = NULL;

        ASSERT_TRUE(rgb_led_sequence != NULL);
        App_SharedRgbLedSequence_Destroy(rgb_led_sequence);
        rgb_led_sequence = NULL;

        ASSERT_TRUE(state_machine != NULL);
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = NULL;
    }

    void SetInitialState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    struct World *            world;
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
};

TEST_F(
    DcmStateMachineTest,
    check_init_immediately_transitions_to_run_on_first_tick)
{
    // We need to tick twice, once to run the `Init` state, and once more
    // to have the state machine transition to the `Run` state.
    App_SharedStateMachine_Tick1kHz(state_machine);
    App_SharedStateMachine_Tick1kHz(state_machine);

    EXPECT_EQ(
        App_GetDriveState(),
        App_SharedStateMachine_GetCurrentState(state_machine));
}

TEST_F(DcmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(DcmStateMachineTest, check_fault_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetFaultState());

    EXPECT_EQ(
        CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}
