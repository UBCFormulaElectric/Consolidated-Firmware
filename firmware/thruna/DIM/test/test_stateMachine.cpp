#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_led.hpp"
#include "fake_io_rgbLed.hpp"
#include "fake_io_switch.hpp"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "app_sevenSegDisplays.h"
#include "App_SharedRgbLedSequence.h"
#include "App_AvgPower.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/App_DriveState.h"
#include "configs/App_RotarySwitchConfig.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "app_globals.h"
}

class DimStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        clock = App_SharedClock_Create();

        avg_power_calc = App_AvgPowerCalc_Create();

        world = App_DimWorld_Create(heartbeat_monitor, clock, avg_power_calc);

        state_machine = App_SharedStateMachine_Create(world, App_GetDriveState());

        app_sevenSegDisplays_init();
        app_globals_init(&globals_config);
    }

    void TearDown() override
    {
        TearDownObject(world, App_DimWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(avg_power_calc, App_AvgPowerCalc_Destroy);

        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_led_enable_reset();
        fake_io_rgbLed_enable_reset();
        fake_io_rgbLed_disable_reset();
        fake_io_switch_isClosed_reset();
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

    struct DimWorld *        world;
    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
    struct Clock *           clock;
    struct AvgPowerCalc *    avg_power_calc;

    const BinaryLed       imd_led           = {};
    const BinaryLed       bspd_led          = {};
    const BinaryLed       shdn_led          = {};
    const BinaryLed       drive_led         = {};
    const Switch          start_switch      = {};
    const Switch          aux_switch        = {};
    const RgbLed          bms_status_led    = {};
    const RgbLed          dcm_status_led    = {};
    const RgbLed          fsm_status_led    = {};
    const RgbLed          pdm_status_led    = {};
    const RgbLed          dim_status_led    = {};
    const SevenSegsConfig seven_segs_config = {};

    const GlobalsConfig globals_config = {
        .imd_led        = &imd_led,
        .bspd_led       = &bspd_led,
        .shdn_led       = &shdn_led,
        .drive_led      = &drive_led,
        .start_switch   = &start_switch,
        .aux_switch     = &aux_switch,
        .bms_status_led = &bms_status_led,
        .dcm_status_led = &dcm_status_led,
        .fsm_status_led = &fsm_status_led,
        .pdm_status_led = &pdm_status_led,
        .dim_status_led = &dim_status_led,
    };
};

TEST_F(DimStateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetDriveState());

    EXPECT_EQ(DIM_STATE_DRIVE, App_CanTx_DIM_Vitals_State_Get());
}

TEST_F(DimStateMachineTest, check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&start_switch, false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_Switches_StartSwitch_Get());

    fake_io_switch_isClosed_returnsForArgs(&start_switch, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_Switches_StartSwitch_Get());
}

TEST_F(DimStateMachineTest, check_aux_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&aux_switch, false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_Switches_AuxSwitch_Get());

    fake_io_switch_isClosed_returnsForArgs(&aux_switch, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_Switches_AuxSwitch_Get());
}

TEST_F(DimStateMachineTest, imd_led_control_in_drive_state)
{
    App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, false));

    App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, false));

    App_CanRx_BMS_OkStatuses_ImdLatchedFaultStatus_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&imd_led, false));
}

TEST_F(DimStateMachineTest, bspd_led_control_in_drive_state)
{
    App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, false));

    App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, false));

    App_CanRx_BMS_OkStatuses_BspdLatchedFaultStatus_Update(false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&bspd_led, false));
}

TEST_F(DimStateMachineTest, dim_board_status_led_control_with_warning)
{
    // Set any non-critical error and check that the DIM LED turns blue
    App_CanAlerts_SetWarning(DIM_WARNING_WATCHDOG_TIMEOUT, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dim_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, dim_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DIM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dim_status_led, false, true, false));
}

TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, true, false, false));
}

TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_warning)
{
    // Set any warning and check that the DCM LED turns blue
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DCM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, false, true, false));
}

TEST_F(DimStateMachineTest, dcm_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // DCM LED red rather than blue
    App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(true);
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, true, false, false));
    ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the FSM LED turns red
    App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, true, false, false));
}

TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_warning)
{
    // Set any warning and check that the FSM LED turns blue
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the FSM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, true, false));
}

TEST_F(DimStateMachineTest, fsm_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // FSM LED red rather than blue
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(true);

    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, true, false, false));
    ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, bms_board_status_led_control_with_fault)
{
    // Set OK statuses such that the red led is not set without fault
    App_CanRx_BMS_OkStatuses_ImdOk_Update(true);
    App_CanRx_BMS_OkStatuses_BspdOk_Update(true);
    App_CanRx_BMS_OkStatuses_BmsOk_Update(true);

    // Set any critical error and check that the BMS LED turns red
    App_CanRx_BMS_Faults_BMS_FAULT_CELL_OVERTEMP_Update(true);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, true, false, false));
}

TEST_F(DimStateMachineTest, pdm_board_status_led_control_with_warning)
{
    // Set any warning and check that the PDM LED turns blue
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&pdm_status_led, false, false, true));
}

TEST_F(DimStateMachineTest, pdm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the PDM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&pdm_status_led, false, true, false));
}

TEST_F(DimStateMachineTest, avg_power_calc_resets_with_switch)
{
    App_AvgPowerCalc_Enable(avg_power_calc, true);
    float test_val = App_AvgPowerCalc_Update(avg_power_calc, 45.6);
    float avg      = 45.6f;
    ASSERT_EQ(test_val, avg);

    App_AvgPowerCalc_Enable(avg_power_calc, false);
    test_val = App_AvgPowerCalc_Update(avg_power_calc, 78.9);
    ASSERT_EQ(test_val, 0);
}
