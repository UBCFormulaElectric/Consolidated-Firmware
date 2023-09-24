#include "test_dimBaseStateMachine.h"

class LedStatusTest : public DimBaseStateMachineTest
{
};

TEST_F(LedStatusTest, imd_led_control_in_drive_state)
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

TEST_F(LedStatusTest, bspd_led_control_in_drive_state)
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

TEST_F(LedStatusTest, dim_board_status_led_control_with_warning)
{
    // Set any non-critical error and check that the DIM LED turns blue
    App_CanAlerts_SetWarning(DIM_WARNING_WATCHDOG_TIMEOUT, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dim_status_led, false, false, true));
}

TEST_F(LedStatusTest, dim_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DIM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dim_status_led, false, true, false));
}

TEST_F(LedStatusTest, dcm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    App_CanRx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, true, false, false));
}

TEST_F(LedStatusTest, dcm_board_status_led_control_with_warning)
{
    // Set any warning and check that the DCM LED turns blue
    App_CanRx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, false, false, true));
}

TEST_F(LedStatusTest, dcm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DCM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&dcm_status_led, false, true, false));
}

TEST_F(LedStatusTest, dcm_board_status_led_control_with_multiple_errors)
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

TEST_F(LedStatusTest, fsm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the FSM LED turns red
    App_CanRx_FSM_Faults_FSM_FAULT_MISSING_HEARTBEAT_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, true, false, false));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_warning)
{
    // Set any warning and check that the FSM LED turns blue
    App_CanRx_FSM_Warnings_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, false, true));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the FSM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, true, false));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_multiple_errors)
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

TEST_F(LedStatusTest, bms_board_status_led_control_with_fault)
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

TEST_F(LedStatusTest, pdm_board_status_led_control_with_warning)
{
    // Set any warning and check that the PDM LED turns blue
    App_CanRx_PDM_Warnings_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Update(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&pdm_status_led, false, false, true));
}

TEST_F(LedStatusTest, pdm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the PDM LED turns green
    LetTimePass(state_machine, 10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&pdm_status_led, false, true, false));
}
