/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canRx.h"
#include "app_canRx.h"
#include "app_canUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool io_canRx_filterMessageId(uint32_t std_id)
{
    bool is_found = false;
    switch (std_id)
    {
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_CRIT_WARNINGS_ID:
        case CAN_MSG_CRIT_FAULTS_ID:
        case CAN_MSG_VC_WARNINGS_ID:
        case CAN_MSG_VC_FAULTS_ID:
        {
            is_found = true;
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return is_found;
}

void io_canRx_updateRxTableWithMessage(JsonCanMsg* msg)
{
    switch (msg->std_id)
    {
        case CAN_MSG_BMS_WARNINGS_ID:
        {
            BMS_Warnings_Signals out_msg;
            app_canUtils_BMS_Warnings_unpack(msg->data, &out_msg);
            
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            BMS_Faults_Signals out_msg;
            app_canUtils_BMS_Faults_unpack(msg->data, &out_msg);
            
            app_canRx_BMS_Fault_DummyFault_update(out_msg.BMS_Fault_DummyFault_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            app_canUtils_FSM_Warnings_unpack(msg->data, &out_msg);
            
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            app_canUtils_FSM_Faults_unpack(msg->data, &out_msg);
            
            app_canRx_FSM_Fault_DummyFault_update(out_msg.FSM_Fault_DummyFault_value);
            break;
        }
        case CAN_MSG_CRIT_WARNINGS_ID:
        {
            CRIT_Warnings_Signals out_msg;
            app_canUtils_CRIT_Warnings_unpack(msg->data, &out_msg);
            
            app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_update(out_msg.CRIT_Warning_StackWaterMarkHighTask1Hz_value);
            app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_update(out_msg.CRIT_Warning_StackWaterMarkHighTask100Hz_value);
            app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_update(out_msg.CRIT_Warning_StackWaterMarkHighTask1kHz_value);
            app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_update(out_msg.CRIT_Warning_StackWaterMarkHighTaskCanRx_value);
            app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_update(out_msg.CRIT_Warning_StackWaterMarkHighTaskCanTx_value);
            app_canRx_CRIT_Warning_WatchdogTimeout_update(out_msg.CRIT_Warning_WatchdogTimeout_value);
            app_canRx_CRIT_Warning_TxOverflow_update(out_msg.CRIT_Warning_TxOverflow_value);
            app_canRx_CRIT_Warning_RxOverflow_update(out_msg.CRIT_Warning_RxOverflow_value);
            break;
        }
        case CAN_MSG_CRIT_FAULTS_ID:
        {
            CRIT_Faults_Signals out_msg;
            app_canUtils_CRIT_Faults_unpack(msg->data, &out_msg);
            
            app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(out_msg.CRIT_Fault_MissingBMSHeartbeat_value);
            app_canRx_CRIT_Fault_MissingFSMHeartbeat_update(out_msg.CRIT_Fault_MissingFSMHeartbeat_value);
            app_canRx_CRIT_Fault_MissingVCHeartbeat_update(out_msg.CRIT_Fault_MissingVCHeartbeat_value);
            app_canRx_CRIT_Fault_MissingRSMHeartbeat_update(out_msg.CRIT_Fault_MissingRSMHeartbeat_value);
            break;
        }
        case CAN_MSG_VC_WARNINGS_ID:
        {
            VC_Warnings_Signals out_msg;
            app_canUtils_VC_Warnings_unpack(msg->data, &out_msg);
            
            break;
        }
        case CAN_MSG_VC_FAULTS_ID:
        {
            VC_Faults_Signals out_msg;
            app_canUtils_VC_Faults_unpack(msg->data, &out_msg);
            
            app_canRx_VC_Fault_DummyFault_update(out_msg.VC_Fault_DummyFault_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

