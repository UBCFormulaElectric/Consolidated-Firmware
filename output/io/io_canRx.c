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
        case CANMSG_VC_VITALS_ID:
        case CANMSG_VC_WARNINGS_ID:
        case CANMSG_VC_FAULTS_ID:
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
        case CANMSG_VC_VITALS_ID:
        {
            VC_Vitals_Signals out_msg;
            app_canUtils_VC_Vitals_unpack(msg->data, &out_msg);
            app_canRx_VC_Heartbeat_update(out_msg.VC_Heartbeat_value);
            app_canRx_VC_State_update(out_msg.VC_State_value);
            app_canRx_VC_BuzzerOn_update(out_msg.VC_BuzzerOn_value);
            app_canRx_VC_NumberOfCanDataLogs_update(out_msg.VC_NumberOfCanDataLogs_value);
            app_canRx_VC_CanLoggingRemainingErrors_update(out_msg.VC_CanLoggingRemainingErrors_value);
            break;
        }
        case CANMSG_VC_WARNINGS_ID:
        {
            VC_Warnings_Signals out_msg;
            app_canUtils_VC_Warnings_unpack(msg->data, &out_msg);
            app_canRx_VC_Warning_StackWaterMarkHighTask1Hz_update(out_msg.VC_Warning_StackWaterMarkHighTask1Hz_value);
            app_canRx_VC_Warning_StackWaterMarkHighTask100Hz_update(out_msg.VC_Warning_StackWaterMarkHighTask100Hz_value);
            app_canRx_VC_Warning_StackWaterMarkHighTask1kHz_update(out_msg.VC_Warning_StackWaterMarkHighTask1kHz_value);
            app_canRx_VC_Warning_StackWaterMarkHighTaskCanRx_update(out_msg.VC_Warning_StackWaterMarkHighTaskCanRx_value);
            app_canRx_VC_Warning_StackWaterMarkHighTaskCanTx_update(out_msg.VC_Warning_StackWaterMarkHighTaskCanTx_value);
            app_canRx_VC_Warning_WatchdogTimeout_update(out_msg.VC_Warning_WatchdogTimeout_value);
            app_canRx_VC_Warning_TxOverflow_update(out_msg.VC_Warning_TxOverflow_value);
            app_canRx_VC_Warning_RxOverflow_update(out_msg.VC_Warning_RxOverflow_value);
            app_canRx_VC_Warning_RegenNotAvailable_update(out_msg.VC_Warning_RegenNotAvailable_value);
            app_canRx_VC_Warning_ImuInitFailed_update(out_msg.VC_Warning_ImuInitFailed_value);
            app_canRx_VC_Warning_FlowRateInputOutOfRange_update(out_msg.VC_Warning_FlowRateInputOutOfRange_value);
            app_canRx_VC_Warning_BatteryRailOvercurrentFault_update(out_msg.VC_Warning_BatteryRailOvercurrentFault_value);
            app_canRx_VC_Warning_AccumulatorRailOvercurrentFault_update(out_msg.VC_Warning_AccumulatorRailOvercurrentFault_value);
            app_canRx_VC_Warning_LvChargeFault_update(out_msg.VC_Warning_LvChargeFault_value);
            app_canRx_VC_Warning_BoostControllerFault_update(out_msg.VC_Warning_BoostControllerFault_value);
            app_canRx_VC_Warning_HighNumberOfCanDataLogs_update(out_msg.VC_Warning_HighNumberOfCanDataLogs_value);
            app_canRx_VC_Warning_CanLoggingSdCardNotPresent_update(out_msg.VC_Warning_CanLoggingSdCardNotPresent_value);
            app_canRx_VC_Warning_CanLoggingErrored_update(out_msg.VC_Warning_CanLoggingErrored_value);
            app_canRx_VC_Warning_BrakeAppsDisagreement_update(out_msg.VC_Warning_BrakeAppsDisagreement_value);
            app_canRx_VC_Warning_SbgInitFailed_update(out_msg.VC_Warning_SbgInitFailed_value);
            break;
        }
        case CANMSG_VC_FAULTS_ID:
        {
            VC_Faults_Signals out_msg;
            app_canUtils_VC_Faults_unpack(msg->data, &out_msg);
            app_canRx_VC_Fault_MissingBMSHeartbeat_update(out_msg.VC_Fault_MissingBMSHeartbeat_value);
            app_canRx_VC_Fault_MissingFSMHeartbeat_update(out_msg.VC_Fault_MissingFSMHeartbeat_value);
            app_canRx_VC_Fault_MissingRSMHeartbeat_update(out_msg.VC_Fault_MissingRSMHeartbeat_value);
            app_canRx_VC_Fault_MissingCRITHeartbeat_update(out_msg.VC_Fault_MissingCRITHeartbeat_value);
            app_canRx_VC_Fault_LeftInverterFault_update(out_msg.VC_Fault_LeftInverterFault_value);
            app_canRx_VC_Fault_RightInverterFault_update(out_msg.VC_Fault_RightInverterFault_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}