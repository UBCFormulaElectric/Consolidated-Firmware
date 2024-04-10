#include "test_critBaseStateMachineTest.h"
#include "fake_io_shutdownSensor.hpp"

class LedStatusTest : public CritBaseStateMachineTest
{
};

TEST_F(LedStatusTest, imd_led_control_in_drive_state)
{
    app_canRx_BMS_ImdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, false));

    app_canRx_BMS_ImdLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, false));

    app_canRx_BMS_ImdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&imd_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&imd_led, false));
}

TEST_F(LedStatusTest, bspd_led_control_in_drive_state)
{
    app_canRx_BMS_BspdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, false));

    app_canRx_BMS_BspdLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, false));

    app_canRx_BMS_BspdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&bspd_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&bspd_led, false));
}

TEST_F(LedStatusTest, ams_led_control_in_drive_state)
{
    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&ams_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&ams_led, false));

    app_canRx_BMS_BmsLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&ams_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&ams_led, false));

    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&ams_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&ams_led, false));
}

TEST_F(LedStatusTest, shdn_led_control_in_drive_state)
{
    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen,true);
    ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&shdn_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, false));

    app_canRx_BMS_BmsLatchedFault_update(true);
    LetTimePass(10);
    fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, false);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, true));
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, false));

    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, true);
    ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, true));
    ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&shdn_led, false));
}

/**
 * CRIT TESTS
 */

TEST_F(LedStatusTest, crit_board_status_led_control_with_warning)
{
    // Set any non-critical error and check that the CRIT LED turns blue
    app_canAlerts_CRIT_Warning_WatchdogTimeout_set(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&crit_status_led, false, false, true));
}

TEST_F(LedStatusTest, crit_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the CRIT LED turns green
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&crit_status_led, false, true, false));
}

TEST_F(LedStatusTest, crit_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    app_canTx_CRIT_Fault_MissingBMSHeartbeat_set(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&crit_status_led, true, false, false));
}

/**
 * VC TESTS
 */

TEST_F(LedStatusTest, vc_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    app_canRx_VC_Fault_MissingBMSHeartbeat_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&vc_status_led, true, false, false));
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_warning)
{
    // Set any warning and check that the DCM LED turns blue
    app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&vc_status_led, false, false, true));
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the DCM LED turns green
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&vc_status_led, false, true, false));
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // DCM LED red rather than blue
    app_canRx_VC_Fault_MissingBMSHeartbeat_update(true);
    app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&vc_status_led, true, false, false));
    ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&vc_status_led, false, false, true));
}

/**
 * FSM TESTS
 */

TEST_F(LedStatusTest, fsm_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the FSM LED turns red
    app_canRx_FSM_Fault_MissingBMSHeartbeat_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, true, false, false));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_warning)
{
    // Set any warning and check that the FSM LED turns blue
    app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, false, true));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the FSM LED turns green
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, true, false));
}

TEST_F(LedStatusTest, fsm_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // FSM LED red rather than blue
    app_canRx_FSM_Warning_StackWaterMarkHighTask1Hz_update(true);
    app_canRx_FSM_Fault_MissingBMSHeartbeat_update(true);

    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, true, false, false));
    ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&fsm_status_led, false, false, true));
}

/**
 * BMS TESTS
 */

TEST_F(LedStatusTest, bms_board_status_led_control_with_fault)
{
    // Set OK statuses such that the red led is not set without fault
    app_canRx_BMS_ImdOk_update(true);
    app_canRx_BMS_BspdOk_update(true);
    app_canRx_BMS_BmsOk_update(true);

    // Set any critical error and check that the BMS LED turns red
    app_canRx_BMS_Fault_CellOvertemp_update(true);
    LetTimePass(10);

    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, true, false, false));
}

TEST_F(LedStatusTest, bms_board_status_led_control_with_warning)
{
    // Set any warning and check that the BMS LED turns blue
    app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, false, false, true));
}

TEST_F(LedStatusTest, bms_board_status_led_control_with_no_error)
{
    // Don't set any error and check that the BMS LED turns green
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, false, true, false));
}

TEST_F(LedStatusTest, bms_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // BMS LED red rather than blue
    app_canRx_BMS_Warning_StackWaterMarkHighTask1Hz_update(true);
    app_canRx_BMS_Fault_CellOvertemp_update(true);

    LetTimePass(10);
    ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, true, false, false));
    ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&bms_status_led, false, false, true));
}

/**
 * RSM TESTS
 */

// TEST_F(LedStatusTest, rsm_board_status_led_control_with_critical_error)
// {
//     // Set any critical error and check that the FSM LED turns red
//     app_canRx_RSM_Fault_MissingBMSHeartbeat_update(true);
//     LetTimePass(10);
//     ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&rsm_status_led, true, false, false));
// }

// TEST_F(LedStatusTest, rsm_board_status_led_control_with_warning)
// {
//     // Set any warning and check that the FSM LED turns blue
//     app_canRx_RSM_Warning_StackWaterMarkHighTask1Hz_update(true);
//     LetTimePass(10);
//     ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&rsm_status_led, false, false, true));
// }

// TEST_F(LedStatusTest, rsm_board_status_led_control_with_no_error)
// {
//     // Don't set any error and check that the FSM LED turns green
//     LetTimePass(10);
//     ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&rsm_status_led, false, true, false));
// }

// TEST_F(LedStatusTest, rsm_board_status_led_control_with_multiple_errors)
// {
//     // If the error table contains critical and non-critical errors
//     // simultaneously, the critical error should take precedence and turn the
//     // FSM LED red rather than blue
//     app_canRx_RSM_Warning_StackWaterMarkHighTask1Hz_update(true);
//     app_canRx_RSM_Fault_MissingBMSHeartbeat_update(true);

//     LetTimePass(10);
//     ASSERT_EQ(1, fake_io_rgbLed_enable_callCountForArgs(&rsm_status_led, true, false, false));
//     ASSERT_EQ(0, fake_io_rgbLed_enable_callCountForArgs(&rsm_status_led, false, false, true));
// }