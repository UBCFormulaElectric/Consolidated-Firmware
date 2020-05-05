#include "Test_Dim.h"
#include "Test_SevenSegDisplays.h"
#include "Test_RegenPaddle.h"
#include "Test_RotarySwitch.h"
#include "Test_Led.h"
#include "Test_BinarySwitch.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "states/App_DriveState.h"
#include "App_SharedRgbLedSequence.h"
#include "App_Led.h"
#include "App_CanMsgs.h"
#include "App_Switches.h"
}

FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_STARTUP,
    const struct CanMsgs_dim_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_dim_watchdog_timeout_t *);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

FAKE_VALUE_FUNC(uint32_t, get_drive_mode_switch_position);

FAKE_VALUE_FUNC(bool, start_switch_is_turned_on);
FAKE_VALUE_FUNC(bool, traction_control_switch_is_turned_on);
FAKE_VALUE_FUNC(bool, torque_vectoring_switch_is_turned_on);

FAKE_VOID_FUNC(turn_on_imd_led);
FAKE_VOID_FUNC(turn_off_imd_led);
FAKE_VOID_FUNC(turn_on_bspd_led);
FAKE_VOID_FUNC(turn_off_bspd_led);

class DimStateMachineTest : public SevenSegDisplaysTest,
                            public RegenPaddleTest,
                            public RotarySwitchTest,
                            public LedTest,
                            public BinarySwitchTest
{
  protected:
    void SetUp() override
    {
        SevenSegDisplaysTest::SetUp();
        RegenPaddleTest::SetUp();

        constexpr uint32_t DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_HEARTBEAT_BOARDS_TO_CHECK =
            BMS_HEARTBEAT_ONE_HOT;

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_DIM_STARTUP,
            send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT);
        can_rx_interface = App_CanRx_Create();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS,
            DEFAULT_HEARTBEAT_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        drive_mode_switch = App_RotarySwitch_Create(
            get_drive_mode_switch_position, NUM_DRIVE_MODE_SWITCH_POSITIONS);

        imd_led = App_Led_Create(turn_on_imd_led, turn_off_imd_led);

        bspd_led = App_Led_Create(turn_on_bspd_led, turn_off_bspd_led);

        start_switch = App_BinarySwitch_Create(start_switch_is_turned_on);

        traction_control_switch =
            App_BinarySwitch_Create(traction_control_switch_is_turned_on);

        torque_vectoring_switch =
            App_BinarySwitch_Create(torque_vectoring_switch_is_turned_on);

        world = App_DimWorld_Create(
            can_tx_interface, can_rx_interface, seven_seg_displays,
            heartbeat_monitor, regen_paddle, rgb_led_sequence,
            drive_mode_switch, imd_led, bspd_led, start_switch,
            traction_control_switch, torque_vectoring_switch);

        // Default to starting the state machine in the `Drive` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_DIM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(get_drive_mode_switch_position);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(turn_on_imd_led);
        RESET_FAKE(turn_off_imd_led);
        RESET_FAKE(turn_on_bspd_led);
        RESET_FAKE(turn_off_bspd_led);
        RESET_FAKE(start_switch_is_turned_on);
        RESET_FAKE(traction_control_switch_is_turned_on);
        RESET_FAKE(torque_vectoring_switch_is_turned_on);
    }

    void TearDown() override
    {
        ASSERT_TRUE(world != NULL);
        App_DimWorld_Destroy(world);
        world = NULL;

        ASSERT_TRUE(can_tx_interface != NULL);
        App_CanTx_Destroy(can_tx_interface);
        can_tx_interface = NULL;

        ASSERT_TRUE(can_rx_interface != NULL);
        App_CanRx_Destroy(can_rx_interface);
        can_rx_interface = NULL;

        SevenSegDisplaysTest::TearDown();
        RegenPaddleTest::TearDown();

        ASSERT_TRUE(heartbeat_monitor != NULL);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);
        heartbeat_monitor = NULL;

        ASSERT_TRUE(rgb_led_sequence != NULL);
        App_SharedRgbLedSequence_Destroy(rgb_led_sequence);
        rgb_led_sequence = NULL;

        RotarySwitchTest::TearDownRotarySwitch(drive_mode_switch);
        LedTest::TearDownLed(imd_led);
        LedTest::TearDownLed(bspd_led);
        BinarySwitchTest::TearDownBinarySwitch(start_switch);
        BinarySwitchTest::TearDownBinarySwitch(traction_control_switch);
        BinarySwitchTest::TearDownBinarySwitch(torque_vectoring_switch);

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
    struct DimCanTxInterface *can_tx_interface;
    struct DimCanRxInterface *can_rx_interface;
    struct StateMachine *     state_machine;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct Led *              imd_led;
    struct Led *              bspd_led;
    struct BinarySwitch *     start_switch;
    struct BinarySwitch *     traction_control_switch;
    struct BinarySwitch *     torque_vectoring_switch;
    struct RotarySwitch *     drive_mode_switch;
};

