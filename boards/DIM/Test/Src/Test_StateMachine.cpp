#include "Test_Dim.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_SharedRgbLedSequence.h"
#include "App_Led.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/App_DriveState.h"
#include "configs/App_RotarySwitchConfig.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_DIM_STARTUP, const struct CanMsgs_dim_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT, const struct CanMsgs_dim_watchdog_timeout_t *);

FAKE_VOID_FUNC(set_right_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_right_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_right_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(display_value_callback);

FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);

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

FAKE_VOID_FUNC(turn_bms_status_led_red);
FAKE_VOID_FUNC(turn_bms_status_led_green);
FAKE_VOID_FUNC(turn_bms_status_led_blue);
FAKE_VOID_FUNC(turn_off_bms_status_led);

FAKE_VOID_FUNC(turn_dcm_status_led_red);
FAKE_VOID_FUNC(turn_dcm_status_led_green);
FAKE_VOID_FUNC(turn_dcm_status_led_blue);
FAKE_VOID_FUNC(turn_off_dcm_status_led);

FAKE_VOID_FUNC(turn_dim_status_led_red);
FAKE_VOID_FUNC(turn_dim_status_led_green);
FAKE_VOID_FUNC(turn_dim_status_led_blue);
FAKE_VOID_FUNC(turn_off_dim_status_led);

FAKE_VOID_FUNC(turn_fsm_status_led_red);
FAKE_VOID_FUNC(turn_fsm_status_led_green);
FAKE_VOID_FUNC(turn_fsm_status_led_blue);
FAKE_VOID_FUNC(turn_off_fsm_status_led);

FAKE_VOID_FUNC(turn_pdm_status_led_red);
FAKE_VOID_FUNC(turn_pdm_status_led_green);
FAKE_VOID_FUNC(turn_pdm_status_led_blue);
FAKE_VOID_FUNC(turn_off_pdm_status_led);

class DimStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        left_l_seven_seg_display   = App_SevenSegDisplay_Create(set_left_l_hex_digit);
        left_m_seven_seg_display   = App_SevenSegDisplay_Create(set_left_m_hex_digit);
        left_r_seven_seg_display   = App_SevenSegDisplay_Create(set_left_r_hex_digit);
        middle_l_seven_seg_display = App_SevenSegDisplay_Create(set_middle_l_hex_digit);
        middle_m_seven_seg_display = App_SevenSegDisplay_Create(set_middle_m_hex_digit);
        middle_r_seven_seg_display = App_SevenSegDisplay_Create(set_middle_r_hex_digit);
        right_l_seven_seg_display  = App_SevenSegDisplay_Create(set_right_l_hex_digit);
        right_m_seven_seg_display  = App_SevenSegDisplay_Create(set_right_m_hex_digit);
        right_r_seven_seg_display  = App_SevenSegDisplay_Create(set_right_r_hex_digit);


        seven_seg_displays = App_SevenSegDisplays_Create(
            left_l_seven_seg_display, left_m_seven_seg_display, left_r_seven_seg_display, middle_l_seven_seg_display, middle_m_seven_seg_display, middle_r_seven_seg_display,
            right_l_seven_seg_display, right_m_seven_seg_display, right_r_seven_seg_display, display_value_callback);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        drive_mode_switch = App_RotarySwitch_Create(get_drive_mode_switch_position, NUM_DRIVE_MODE_SWITCH_POSITIONS);

        imd_led = App_Led_Create(turn_on_imd_led, turn_off_imd_led);

        bspd_led = App_Led_Create(turn_on_bspd_led, turn_off_bspd_led);

        start_switch = App_BinarySwitch_Create(start_switch_is_turned_on);

        traction_control_switch = App_BinarySwitch_Create(traction_control_switch_is_turned_on);

        torque_vectoring_switch = App_BinarySwitch_Create(torque_vectoring_switch_is_turned_on);

        bms_status_led = App_SharedRgbLed_Create(
            turn_bms_status_led_red, turn_bms_status_led_green, turn_bms_status_led_blue, turn_off_bms_status_led);

        dcm_status_led = App_SharedRgbLed_Create(
            turn_dcm_status_led_red, turn_dcm_status_led_green, turn_dcm_status_led_blue, turn_off_dcm_status_led);

        dim_status_led = App_SharedRgbLed_Create(
            turn_dim_status_led_red, turn_dim_status_led_green, turn_dim_status_led_blue, turn_off_dim_status_led);

        fsm_status_led = App_SharedRgbLed_Create(
            turn_fsm_status_led_red, turn_fsm_status_led_green, turn_fsm_status_led_blue, turn_off_fsm_status_led);

        pdm_status_led = App_SharedRgbLed_Create(
            turn_pdm_status_led_red, turn_pdm_status_led_green, turn_pdm_status_led_blue, turn_off_pdm_status_led);

        clock = App_SharedClock_Create();

        world = App_DimWorld_Create(
            seven_seg_displays, heartbeat_monitor, rgb_led_sequence, drive_mode_switch, imd_led, bspd_led, start_switch,
            traction_control_switch, bms_status_led, dcm_status_led, dim_status_led, fsm_status_led, pdm_status_led,
            clock);
        // Default to starting the state machine in the `Drive` state
        state_machine = App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_DIM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT);
        RESET_FAKE(set_left_l_hex_digit);
        RESET_FAKE(set_left_m_hex_digit);
        RESET_FAKE(set_left_r_hex_digit);
        RESET_FAKE(set_middle_l_hex_digit);
        RESET_FAKE(set_middle_m_hex_digit);
        RESET_FAKE(set_middle_r_hex_digit);
        RESET_FAKE(set_right_l_hex_digit);
        RESET_FAKE(set_right_m_hex_digit);
        RESET_FAKE(set_right_r_hex_digit);
        RESET_FAKE(display_value_callback);
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
        RESET_FAKE(turn_bms_status_led_red);
        RESET_FAKE(turn_bms_status_led_green);
        RESET_FAKE(turn_bms_status_led_blue);
        RESET_FAKE(turn_off_bms_status_led);
        RESET_FAKE(turn_dcm_status_led_red);
        RESET_FAKE(turn_dcm_status_led_green);
        RESET_FAKE(turn_dcm_status_led_blue);
        RESET_FAKE(turn_off_dcm_status_led);
        RESET_FAKE(turn_dim_status_led_red);
        RESET_FAKE(turn_dim_status_led_green);
        RESET_FAKE(turn_dim_status_led_blue);
        RESET_FAKE(turn_off_dim_status_led);
        RESET_FAKE(turn_fsm_status_led_red);
        RESET_FAKE(turn_fsm_status_led_green);
        RESET_FAKE(turn_fsm_status_led_blue);
        RESET_FAKE(turn_off_fsm_status_led);
        RESET_FAKE(turn_pdm_status_led_red);
        RESET_FAKE(turn_pdm_status_led_green);
        RESET_FAKE(turn_pdm_status_led_blue);
        RESET_FAKE(turn_off_pdm_status_led);
    }

    void TearDown() override
    {
        TearDownObject(world, App_DimWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(left_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(left_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(left_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(seven_seg_displays, App_SevenSegDisplays_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(drive_mode_switch, App_RotarySwitch_Destroy);
        TearDownObject(imd_led, App_Led_Destroy);
        TearDownObject(bspd_led, App_Led_Destroy);
        TearDownObject(start_switch, App_BinarySwitch_Destroy);
        TearDownObject(traction_control_switch, App_BinarySwitch_Destroy);
        TearDownObject(torque_vectoring_switch, App_BinarySwitch_Destroy);
        TearDownObject(bms_status_led, App_SharedRgbLed_Destroy);
        TearDownObject(dcm_status_led, App_SharedRgbLed_Destroy);
        TearDownObject(dim_status_led, App_SharedRgbLed_Destroy);
        TearDownObject(fsm_status_led, App_SharedRgbLed_Destroy);
        TearDownObject(pdm_status_led, App_SharedRgbLed_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct DimWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_DimWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        // DIM doesn't use any signals currently
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    }

    struct World *           world;
    struct StateMachine *    state_machine;
    struct SevenSegDisplay * left_l_seven_seg_display;
    struct SevenSegDisplay * left_m_seven_seg_display;
    struct SevenSegDisplay * left_r_seven_seg_display;
    struct SevenSegDisplay * middle_l_seven_seg_display;
    struct SevenSegDisplay * middle_m_seven_seg_display;
    struct SevenSegDisplay * middle_r_seven_seg_display;
    struct SevenSegDisplay * right_l_seven_seg_display;
    struct SevenSegDisplay * right_m_seven_seg_display;
    struct SevenSegDisplay * right_r_seven_seg_display;
    struct SevenSegDisplays *seven_seg_displays;
    struct HeartbeatMonitor *heartbeat_monitor;
    struct RgbLedSequence *  rgb_led_sequence;
    struct Led *             imd_led;
    struct Led *             bspd_led;
    struct BinarySwitch *    start_switch;
    struct BinarySwitch *    traction_control_switch;
    struct BinarySwitch *    torque_vectoring_switch;
    struct RotarySwitch *    drive_mode_switch;
    struct RgbLed *          bms_status_led;
    struct RgbLed *          dcm_status_led;
    struct RgbLed *          dim_status_led;
    struct RgbLed *          fsm_status_led;
    struct RgbLed *          pdm_status_led;
    struct Clock *           clock;
};

// DIM-12
TEST_F(DimStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(DIM_STATE_DRIVE, App_CanTx_DIM_Vitals_State_Get());
}

// DIM-9
// TODO: Redo 7-seg display tests
// TEST_F(DimStateMachineTest, check_7_seg_displays_show_state_of_charge_in_drive_state_if_there_is_no_error)
//{
//    App_CanRx_BMS_Vitals_StateOfCharge_Update(0.0f);
//    LetTimePass(state_machine, 10);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(false, set_middle_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[0].value);
//
//    App_CanRx_BMS_Vitals_StateOfCharge_Update(50.0f);
//    LetTimePass(state_machine, 10);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[1].value);
//    ASSERT_EQ(5, set_middle_hex_digit_fake.arg0_history[1].value);
//
//    App_CanRx_BMS_Vitals_StateOfCharge_Update(100.0f);
//    LetTimePass(state_machine, 10);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[2].value);
//    ASSERT_EQ(0, set_middle_hex_digit_fake.arg0_history[2].value);
//    ASSERT_EQ(1, set_right_hex_digit_fake.arg0_history[2].value);
//}
//
//// DIM-9
// TEST_F(DimStateMachineTest, check_7_seg_displays_show_error_id_in_drive_state_if_there_is_any_error)
//{
//    // Set error for some made-up error ID, the value 10 was chosen because
//    // it will be value as valid as we have more than 10 errors, which should
//    // always be true.
//    //    App_SharedErrorTable_SetError(error_table, (enum ErrorId)(10), true);
//
//    LetTimePass(state_machine, 10);
//
//    // When an error ID shows up on the 7-segment displays, it will have an
//    // offset of 500 added to it. This is why we are asserting for the value
//    // of 10 + 500 = 510 below.
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(1, set_middle_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_val.value);
//}
//
//// DIM-9
// TEST_F(DimStateMachineTest, check_7_seg_displays_cycle_through_two_error_ids_in_drive_state)
//{
//    // Set errors for some made-up error IDs, the value 10 and 11 were chosen
//    // because it will be valid as long as we have more than 11 errors, which
//    // should always be true.
//    //    App_SharedErrorTable_SetError(error_table, (enum ErrorId)(10), true);
//    //    App_SharedErrorTable_SetError(error_table, (enum ErrorId)(11), true);
//
//    // When an error ID shows up on the 7-segment displays, it will have an
//    // offset of 500 added to it. This is why we are asserting for the value
//    // of 10 + 500 = 510 below. The same applies for the rest of this test case.
//    LetTimePass(state_machine, 999);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(1, set_middle_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_val.value);
//
//    LetTimePass(state_machine, 1000);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(1, set_left_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(1, set_middle_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_val.value);
//
//    LetTimePass(state_machine, 1000);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(1, set_middle_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_val.value);
//
//    LetTimePass(state_machine, 1000);
//    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_val.enabled);
//    ASSERT_EQ(1, set_left_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(1, set_middle_hex_digit_fake.arg0_val.value);
//    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_val.value);
//}

// DIM-4
TEST_F(DimStateMachineTest, check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    start_switch_is_turned_on_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_Switches_StartSwitch_Get());

    start_switch_is_turned_on_fake.return_val = true;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_Switches_StartSwitch_Get());
}

// DIM-4
TEST_F(DimStateMachineTest, check_aux_switch_is_broadcasted_over_can_in_drive_state)
{
    traction_control_switch_is_turned_on_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_Switches_AuxSwitch_Get());

    traction_control_switch_is_turned_on_fake.return_val = true;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_Switches_AuxSwitch_Get());
}

// DIM-5
TEST_F(DimStateMachineTest, imd_led_control_in_drive_state)
{
    App_CanRx_BMS_OkStatuses_ImdOk_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_imd_led_fake.call_count);

    App_CanRx_BMS_OkStatuses_ImdOk_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_imd_led_fake.call_count);

    App_CanRx_BMS_OkStatuses_ImdOk_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_on_imd_led_fake.call_count);
    ASSERT_EQ(2, turn_off_imd_led_fake.call_count);
}

