/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "Io_CanRx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool Io_CanRx_FilterMessageId(uint32_t std_id)
{
    bool is_found = false;
    switch (std_id)
    {
        case CAN_MSG_FSM_APPS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
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

void Io_CanRx_UpdateRxTableWithMessage(struct CanMsg* msg)
{
    switch (msg->std_id)
    {
        case CAN_MSG_FSM_APPS_ID:
        {
            FsmApps_Signals out_msg;
            App_CanUtils_FsmApps_Unpack(msg->data, &out_msg);
            
            App_CanRx_FsmApps_PappsMappedPedalPercentage_Update(out_msg.PappsMappedPedalPercentage_value);
            App_CanRx_FsmApps_SappsMappedPedalPercentage_Update(out_msg.SappsMappedPedalPercentage_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FsmWarnings_Signals out_msg;
            App_CanUtils_FsmWarnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_FsmWarnings_PappsOutOfRange_Update(out_msg.PappsOutOfRange_value);
            App_CanRx_FsmWarnings_SappsOutOfRange_Update(out_msg.SappsOutOfRange_value);
            App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Update(out_msg.StackWatermarkAboveThresholdTask1Hz_value);
            App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Update(out_msg.StackWatermarkAboveThresholdTask1kHz_value);
            App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Update(out_msg.StackWatermarkAboveThresholdTaskCanRx_value);
            App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Update(out_msg.StackWatermarkAboveThresholdTaskCanTx_value);
            App_CanRx_FsmWarnings_WatchdogFault_Update(out_msg.WatchdogFault_value);
            App_CanRx_FsmWarnings_BspdFault_Update(out_msg.BspdFault_value);
            App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Update(out_msg.LeftWheelSpeedOutOfRange_value);
            App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Update(out_msg.RightWheelSpeedOutOfRange_value);
            App_CanRx_FsmWarnings_FlowRateOutOfRange_Update(out_msg.FlowRateOutOfRange_value);
            App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Update(out_msg.SteeringAngleOutOfRange_value);
            App_CanRx_FsmWarnings_BrakePressureOutOfRange_Update(out_msg.BrakePressureOutOfRange_value);
            App_CanRx_FsmWarnings_BrakePressureSc_Update(out_msg.BrakePressureSc_value);
            App_CanRx_FsmWarnings_BrakePressureOc_Update(out_msg.BrakePressureOc_value);
            App_CanRx_FsmWarnings_SteeringWheelBroke_Update(out_msg.SteeringWheelBroke_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            App_CanUtils_FSM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Warnings_FSM_TEST1_Update(out_msg.FSM_TEST1_value);
            App_CanRx_FSM_Warnings_FSM_TEST2_Update(out_msg.FSM_TEST2_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            App_CanUtils_FSM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Update(out_msg.FSM_FAULT_TEST3_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