TEST_F(DimStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(
        CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_7_seg_displays_show_state_of_charge_in_drive_state)
{
    App_CanRx_BMS_STATE_OF_CHARGE_SetSignal_STATE_OF_CHARGE(
        can_rx_interface, 0.0f);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[0].value);

    App_CanRx_BMS_STATE_OF_CHARGE_SetSignal_STATE_OF_CHARGE(
        can_rx_interface, 50.0f);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(5, set_middle_hex_digit_fake.arg0_history[1].value);

    App_CanRx_BMS_STATE_OF_CHARGE_SetSignal_STATE_OF_CHARGE(
        can_rx_interface, 100.0f);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[2].value);
    ASSERT_EQ(0, set_middle_hex_digit_fake.arg0_history[2].value);
    ASSERT_EQ(1, set_right_hex_digit_fake.arg0_history[2].value);
}

TEST_F(
    DimStateMachineTest,
    check_raw_paddle_position_is_broadcasted_over_can_in_drive_state)
{
    get_raw_paddle_position_fake.return_val = 50;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        get_raw_paddle_position_fake.return_val,
        App_CanTx_GetPeriodicSignal_RAW_PADDLE_POSITION(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_mapped_paddle_position_is_broadcasted_over_can_in_drive_state)
{
    get_raw_paddle_position_fake.return_val = 50;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        get_raw_paddle_position_fake.return_val,
        App_CanTx_GetPeriodicSignal_MAPPED_PADDLE_POSITION(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_drive_mode_is_broadcasted_over_can_in_drive_state)
{
    get_drive_mode_switch_position_fake.return_val = 2;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_DRIVE_MODE_SWITCH_DRIVE_MODE_DRIVE_MODE_3_CHOICE,
        App_CanTx_GetPeriodicSignal_DRIVE_MODE(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    start_switch_is_turned_on_fake.return_val = false;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE,
        App_CanTx_GetPeriodicSignal_START_SWITCH(can_tx_interface));

    start_switch_is_turned_on_fake.return_val = true;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE,
        App_CanTx_GetPeriodicSignal_START_SWITCH(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_traction_control_switch_is_broadcasted_over_can_in_drive_state)
{
    traction_control_switch_is_turned_on_fake.return_val = false;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE,
        App_CanTx_GetPeriodicSignal_TRACTION_CONTROL_SWITCH(can_tx_interface));

    traction_control_switch_is_turned_on_fake.return_val = true;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE,
        App_CanTx_GetPeriodicSignal_TRACTION_CONTROL_SWITCH(can_tx_interface));
}

TEST_F(
    DimStateMachineTest,
    check_torque_vectoring_switch_is_broadcasted_over_can_in_drive_state)
{
    torque_vectoring_switch_is_turned_on_fake.return_val = false;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE,
        App_CanTx_GetPeriodicSignal_TORQUE_VECTORING_SWITCH(can_tx_interface));

    torque_vectoring_switch_is_turned_on_fake.return_val = true;
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE,
        App_CanTx_GetPeriodicSignal_TORQUE_VECTORING_SWITCH(can_tx_interface));
}

TEST_F(DimStateMachineTest, imd_led_control_in_drive_state)
{
    App_CanRx_BMS_IMD_SetSignal_OK_HS(
        can_rx_interface, CANMSGS_BMS_IMD_OK_HS_NO_FAULT_CHOICE);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(0, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_imd_led_fake.call_count);

    App_CanRx_BMS_IMD_SetSignal_OK_HS(
        can_rx_interface, CANMSGS_BMS_IMD_OK_HS_FAULT_CHOICE);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(1, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_imd_led_fake.call_count);

    App_CanRx_BMS_IMD_SetSignal_OK_HS(
        can_rx_interface, CANMSGS_BMS_IMD_OK_HS_NO_FAULT_CHOICE);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(1, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(2, turn_off_imd_led_fake.call_count);
}

TEST_F(DimStateMachineTest, bspd_led_control_in_drive_state)
{
    App_CanRx_FSM_ERRORS_SetSignal_BSPD_FAULT(can_rx_interface, false);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(0, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_bspd_led_fake.call_count);

    App_CanRx_FSM_ERRORS_SetSignal_BSPD_FAULT(can_rx_interface, true);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(1, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_bspd_led_fake.call_count);

    App_CanRx_FSM_ERRORS_SetSignal_BSPD_FAULT(can_rx_interface, false);
    App_SharedStateMachine_Tick(state_machine);
    ASSERT_EQ(1, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(2, turn_off_bspd_led_fake.call_count);
}
