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
        case CAN_MSG_DIM_VITALS_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_DCM_VITALS_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_PDM_VITALS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        case CAN_MSG_DEBUG_CHARGING_ID:
        case CAN_MSG_DEBUG_CELL_BALANCING_ID:
        case CAN_MSG_FSM_VITALS_ID:
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
            
            App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(out_msg.DIM_StackWaterMarkHighTask1HzWarning_value);
            App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(out_msg.DIM_StackWaterMarkHighTask100HzWarning_value);
            App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(out_msg.DIM_StackWaterMarkHighTask1kHzWarning_value);
            App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(out_msg.DIM_StackWaterMarkHighTaskCanRxWarning_value);
            App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(out_msg.DIM_StackWaterMarkHighTaskCanTxWarning_value);
            App_CanRx_DIM_WatchdogTimeoutWarning_Update(out_msg.DIM_WatchdogTimeoutWarning_value);
            App_CanRx_DIM_TxOverflowWarning_Update(out_msg.DIM_TxOverflowWarning_value);
            App_CanRx_DIM_RxOverflowWarning_Update(out_msg.DIM_RxOverflowWarning_value);
            break;
        }
        case CAN_MSG_DIM_FAULTS_ID:
        {
            DIM_Faults_Signals out_msg;
            App_CanUtils_DIM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_MissingHeartbeatFault_Update(out_msg.DIM_MissingHeartbeatFault_value);
            break;
        }
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
            
            App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Update(out_msg.DCM_StackWaterMarkHighTask1HzWarning_value);
            App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Update(out_msg.DCM_StackWaterMarkHighTask100HzWarning_value);
            App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Update(out_msg.DCM_StackWaterMarkHighTask1kHzWarning_value);
            App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Update(out_msg.DCM_StackWaterMarkHighTaskCanRxWarning_value);
            App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Update(out_msg.DCM_StackWaterMarkHighTaskCanTxWarning_value);
            App_CanRx_DCM_WatchdogTimeoutWarning_Update(out_msg.DCM_WatchdogTimeoutWarning_value);
            App_CanRx_DCM_TxOverflowWarning_Update(out_msg.DCM_TxOverflowWarning_value);
            App_CanRx_DCM_RxOverflowWarning_Update(out_msg.DCM_RxOverflowWarning_value);
            break;
        }
        case CAN_MSG_DCM_FAULTS_ID:
        {
            DCM_Faults_Signals out_msg;
            App_CanUtils_DCM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_MissingHeartbeatFault_Update(out_msg.DCM_MissingHeartbeatFault_value);
            App_CanRx_DCM_LeftInverterFault_Update(out_msg.DCM_LeftInverterFault_value);
            App_CanRx_DCM_RightInverterFault_Update(out_msg.DCM_RightInverterFault_value);
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
            
            App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(out_msg.PDM_StackWaterMarkHighTask1HzWarning_value);
            App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(out_msg.PDM_StackWaterMarkHighTask100HzWarning_value);
            App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(out_msg.PDM_StackWaterMarkHighTask1kHzWarning_value);
            App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(out_msg.PDM_StackWaterMarkHighTaskCanRxWarning_value);
            App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(out_msg.PDM_StackWaterMarkHighTaskCanTxWarning_value);
            App_CanRx_PDM_WatchdogTimeoutWarning_Update(out_msg.PDM_WatchdogTimeoutWarning_value);
            App_CanRx_PDM_TxOverflowWarning_Update(out_msg.PDM_TxOverflowWarning_value);
            App_CanRx_PDM_RxOverflowWarning_Update(out_msg.PDM_RxOverflowWarning_value);
            break;
        }
        case CAN_MSG_PDM_FAULTS_ID:
        {
            PDM_Faults_Signals out_msg;
            App_CanUtils_PDM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_PDM_DummyFault_Update(out_msg.PDM_DummyFault_value);
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
            
            App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(out_msg.FSM_StackWaterMarkHighTask1HzWarning_value);
            App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(out_msg.FSM_StackWaterMarkHighTask100HzWarning_value);
            App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(out_msg.FSM_StackWaterMarkHighTask1kHzWarning_value);
            App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(out_msg.FSM_StackWaterMarkHighTaskCanRxWarning_value);
            App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(out_msg.FSM_StackWaterMarkHighTaskCanTxWarning_value);
            App_CanRx_FSM_WatchdogTimeoutWarning_Update(out_msg.FSM_WatchdogTimeoutWarning_value);
            App_CanRx_FSM_TxOverflowWarning_Update(out_msg.FSM_TxOverflowWarning_value);
            App_CanRx_FSM_RxOverflowWarning_Update(out_msg.FSM_RxOverflowWarning_value);
            App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(out_msg.FSM_LeftWheelSpeedOutOfRangeWarning_value);
            App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(out_msg.FSM_RightWheelSpeedOutOfRangeWarning_value);
            App_CanRx_FSM_SteeringAngleOCSCWarning_Update(out_msg.FSM_SteeringAngleOCSCWarning_value);
            App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(out_msg.FSM_SteeringAngleOutOfRangeWarning_value);
            App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(out_msg.FSM_FrontBrakePressureOutOfRangeWarning_value);
            App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(out_msg.FSM_RearBrakePressureOutOfRangeWarning_value);
            App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(out_msg.FSM_FlowRateOutOfRangeWarning_value);
            App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(out_msg.FSM_BrakeAppsDisagreementWarning_value);
            App_CanRx_FSM_AppsDisagreementWarning_Update(out_msg.FSM_AppsDisagreementWarning_value);
            break;
        }
        case CAN_MSG_FSM_FAULTS_ID:
        {
            FSM_Faults_Signals out_msg;
            App_CanUtils_FSM_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_MissingHeartbeatFault_Update(out_msg.FSM_MissingHeartbeatFault_value);
            App_CanRx_FSM_StateMachineFault_Update(out_msg.FSM_StateMachineFault_value);
            App_CanRx_FSM_PappsOCSCFault_Update(out_msg.FSM_PappsOCSCFault_value);
            App_CanRx_FSM_SappsOCSCFault_Update(out_msg.FSM_SappsOCSCFault_value);
            App_CanRx_FSM_FlowMeterUnderflowFault_Update(out_msg.FSM_FlowMeterUnderflowFault_value);
            App_CanRx_FSM_TorquePlausabilityFailedFault_Update(out_msg.FSM_TorquePlausabilityFailedFault_value);
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
}

