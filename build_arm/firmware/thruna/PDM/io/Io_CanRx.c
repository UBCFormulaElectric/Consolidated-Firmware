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
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_CONTACTORS_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
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
        case CAN_MSG_BMS_VITALS_ID:
        {
            BMS_Vitals_Signals out_msg;
            App_CanUtils_BMS_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Heartbeat_Update(out_msg.BMS_Heartbeat_value);
            App_CanRx_BMS_State_Update(out_msg.BMS_State_value);
            break;
        }
        case CAN_MSG_BMS_CONTACTORS_ID:
        {
            BMS_Contactors_Signals out_msg;
            App_CanUtils_BMS_Contactors_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_AirPositive_Update(out_msg.BMS_AirPositive_value);
            App_CanRx_BMS_AirNegative_Update(out_msg.BMS_AirNegative_value);
            App_CanRx_BMS_PrechargeRelay_Update(out_msg.BMS_PrechargeRelay_value);
            break;
        }
        case CAN_MSG_BMS_WARNINGS_ID:
        {
            BMS_Warnings_Signals out_msg;
            App_CanUtils_BMS_Warnings_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(out_msg.BMS_Warning_StackWaterMarkHighTask1Hz_value);
            App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(out_msg.BMS_Warning_StackWaterMarkHighTask100Hz_value);
            App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(out_msg.BMS_Warning_StackWaterMarkHighTask1kHz_value);
            App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(out_msg.BMS_Warning_StackWaterMarkHighTaskCanRx_value);
            App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(out_msg.BMS_Warning_StackWaterMarkHighTaskCanTx_value);
            App_CanRx_BMS_Warning_WatchdogTimeout_Update(out_msg.BMS_Warning_WatchdogTimeout_value);
            App_CanRx_BMS_Warning_TxOverflow_Update(out_msg.BMS_Warning_TxOverflow_value);
            App_CanRx_BMS_Warning_RxOverflow_Update(out_msg.BMS_Warning_RxOverflow_value);
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            BMS_Faults_Signals out_msg;
            App_CanUtils_BMS_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_Fault_MissingHeartbeat_Update(out_msg.BMS_Fault_MissingHeartbeat_value);
            App_CanRx_BMS_Fault_StateMachine_Update(out_msg.BMS_Fault_StateMachine_value);
            App_CanRx_BMS_Fault_CellUndervoltage_Update(out_msg.BMS_Fault_CellUndervoltage_value);
            App_CanRx_BMS_Fault_CellOvervoltage_Update(out_msg.BMS_Fault_CellOvervoltage_value);
            App_CanRx_BMS_Fault_ModuleCommunicationError_Update(out_msg.BMS_Fault_ModuleCommunicationError_value);
            App_CanRx_BMS_Fault_CellUndertemp_Update(out_msg.BMS_Fault_CellUndertemp_value);
            App_CanRx_BMS_Fault_CellOvertemp_Update(out_msg.BMS_Fault_CellOvertemp_value);
            App_CanRx_BMS_Fault_Charger_Update(out_msg.BMS_Fault_Charger_value);
            App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(out_msg.BMS_Fault_ChargerDisconnectedDuringCharge_value);
            App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(out_msg.BMS_Fault_ChargerExternalShutdown_value);
            App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(out_msg.BMS_Fault_TractiveSystemOvercurrent_value);
            App_CanRx_BMS_Fault_PrechargeFailure_Update(out_msg.BMS_Fault_PrechargeFailure_value);
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
        default:
        {
            // Do nothing
            break;
        }
    }
}

