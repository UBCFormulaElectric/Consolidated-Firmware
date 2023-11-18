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
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_CONTACTORS_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
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
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        {
            Debug_CanMode_Signals out_msg;
            App_CanUtils_Debug_CanMode_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_EnableDebugMode_Update(out_msg.Debug_EnableDebugMode_value);
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
            
            App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(out_msg.BMS_StackWaterMarkHighTask1HzWarning_value);
            App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(out_msg.BMS_StackWaterMarkHighTask100HzWarning_value);
            App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(out_msg.BMS_StackWaterMarkHighTask1kHzWarning_value);
            App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(out_msg.BMS_StackWaterMarkHighTaskCanRxWarning_value);
            App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(out_msg.BMS_StackWaterMarkHighTaskCanTxWarning_value);
            App_CanRx_BMS_WatchdogTimeoutWarning_Update(out_msg.BMS_WatchdogTimeoutWarning_value);
            App_CanRx_BMS_TxOverflowWarning_Update(out_msg.BMS_TxOverflowWarning_value);
            App_CanRx_BMS_RxOverflowWarning_Update(out_msg.BMS_RxOverflowWarning_value);
            break;
        }
        case CAN_MSG_BMS_FAULTS_ID:
        {
            BMS_Faults_Signals out_msg;
            App_CanUtils_BMS_Faults_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_MissingHeartbeatFault_Update(out_msg.BMS_MissingHeartbeatFault_value);
            App_CanRx_BMS_StateMachineFault_Update(out_msg.BMS_StateMachineFault_value);
            App_CanRx_BMS_CellUndervoltageFault_Update(out_msg.BMS_CellUndervoltageFault_value);
            App_CanRx_BMS_CellOvervoltageFault_Update(out_msg.BMS_CellOvervoltageFault_value);
            App_CanRx_BMS_ModuleCommunicationFault_Update(out_msg.BMS_ModuleCommunicationFault_value);
            App_CanRx_BMS_CellUndertempFault_Update(out_msg.BMS_CellUndertempFault_value);
            App_CanRx_BMS_CellOvertempFault_Update(out_msg.BMS_CellOvertempFault_value);
            App_CanRx_BMS_ChargerFault_Update(out_msg.BMS_ChargerFault_value);
            App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(out_msg.BMS_ChargerDisconnectedDuringChargeFault_value);
            App_CanRx_BMS_ChargerExternalShutdownFault_Update(out_msg.BMS_ChargerExternalShutdownFault_value);
            App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(out_msg.BMS_TractiveSystemOvercurrentFault_value);
            App_CanRx_BMS_PrechargeFailureFault_Update(out_msg.BMS_PrechargeFailureFault_value);
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
        default:
        {
            // Do nothing
            break;
        }
    }
}

