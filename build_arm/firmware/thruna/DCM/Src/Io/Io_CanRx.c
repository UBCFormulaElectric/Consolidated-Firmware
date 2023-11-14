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
        case CAN_MSG_DIM_SWITCHES_ID:
        case CAN_MSG_DIM_WARNINGS_ID:
        case CAN_MSG_DIM_FAULTS_ID:
        case CAN_MSG_INVR_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVR_INTERNAL_STATES_ID:
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVL_INTERNAL_STATES_ID:
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_AVAILABLE_POWER_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_PDM_WARNINGS_ID:
        case CAN_MSG_PDM_FAULTS_ID:
        case CAN_MSG_DEBUG_CAN_MODE_ID:
        case CAN_MSG_FSM_VITALS_ID:
        case CAN_MSG_FSM_APPS_ID:
        case CAN_MSG_FSM_BRAKE_ID:
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
        case CAN_MSG_DIM_SWITCHES_ID:
        {
            DIM_Switches_Signals out_msg;
            App_CanUtils_DIM_Switches_Unpack(msg->data, &out_msg);
            
            App_CanRx_DIM_StartSwitch_Update(out_msg.DIM_StartSwitch_value);
            App_CanRx_DIM_AuxSwitch_Update(out_msg.DIM_AuxSwitch_value);
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
        case CAN_MSG_INVR_INTERNAL_STATES_ID:
        {
            INVR_InternalStates_Signals out_msg;
            App_CanUtils_INVR_InternalStates_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVR_VsmState_Update(out_msg.INVR_VsmState_value);
            App_CanRx_INVR_PwmFrequency_Update(out_msg.INVR_PwmFrequency_value);
            App_CanRx_INVR_InverterState_Update(out_msg.INVR_InverterState_value);
            App_CanRx_INVR_Relay1State_Update(out_msg.INVR_Relay1State_value);
            App_CanRx_INVR_Relay2State_Update(out_msg.INVR_Relay2State_value);
            App_CanRx_INVR_Relay3State_Update(out_msg.INVR_Relay3State_value);
            App_CanRx_INVR_Relay4State_Update(out_msg.INVR_Relay4State_value);
            App_CanRx_INVR_Relay5State_Update(out_msg.INVR_Relay5State_value);
            App_CanRx_INVR_Relay6State_Update(out_msg.INVR_Relay6State_value);
            App_CanRx_INVR_InverterRunMode_Update(out_msg.INVR_InverterRunMode_value);
            App_CanRx_INVR_InverterActiveDischargeState_Update(out_msg.INVR_InverterActiveDischargeState_value);
            App_CanRx_INVR_InverterCommandMode_Update(out_msg.INVR_InverterCommandMode_value);
            App_CanRx_INVR_RollingCounter_Update(out_msg.INVR_RollingCounter_value);
            App_CanRx_INVR_InverterEnableState_Update(out_msg.INVR_InverterEnableState_value);
            App_CanRx_INVR_StartModeActive_Update(out_msg.INVR_StartModeActive_value);
            App_CanRx_INVR_InverterLockoutEnabled_Update(out_msg.INVR_InverterLockoutEnabled_value);
            App_CanRx_INVR_DirectionCommand_Update(out_msg.INVR_DirectionCommand_value);
            App_CanRx_INVR_BmsActive_Update(out_msg.INVR_BmsActive_value);
            App_CanRx_INVR_BmsTorqueLimiting_Update(out_msg.INVR_BmsTorqueLimiting_value);
            App_CanRx_INVR_MaxSpeedLimiting_Update(out_msg.INVR_MaxSpeedLimiting_value);
            App_CanRx_INVR_HotSpotLimiting_Update(out_msg.INVR_HotSpotLimiting_value);
            App_CanRx_INVR_LowSpeedLimiting_Update(out_msg.INVR_LowSpeedLimiting_value);
            App_CanRx_INVR_CoolantTemperatureLimiting_Update(out_msg.INVR_CoolantTemperatureLimiting_value);
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
        case CAN_MSG_INVL_INTERNAL_STATES_ID:
        {
            INVL_InternalStates_Signals out_msg;
            App_CanUtils_INVL_InternalStates_Unpack(msg->data, &out_msg);
            
            App_CanRx_INVL_VsmState_Update(out_msg.INVL_VsmState_value);
            App_CanRx_INVL_PwmFrequency_Update(out_msg.INVL_PwmFrequency_value);
            App_CanRx_INVL_InverterState_Update(out_msg.INVL_InverterState_value);
            App_CanRx_INVL_Relay1State_Update(out_msg.INVL_Relay1State_value);
            App_CanRx_INVL_Relay2State_Update(out_msg.INVL_Relay2State_value);
            App_CanRx_INVL_Relay3State_Update(out_msg.INVL_Relay3State_value);
            App_CanRx_INVL_Relay4State_Update(out_msg.INVL_Relay4State_value);
            App_CanRx_INVL_Relay5State_Update(out_msg.INVL_Relay5State_value);
            App_CanRx_INVL_Relay6State_Update(out_msg.INVL_Relay6State_value);
            App_CanRx_INVL_InverterRunMode_Update(out_msg.INVL_InverterRunMode_value);
            App_CanRx_INVL_InverterActiveDischargeState_Update(out_msg.INVL_InverterActiveDischargeState_value);
            App_CanRx_INVL_InverterCommandMode_Update(out_msg.INVL_InverterCommandMode_value);
            App_CanRx_INVL_RollingCounter_Update(out_msg.INVL_RollingCounter_value);
            App_CanRx_INVL_InverterEnableState_Update(out_msg.INVL_InverterEnableState_value);
            App_CanRx_INVL_StartModeActive_Update(out_msg.INVL_StartModeActive_value);
            App_CanRx_INVL_InverterLockoutEnabled_Update(out_msg.INVL_InverterLockoutEnabled_value);
            App_CanRx_INVL_DirectionCommand_Update(out_msg.INVL_DirectionCommand_value);
            App_CanRx_INVL_BmsActive_Update(out_msg.INVL_BmsActive_value);
            App_CanRx_INVL_BmsTorqueLimiting_Update(out_msg.INVL_BmsTorqueLimiting_value);
            App_CanRx_INVL_MaxSpeedLimiting_Update(out_msg.INVL_MaxSpeedLimiting_value);
            App_CanRx_INVL_HotSpotLimiting_Update(out_msg.INVL_HotSpotLimiting_value);
            App_CanRx_INVL_LowSpeedLimiting_Update(out_msg.INVL_LowSpeedLimiting_value);
            App_CanRx_INVL_CoolantTemperatureLimiting_Update(out_msg.INVL_CoolantTemperatureLimiting_value);
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
        case CAN_MSG_BMS_AVAILABLE_POWER_ID:
        {
            BMS_AvailablePower_Signals out_msg;
            App_CanUtils_BMS_AvailablePower_Unpack(msg->data, &out_msg);
            
            App_CanRx_BMS_AvailablePower_Update(out_msg.BMS_AvailablePower_value);
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
        case CAN_MSG_FSM_VITALS_ID:
        {
            FSM_Vitals_Signals out_msg;
            App_CanUtils_FSM_Vitals_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_Heartbeat_Update(out_msg.FSM_Heartbeat_value);
            App_CanRx_FSM_State_Update(out_msg.FSM_State_value);
            break;
        }
        case CAN_MSG_FSM_APPS_ID:
        {
            FSM_Apps_Signals out_msg;
            App_CanUtils_FSM_Apps_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_PappsMappedPedalPercentage_Update(out_msg.FSM_PappsMappedPedalPercentage_value);
            App_CanRx_FSM_SappsMappedPedalPercentage_Update(out_msg.FSM_SappsMappedPedalPercentage_value);
            App_CanRx_FSM_PappsRawPedalPercentage_Update(out_msg.FSM_PappsRawPedalPercentage_value);
            App_CanRx_FSM_SappsRawPedalPercentage_Update(out_msg.FSM_SappsRawPedalPercentage_value);
            App_CanRx_FSM_TorqueLimit_Update(out_msg.FSM_TorqueLimit_value);
            App_CanRx_FSM_TorquePlausabilityFailed_Update(out_msg.FSM_TorquePlausabilityFailed_value);
            break;
        }
        case CAN_MSG_FSM_BRAKE_ID:
        {
            FSM_Brake_Signals out_msg;
            App_CanUtils_FSM_Brake_Unpack(msg->data, &out_msg);
            
            App_CanRx_FSM_FrontBrakePressure_Update(out_msg.FSM_FrontBrakePressure_value);
            App_CanRx_FSM_RearBrakePressure_Update(out_msg.FSM_RearBrakePressure_value);
            App_CanRx_FSM_BrakePedalPercentage_Update(out_msg.FSM_BrakePedalPercentage_value);
            App_CanRx_FSM_BrakeActuated_Update(out_msg.FSM_BrakeActuated_value);
            App_CanRx_FSM_BrakePressureSensorOCSC_Update(out_msg.FSM_BrakePressureSensorOCSC_value);
            App_CanRx_FSM_BrakePedalSensorOCSC_Update(out_msg.FSM_BrakePedalSensorOCSC_value);
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

