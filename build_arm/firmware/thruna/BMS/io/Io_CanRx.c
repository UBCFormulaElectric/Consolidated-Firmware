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
        case CAN_MSG_DCM_VITALS_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_DIM_VITALS_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_FSM_VITALS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        case CAN_MSG_DEBUG_CHARGING_ID:
        case CAN_MSG_DEBUG_CELL_BALANCING_ID:
        case CAN_MSG_PDM_VITALS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
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

void Io_CanRx_UpdateRxTableWithMessage(CanMsg* msg)
{
    switch (msg->std_id)
    {
        case CAN_MSG_DCM_VITALS_ID:
        {
            DCM_Vitals_Signals out_msg;
            App_CanUtils_DCM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Heartbeat_Update(out_msg.DCM_Heartbeat_value);
            App_CanRx_DCM_State_Update(out_msg.DCM_State_value);
            break;
        }
        case CAN_MSG_DCM_WARNINGS_ID:
        {
            DCM_Warnings_Signals out_msg;
            App_CanUtils_DCM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Update(out_msg.DCM_Warning_StackWaterMarkHighTask1Hz_value);
            App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Update(out_msg.DCM_Warning_StackWaterMarkHighTask100Hz_value);
            App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Update(out_msg.DCM_Warning_StackWaterMarkHighTask1kHz_value);
            App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Update(out_msg.DCM_Warning_StackWaterMarkHighTaskCanRx_value);
            App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Update(out_msg.DCM_Warning_StackWaterMarkHighTaskCanTx_value);
            App_CanRx_DCM_Warning_WatchdogTimeout_Update(out_msg.DCM_Warning_WatchdogTimeout_value);
            App_CanRx_DCM_Warning_TxOverflow_Update(out_msg.DCM_Warning_TxOverflow_value);
            App_CanRx_DCM_Warning_RxOverflow_Update(out_msg.DCM_Warning_RxOverflow_value);
            break;
        }
        case CAN_MSG_DCM_FAULTS_ID:
        {
            DCM_Faults_Signals out_msg;
            App_CanUtils_DCM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_Fault_MissingHeartbeat_Update(out_msg.DCM_Fault_MissingHeartbeat_value);
            App_CanRx_DCM_Fault_LeftInverterFault_Update(out_msg.DCM_Fault_LeftInverterFault_value);
            App_CanRx_DCM_Fault_RightInverterFault_Update(out_msg.DCM_Fault_RightInverterFault_value);
            break;
        }
        case CAN_MSG_DIM_VITALS_ID:
        {
            DIM_Vitals_Signals out_msg;
            App_CanUtils_DIM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Heartbeat_Update(out_msg.DIM_Heartbeat_value);
            App_CanRx_DIM_State_Update(out_msg.DIM_State_value);
            break;
        }
        case CAN_MSG_DIM_WARNINGS_ID:
        {
            DIM_Warnings_Signals out_msg;
            App_CanUtils_DIM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(out_msg.DIM_Warning_StackWaterMarkHighTask1Hz_value);
            App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(out_msg.DIM_Warning_StackWaterMarkHighTask100Hz_value);
            App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(out_msg.DIM_Warning_StackWaterMarkHighTask1kHz_value);
            App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(out_msg.DIM_Warning_StackWaterMarkHighTaskCanRx_value);
            App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(out_msg.DIM_Warning_StackWaterMarkHighTaskCanTx_value);
            App_CanRx_DIM_Warning_WatchdogTimeout_Update(out_msg.DIM_Warning_WatchdogTimeout_value);
            App_CanRx_DIM_Warning_TxOverflow_Update(out_msg.DIM_Warning_TxOverflow_value);
            App_CanRx_DIM_Warning_RxOverflow_Update(out_msg.DIM_Warning_RxOverflow_value);
            break;
        }
        case CAN_MSG_DIM_FAULTS_ID:
        {
            DIM_Faults_Signals out_msg;
            App_CanUtils_DIM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_Fault_MissingHeartbeat_Update(out_msg.DIM_Fault_MissingHeartbeat_value);
            break;
        }
        case CAN_MSG_FSM_VITALS_ID:
        {
            FSM_Vitals_Signals out_msg;
            App_CanUtils_FSM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Heartbeat_Update(out_msg.FSM_Heartbeat_value);
            App_CanRx_FSM_State_Update(out_msg.FSM_State_value);
            break;
        }
        case CAN_MSG_FSM_WARNINGS_ID:
        {
            FSM_Warnings_Signals out_msg;
            App_CanUtils_FSM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(out_msg.FSM_Warning_StackWaterMarkHighTask1Hz_value);
            App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(out_msg.FSM_Warning_StackWaterMarkHighTask100Hz_value);
            App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(out_msg.FSM_Warning_StackWaterMarkHighTask1kHz_value);
            App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(out_msg.FSM_Warning_StackWaterMarkHighTaskCanRx_value);
            App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(out_msg.FSM_Warning_StackWaterMarkHighTaskCanTx_value);
            App_CanRx_FSM_Warning_WatchdogTimeout_Update(out_msg.FSM_Warning_WatchdogTimeout_value);
            App_CanRx_FSM_Warning_TxOverflow_Update(out_msg.FSM_Warning_TxOverflow_value);
            App_CanRx_FSM_Warning_RxOverflow_Update(out_msg.FSM_Warning_RxOverflow_value);
            App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(out_msg.FSM_Warning_LeftWheelSpeedOutOfRange_value);
            App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(out_msg.FSM_Warning_RightWheelSpeedOutOfRange_value);
            App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(out_msg.FSM_Warning_SteeringAngleOCSC_value);
            App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(out_msg.FSM_Warning_SteeringAngleOutOfRange_value);
            App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(out_msg.FSM_Warning_FrontBrakePressureOutOfRange_value);
            App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(out_msg.FSM_Warning_RearBrakePressureOutOfRangeWarning_value);
            App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(out_msg.FSM_Warning_FlowRateOutOfRange_value);
            App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(out_msg.FSM_Warning_BrakeAppsDisagreement_value);
            App_CanRx_FSM_Warning_AppsDisagreement_Update(out_msg.FSM_Warning_AppsDisagreement_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            App_CanUtils_FSM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Fault_MissingHeartbeat_Update(out_msg.FSM_Fault_MissingHeartbeat_value);
            App_CanRx_FSM_Fault_StateMachine_Update(out_msg.FSM_Fault_StateMachine_value);
            App_CanRx_FSM_Fault_PappsOCSC_Update(out_msg.FSM_Fault_PappsOCSC_value);
            App_CanRx_FSM_Fault_SappsOCSCFault_Update(out_msg.FSM_Fault_SappsOCSCFault_value);
            App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(out_msg.FSM_Fault_FlowMeterUnderflow_value);
            App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(out_msg.FSM_Fault_TorquePlausabilityFailed_value);
            break;
        }
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        {
            Debug_CanMode_Signals out_msg;
            App_CanUtils_Debug_CanMode_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_EnableDebugMode_Update(out_msg.Debug_EnableDebugMode_value);
            break;
        }
        case CAN_MSG_DEBUG_CHARGING_ID:
        {
            Debug_Charging_Signals out_msg;
            App_CanUtils_Debug_Charging_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_StartCharging_Update(out_msg.Debug_StartCharging_value);
            break;
        }
        case CAN_MSG_DEBUG_CELL_BALANCING_ID:
        {
            Debug_CellBalancing_Signals out_msg;
            App_CanUtils_Debug_CellBalancing_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_CellBalancingRequest_Update(out_msg.Debug_CellBalancingRequest_value);
            App_CanRx_Debug_CellBalancingOverrideTarget_Update(out_msg.Debug_CellBalancingOverrideTarget_value);
            App_CanRx_Debug_CellBalancingOverrideTargetValue_Update(out_msg.Debug_CellBalancingOverrideTargetValue_value);
            App_CanRx_Debug_CellBalancingOverridePWM_Update(out_msg.Debug_CellBalancingOverridePWM_value);
            App_CanRx_Debug_CellBalancingOverridePWMFrequency_Update(out_msg.Debug_CellBalancingOverridePWMFrequency_value);
            App_CanRx_Debug_CellBalancingOverridePWMDuty_Update(out_msg.Debug_CellBalancingOverridePWMDuty_value);
            break;
        }
        case CAN_MSG_PDM_VITALS_ID:
        {
            PDM_Vitals_Signals out_msg;
            App_CanUtils_PDM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_Heartbeat_Update(out_msg.PDM_Heartbeat_value);
            App_CanRx_PDM_State_Update(out_msg.PDM_State_value);
            break;
        }
        case CAN_MSG_PDM_WARNINGS_ID:
        {
            PDM_Warnings_Signals out_msg;
            App_CanUtils_PDM_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(out_msg.PDM_Warning_StackWaterMarkHighTask1Hz_value);
            App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(out_msg.PDM_Warning_StackWaterMarkHighTask100Hz_value);
            App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(out_msg.PDM_Warning_StackWaterMarkHighTask1kHz_value);
            App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(out_msg.PDM_Warning_StackWaterMarkHighTaskCanRx_value);
            App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(out_msg.PDM_Warning_StackWaterMarkHighTaskCanTx_value);
            App_CanRx_PDM_Warning_WatchdogTimeout_Update(out_msg.PDM_Warning_WatchdogTimeout_value);
            App_CanRx_PDM_Warning_TxOverflow_Update(out_msg.PDM_Warning_TxOverflow_value);
            App_CanRx_PDM_Warning_RxOverflow_Update(out_msg.PDM_Warning_RxOverflow_value);
            break;
        }
        case CAN_MSG_PDM_FAULTS_ID:
        {
            PDM_Faults_Signals out_msg;
            App_CanUtils_PDM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_Fault_DummyFault_Update(out_msg.PDM_Fault_DummyFault_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

