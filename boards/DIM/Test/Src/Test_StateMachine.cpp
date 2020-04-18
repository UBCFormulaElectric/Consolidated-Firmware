#include "Test_SevenSegDisplays.h"
#include "Test_StateMachine.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
}

DEFINE_FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_STARTUP,
    struct CanMsgs_dim_startup_t *);
DEFINE_FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT,
    struct CanMsgs_dim_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);

class DimStateMachineTest : public DimTest
{
  protected:
    void SetUp() override
    {
        constexpr uint32_t DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_HEARTBEAT_BOARDS_TO_CHECK =
            BMS_HEARTBEAT_ONE_HOT;

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DIM_STARTUP,
            send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT);
        can_rx_interface       = App_CanRx_Create();
        left_seven_seg_display = App_SevenSegDisplay_Create(set_left_hex_digit);
        middle_seven_seg_display =
            App_SevenSegDisplay_Create(set_middle_hex_digit);
        right_seven_seg_display =
            App_SevenSegDisplay_Create(set_right_hex_digit);
        seven_seg_displays = App_SevenSegDisplays_Create(
            left_seven_seg_display, middle_seven_seg_display,
            right_seven_seg_display);
        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS,
            DEFAULT_HEARTBEAT_BOARDS_TO_CHECK, heartbeat_timeout_callback);
        world = App_DimWorld_Create(
            can_tx_interface, can_rx_interface, seven_seg_displays,
            heartbeat_monitor);

        // Default to starting the state machine in the `Drive` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_DIM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
    }

    void SetInitalState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    void TearDown() override
    {
        ASSERT_TRUE(world != NULL);
        ASSERT_TRUE(can_tx_interface != NULL);
        ASSERT_TRUE(can_rx_interface != NULL);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_TRUE(heartbeat_monitor != NULL);

        App_DimWorld_Destroy(world);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_SharedStateMachine_Destroy(state_machine);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);

        world             = NULL;
        can_tx_interface  = NULL;
        can_rx_interface  = NULL;
        state_machine     = NULL;
        heartbeat_monitor = NULL;
    }

    struct World *            world;
    struct DimCanTxInterface *can_tx_interface;
    struct DimCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
    struct SevenSegDisplay *  left_seven_seg_display;
    struct SevenSegDisplay *  middle_seven_seg_display;
    struct SevenSegDisplay *  right_seven_seg_display;
    struct SevenSegDisplays * seven_seg_displays;
    struct HeartbeatMonitor * heartbeat_monitor;
};

TEST_F(DimStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitalState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}
