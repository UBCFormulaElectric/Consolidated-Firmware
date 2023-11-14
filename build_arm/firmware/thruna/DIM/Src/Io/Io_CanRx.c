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
        case CAN_MSG_INVR_TEMPERATURES1_ID:
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_OK_STATUSES_ID:
        case CAN_MSG_BMS_CONTACTORS_ID:
        case CAN_MSG_BMS_CELL_VOLTAGES_ID:
        case CAN_MSG_BMS_TRACTIVE_SYSTEM_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
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
        case CAN_MSG_INVR_TEMPERATURES1_ID:
        {
            INVR_Temperatures1_Signals out_msg;
            App_CanUtils_INVR_Temperatures1_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_ModuleATemperature_Update(out_msg.INVR_ModuleATemperature_value);
            App_CanRx_INVR_ModuleBTemperature_Update(out_msg.INVR_ModuleBTemperature_value);
            App_CanRx_INVR_ModuleCTemperature_Update(out_msg.INVR_ModuleCTemperature_value);
            App_CanRx_INVR_GateDriverBoardTemperature_Update(out_msg.INVR_GateDriverBoardTemperature_value);
            break;
        }
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        {
            INVR_MotorPositionInfo_Signals out_msg;
            App_CanUtils_INVR_MotorPositionInfo_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_MotorAngle_Update(out_msg.INVR_MotorAngle_value);
            App_CanRx_INVR_MotorSpeed_Update(out_msg.INVR_MotorSpeed_value);
            App_CanRx_INVR_ElectricalOutputFrequency_Update(out_msg.INVR_ElectricalOutputFrequency_value);
            App_CanRx_INVR_DeltaResolverFiltered_Update(out_msg.INVR_DeltaResolverFiltered_value);
            break;
        }
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        {
            INVL_MotorPositionInfo_Signals out_msg;
            App_CanUtils_INVL_MotorPositionInfo_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVL_MotorAngle_Update(out_msg.INVL_MotorAngle_value);
            App_CanRx_INVL_MotorSpeed_Update(out_msg.INVL_MotorSpeed_value);
            App_CanRx_INVL_ElectricalOutputFrequency_Update(out_msg.INVL_ElectricalOutputFrequency_value);
            App_CanRx_INVL_DeltaResolverFiltered_Update(out_msg.INVL_DeltaResolverFiltered_value);
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
        case CAN_MSG_BMS_OK_STATUSES_ID:
        {
            BMS_OkStatuses_Signals out_msg;
            App_CanUtils_BMS_OkStatuses_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_BmsOk_Update(out_msg.BMS_BmsOk_value);
            App_CanRx_BMS_ImdOk_Update(out_msg.BMS_ImdOk_value);
            App_CanRx_BMS_BspdOk_Update(out_msg.BMS_BspdOk_value);
            App_CanRx_BMS_BmsLatchedFault_Update(out_msg.BMS_BmsLatchedFault_value);
            App_CanRx_BMS_ImdLatchedFault_Update(out_msg.BMS_ImdLatchedFault_value);
            App_CanRx_BMS_BspdLatchedFault_Update(out_msg.BMS_BspdLatchedFault_value);
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
        case CAN_MSG_BMS_CELL_VOLTAGES_ID:
        {
            BMS_CellVoltages_Signals out_msg;
            App_CanUtils_BMS_CellVoltages_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_MinCellVoltage_Update(out_msg.BMS_MinCellVoltage_value);
            App_CanRx_BMS_MaxCellVoltage_Update(out_msg.BMS_MaxCellVoltage_value);
            break;
        }
        case CAN_MSG_BMS_TRACTIVE_SYSTEM_ID:
        {
            BMS_TractiveSystem_Signals out_msg;
            App_CanUtils_BMS_TractiveSystem_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_TractiveSystemVoltage_Update(out_msg.BMS_TractiveSystemVoltage_value);
            App_CanRx_BMS_TractiveSystemCurrent_Update(out_msg.BMS_TractiveSystemCurrent_value);
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

