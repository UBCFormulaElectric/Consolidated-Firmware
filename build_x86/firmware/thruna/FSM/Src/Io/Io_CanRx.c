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
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_DCM_VITALS_ID:
        case CAN_MSG_DCM_LEFT_INVERTER_COMMAND_ID:
        case CAN_MSG_DCM_RIGHT_INVERTER_COMMAND_ID:
        case CAN_MSG_DCM_WARNINGS_ID:
        case CAN_MSG_DCM_FAULTS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
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

