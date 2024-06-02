#include "test_critBaseStateMachineTest.h"

class LedStatusTest : public CritBaseStateMachineTest
{
};

TEST_F(LedStatusTest, imd_led_control_in_drive_state)
{
    app_canRx_BMS_ImdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_imd_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_imd_set_callCountForArgs(false));

    app_canRx_BMS_ImdLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_imd_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_imd_set_callCountForArgs(false));

    app_canRx_BMS_ImdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_imd_set_callCountForArgs(true));
    ASSERT_EQ(2, fake_io_led_imd_set_callCountForArgs(false));
}

TEST_F(LedStatusTest, bspd_led_control_in_drive_state)
{
    app_canRx_BMS_BspdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_bspd_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_bspd_set_callCountForArgs(false));

    app_canRx_BMS_BspdLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_bspd_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_bspd_set_callCountForArgs(false));

    app_canRx_BMS_BspdLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_bspd_set_callCountForArgs(true));
    ASSERT_EQ(2, fake_io_led_bspd_set_callCountForArgs(false));
}

TEST_F(LedStatusTest, ams_led_control_in_drive_state)
{
    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(0, fake_io_led_ams_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_ams_set_callCountForArgs(false));

    app_canRx_BMS_BmsLatchedFault_update(true);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_ams_set_callCountForArgs(true));
    ASSERT_EQ(1, fake_io_led_ams_set_callCountForArgs(false));

    app_canRx_BMS_BmsLatchedFault_update(false);
    LetTimePass(10);
    ASSERT_EQ(1, fake_io_led_ams_set_callCountForArgs(true));
    ASSERT_EQ(2, fake_io_led_ams_set_callCountForArgs(false));
}

// TEST_F(LedStatusTest, shdn_led_control_in_drive_state)
//{
//     app_canRx_BMS_BmsLatchedFault_update(false);
//     LetTimePass(10);
//     fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, true);
//     ASSERT_EQ(0, fake_io_led_enable_callCountForArgs(&shdn_led, true));
//     ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, false));
//
//     app_canRx_BMS_BmsLatchedFault_update(true);
//     LetTimePass(10);
//     fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, false);
//     ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, true));
//     ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, false));
//
//     app_canRx_BMS_BmsLatchedFault_update(false);
//     LetTimePass(10);
//     fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, true);
//     ASSERT_EQ(1, fake_io_led_enable_callCountForArgs(&shdn_led, true));
//     ASSERT_EQ(2, fake_io_led_enable_callCountForArgs(&shdn_led, false));
// }

/**
 * VC TESTS
 */

TEST_F(LedStatusTest, vc_board_status_led_control_with_critical_error)
{
    // Set any critical error and check that the DCM LED turns red
    for (const auto &[can_update, assert_func] :
         std::vector<std::pair<std::function<void(bool)>, std::function<uint32_t(BoardLEDStatus)>>>{
             { app_canRx_VC_Fault_MissingBMSHeartbeat_update, fake_io_led_vc_status_set_callCountForArgs },
             { app_canRx_BMS_Fault_CellOvertemp_update, fake_io_led_bms_status_set_callCountForArgs },
             { app_canRx_FSM_Fault_SappsOCSC_update, fake_io_led_fsm_status_set_callCountForArgs },
             { app_canRx_RSM_Fault_FlowMeterUnderflow_update, fake_io_led_rsm_status_set_callCountForArgs },
             { app_canTx_CRIT_Fault_MissingFSMHeartbeat_set, fake_io_led_crit_status_set_callCountForArgs } })
    {
        can_update(true);
        LetTimePass(10);
        ASSERT_EQ(1, assert_func(BOARD_LED_STATUS_FAULT));
    }
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_warning)
{
    // Set any warning and check that the DCM LED turns blue
    // Set any critical error and check that the DCM LED turns red
    for (const auto &[can_update, assert_func] :
         std::vector<std::pair<std::function<void(bool)>, std::function<uint32_t(BoardLEDStatus)>>>{
             { app_canRx_VC_Warning_ImuInitFailed_update, fake_io_led_vc_status_set_callCountForArgs },
             { app_canRx_BMS_Warning_RxOverflow_update, fake_io_led_bms_status_set_callCountForArgs },
             { app_canRx_FSM_Warning_LeftSuspensionOCSC_update, fake_io_led_fsm_status_set_callCountForArgs },
             { app_canRx_RSM_Warning_LoadCell3OCSC_update, fake_io_led_rsm_status_set_callCountForArgs },
             { app_canTx_CRIT_Warning_StackWaterMarkHighTask100Hz_set, fake_io_led_crit_status_set_callCountForArgs } })
    {
        can_update(true);
        LetTimePass(10);
        ASSERT_EQ(1, assert_func(BOARD_LED_STATUS_WARNING));
    }
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_no_error)
{
    LetTimePass(10);
    // Don't set any error and check that the DCM LED turns green
    for (const auto &assert_func : std::vector<std::function<uint32_t(BoardLEDStatus)>>{
             fake_io_led_vc_status_set_callCountForArgs, fake_io_led_bms_status_set_callCountForArgs,
             fake_io_led_fsm_status_set_callCountForArgs, fake_io_led_rsm_status_set_callCountForArgs,
             fake_io_led_crit_status_set_callCountForArgs })
    {
        const auto out = assert_func(BOARD_LED_STATUS_OK);
        ASSERT_EQ(1, out);
    }
}

TEST_F(LedStatusTest, vc_board_status_led_control_with_multiple_errors)
{
    // If the error table contains critical and non-critical errors
    // simultaneously, the critical error should take precedence and turn the
    // DCM LED red rather than blue
    for (const auto &[fault, warning, assert_func] : std::vector<
             std::tuple<std::function<void(bool)>, std::function<void(bool)>, std::function<uint32_t(BoardLEDStatus)>>>{
             { app_canRx_VC_Fault_MissingBMSHeartbeat_update, app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update,
               fake_io_led_vc_status_set_callCountForArgs },
             { app_canRx_BMS_Fault_CellOvertemp_update, app_canRx_BMS_Warning_RxOverflow_update,
               fake_io_led_bms_status_set_callCountForArgs },
             { app_canRx_FSM_Fault_SappsOCSC_update, app_canRx_FSM_Warning_LeftSuspensionOCSC_update,
               fake_io_led_fsm_status_set_callCountForArgs },
             { app_canRx_RSM_Fault_FlowMeterUnderflow_update, app_canRx_RSM_Warning_LoadCell3OCSC_update,
               fake_io_led_rsm_status_set_callCountForArgs },
             { app_canTx_CRIT_Fault_MissingFSMHeartbeat_set, app_canTx_CRIT_Warning_StackWaterMarkHighTask100Hz_set,
               fake_io_led_crit_status_set_callCountForArgs } })
    {
        fault(true);
        warning(true);
        LetTimePass(10);
        ASSERT_EQ(1, assert_func(BOARD_LED_STATUS_FAULT));
        ASSERT_EQ(0, assert_func(BOARD_LED_STATUS_WARNING));
    }
}