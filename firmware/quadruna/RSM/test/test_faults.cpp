#include "test_rsmBaseStateMachineTest.h"
#include "app_coolant.h"
#include "app_loadCell.h"
#include "app_suspension.h"
#include <cmath>

class RsmFaultsTest : public RsmBaseStateMachineTest
{
};

TEST_F(RsmFaultsTest, check_state_transition_fault_state_heartbeat_timeout)
{
    // Test that a missing heartbeat will put the RSM into fault state. The RSM monitors the BMS and FSM heartbeat.
    app_heartbeatMonitor_blockFaults(false);
    // Check in all heartbeats within timeout period
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    // Heartbeat faults initially present at startup until cleared
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    app_canRx_VC_Heartbeat_update(true);  // Check in VC heartbeat
    app_canRx_FSM_Heartbeat_update(true); // check in FSM heartbeat
    LetTimePass(10);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    // Fail to check heartbeat, FSM and VC should fault
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    LetTimePass(20);
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    // Stay faulted indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    // Check heartbeat back in, fault should clear and transition back to init
    app_canRx_VC_Heartbeat_update(true);  // Check in heartbeat
    app_canRx_FSM_Heartbeat_update(true); // Check in heartbeat
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 20U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());

    app_canRx_FSM_Heartbeat_update(false); // do not check in FSM

    LetTimePass(20); // here VC shouldnt fault but FSM should
    ASSERT_FALSE(app_canAlerts_RSM_Fault_MissingVCHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get());
}

TEST_F(RsmFaultsTest, primary_flow_rate_underflow_sets_fault)
{
    app_canRx_VC_State_update(VC_DRIVE_STATE);
    // Flow rate underflow threshold is 1.0 L/min
    const float underflow_threshold = 1.0f;
    fake_io_coolant_getFlowRate_returns(std::nextafter(underflow_threshold, std::numeric_limits<float>::lowest()));

    LetTimePass(10 + FLOW_METER_TIME_TO_FAULT - 1);
    ASSERT_FALSE(app_canAlerts_RSM_Fault_FlowMeterUnderflow_get());
    LetTimePass(1);
    ASSERT_TRUE(app_canAlerts_RSM_Fault_FlowMeterUnderflow_get());

    // Confirm fault set indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Fault_FlowMeterUnderflow_get());

    // Clear condition, confirm fault resets
    fake_io_coolant_getFlowRate_returns(std::nextafter(underflow_threshold, std::numeric_limits<float>::max()));

    LetTimePass(10 + FLOW_METER_TIME_TO_CLEAR - 1);
    ASSERT_TRUE(app_canAlerts_RSM_Fault_FlowMeterUnderflow_get());
    LetTimePass(1);
    ASSERT_FALSE(app_canAlerts_RSM_Fault_FlowMeterUnderflow_get());
}

TEST_F(RsmFaultsTest, left_suspension_ocsc_sets_warning)
{
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_LeftSuspensionOCSC_get());

    // Set left suspension OCSC
    fake_io_suspension_leftSensorOCSC_returns(true);
    LetTimePass(10);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LeftSuspensionOCSC_get());

    // Confirm set indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LeftSuspensionOCSC_get());

    // Clear condition, confirm warning resets
    fake_io_suspension_leftSensorOCSC_returns(false);
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_LeftSuspensionOCSC_get());
}

TEST_F(RsmFaultsTest, right_suspension_ocsc_sets_warning)
{
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_RightSuspensionOCSC_get());

    // Set right suspension OCSC
    fake_io_suspension_rightSensorOCSC_returns(true);
    LetTimePass(10);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_RightSuspensionOCSC_get());

    // Confirm set indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_RightSuspensionOCSC_get());

    // Clear condition, confirm warning resets
    fake_io_suspension_rightSensorOCSC_returns(false);
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_RightSuspensionOCSC_get());
}

TEST_F(RsmFaultsTest, load_cell_3_ocsc_sets_warning)
{
    fake_io_loadCell_sensor3OCSC_returns(true);
    LetTimePass(10);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LoadCell3OCSC_get());

    // Confirm set indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LoadCell3OCSC_get());

    // Clear condition, confirm warning resets
    fake_io_loadCell_sensor3OCSC_returns(false);
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_LoadCell3OCSC_get());
}

TEST_F(RsmFaultsTest, load_cell_4_ocsc_sets_warning)
{
    fake_io_loadCell_sensor4OCSC_returns(true);
    LetTimePass(10);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LoadCell4OCSC_get());

    // Confirm set indefinitely
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_RSM_Warning_LoadCell4OCSC_get());

    // Clear condition, confirm warning resets
    fake_io_loadCell_sensor4OCSC_returns(false);
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_RSM_Warning_LoadCell4OCSC_get());
}