// DIM-6
TEST_F(DimStateMachineTest, bspd_led_control_in_drive_state)
{
    App_CanRx_BMS_OkStatuses_BspdOk_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_bspd_led_fake.call_count);

    App_CanRx_BMS_OkStatuses_BspdOk_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(1, turn_off_bspd_led_fake.call_count);

    App_CanRx_BMS_OkStatuses_BspdOk_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_on_bspd_led_fake.call_count);
    ASSERT_EQ(2, turn_off_bspd_led_fake.call_count);
}

TEST_F(DimStateMachineTest, rgb_led_sequence_in_drive_state)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count, &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    // Verify that we cycle through red, green, blue, red, etc. for 99 times.
    // The number 99 can be changed to any other number that is a multiple
    // of 3. The significance of 3 is because we have 3 colors (Red, green,
    // and blue).
    for (size_t i = 0; i < 99; i++)
    {
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
    }
}

// DIM-2
TEST_F(DimStateMachineTest, dim_board_status_led_control_with_warning)
{
    // Set any non-critical error and check that the DIM LED turns blue
    App_CanTx_DIM_Warnings_StackWatermarkAboveThresholdTask1Hz_Set(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dim_status_led_blue_fake.call_count);

    // Reset warning for subsequent tests
    App_CanTx_DIM_Warnings_StackWatermarkAboveThresholdTask1Hz_Set(false);
}

