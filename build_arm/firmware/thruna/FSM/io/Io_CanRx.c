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
        case CAN_MSG_DCM_LEFT_INVERTER_COMMAND_ID:
        case CAN_MSG_DCM_RIGHT_INVERTER_COMMAND_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
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
        case CAN_MSG_DCM_LEFT_INVERTER_COMMAND_ID:
        {
            DCM_LeftInverterCommand_Signals out_msg;
            App_CanUtils_DCM_LeftInverterCommand_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_LeftInverterTorqueCommand_Update(out_msg.DCM_LeftInverterTorqueCommand_value);
            App_CanRx_DCM_LeftInverterSpeedCommand_Update(out_msg.DCM_LeftInverterSpeedCommand_value);
            App_CanRx_DCM_LeftInverterDirectionCommand_Update(out_msg.DCM_LeftInverterDirectionCommand_value);
            App_CanRx_DCM_LeftInverterEnable_Update(out_msg.DCM_LeftInverterEnable_value);
            App_CanRx_DCM_LeftInverterEnableDischarge_Update(out_msg.DCM_LeftInverterEnableDischarge_value);
            App_CanRx_DCM_LeftInverterEnableSpeedMode_Update(out_msg.DCM_LeftInverterEnableSpeedMode_value);
            App_CanRx_DCM_LeftInverterTorqueLimit_Update(out_msg.DCM_LeftInverterTorqueLimit_value);
            break;
        }
        case CAN_MSG_DCM_RIGHT_INVERTER_COMMAND_ID:
        {
            DCM_RightInverterCommand_Signals out_msg;
            App_CanUtils_DCM_RightInverterCommand_Unpack(msg->data, &out_msg);
            
            App_CanRx_DCM_RightInverterTorqueCommand_Update(out_msg.DCM_RightInverterTorqueCommand_value);
            App_CanRx_DCM_RightInverterSpeedCommand_Update(out_msg.DCM_RightInverterSpeedCommand_value);
            App_CanRx_DCM_RightInverterDirectionCommand_Update(out_msg.DCM_RightInverterDirectionCommand_value);
            App_CanRx_DCM_RightInverterEnable_Update(out_msg.DCM_RightInverterEnable_value);
            App_CanRx_DCM_RightInverterEnableDischarge_Update(out_msg.DCM_RightInverterEnableDischarge_value);
            App_CanRx_DCM_RightInverterEnableSpeedMode_Update(out_msg.DCM_RightInverterEnableSpeedMode_value);
            App_CanRx_DCM_RightInverterTorqueLimit_Update(out_msg.DCM_RightInverterTorqueLimit_value);
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
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        {
            Debug_CanMode_Signals out_msg;
            App_CanUtils_Debug_CanMode_Unpack(msg->data, &out_msg);
            
            App_CanRx_Debug_EnableDebugMode_Update(out_msg.Debug_EnableDebugMode_value);
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