// DIM-2
TEST_F(DimStateMachineTest, dim_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DIM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dim_status_led_green_fake.call_count);
}

// DIM-2
TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    // TODO: JSON2CAN
    //    App_SharedErrorTable_SetError(error_table, DCM_AIR_SHUTDOWN_MISSING_HEARTBEAT, true);
    App_CanRx_DCM_Warnings_MissingHeartbeat_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dcm_status_led_red_fake.call_count);

    App_CanRx_DCM_Warnings_MissingHeartbeat_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_warning)
{
    // Set any warning and check that the DCM LED turns blue
    App_CanRx_DCM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dcm_status_led_blue_fake.call_count);

    // Reset warning for next test
    App_CanRx_DCM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DCM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dcm_status_led_green_fake.call_count);
}

// DIM-2
TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // DCM LED red rather than blue
    // TODO: JSON2CAN
    //    App_SharedErrorTable_SetError(error_table, DCM_AIR_SHUTDOWN_MISSING_HEARTBEAT, true);
    //    App_SharedErrorTable_SetError(error_table, DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, true);
    App_CanRx_DCM_Warnings_MissingHeartbeat_Update(true);
    App_CanRx_DCM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_dcm_status_led_red_fake.call_count);
    ASSERT_EQ(0, turn_dcm_status_led_blue_fake.call_count);

    App_CanRx_DCM_Warnings_MissingHeartbeat_Update(false);
    App_CanRx_DCM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the FSM LED turns red
    // TODO: JSON2CAN
    //    App_SharedErrorTable_SetError(error_table, FSM_AIR_SHUTDOWN_MISSING_HEARTBEAT, true);
    App_CanRx_FSM_Warnings_MissingHeartbeat_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_fsm_status_led_red_fake.call_count);

    App_CanRx_FSM_Warnings_MissingHeartbeat_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_warning)
{
    // Set any warning and check that the FSM LED turns blue
    // TODO: JSON2CAN
    //    App_SharedErrorTable_SetError(error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, true);
    App_CanRx_FSM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_fsm_status_led_blue_fake.call_count);

    App_CanRx_FSM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the FSM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_fsm_status_led_green_fake.call_count);
}

// DIM-2
TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // FSM LED red rather than blue
    // TODO: JSON2CAN
    //    App_SharedErrorTable_SetError(error_table, FSM_AIR_SHUTDOWN_MISSING_HEARTBEAT, true);
    //    App_SharedErrorTable_SetError(error_table, FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, true);
    App_CanRx_FSM_Warnings_MissingHeartbeat_Update(true);
    App_CanRx_FSM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(true);

    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_fsm_status_led_red_fake.call_count);
    ASSERT_EQ(0, turn_fsm_status_led_blue_fake.call_count);

    App_CanRx_FSM_Warnings_MissingHeartbeat_Update(false);
    App_CanRx_FSM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, bms_board_status_led_control_with_fault)
{
    // Set OK statuses such that the red led is not set without fault
    App_CanRx_BMS_OkStatuses_ImdOk_Update(true);
    App_CanRx_BMS_OkStatuses_BspdOk_Update(true);
    App_CanRx_BMS_OkStatuses_BmsOk_Update(true);

    // Set any critical error and check that the BMS LED turns red
    App_CanRx_BMS_Vitals_CurrentState_Update(BMS_FAULT_STATE);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(1, turn_bms_status_led_red_fake.call_count);
}

// DIM-2
TEST_F(DimStateMachineTest, pdm_board_status_led_control_with_warning)
{
    // Set any warning and check that the PDM LED turns blue
    App_CanRx_PDM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_pdm_status_led_blue_fake.call_count);

    // Reset warning for subsequent tests
    App_CanRx_PDM_Warnings_StackWatermarkAboveThresholdTask1Hz_Update(false);
}

// DIM-2
TEST_F(DimStateMachineTest, pdm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the PDM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, turn_pdm_status_led_green_fake.call_count);
}
} // namespace StateMachineTest
