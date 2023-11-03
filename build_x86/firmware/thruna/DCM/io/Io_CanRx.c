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
        case CAN_MSG_BMS_VITALS_ID:
        case CAN_MSG_BMS_AVAILABLE_POWER_ID:
        case CAN_MSG_BMS_WARNINGS_ID:
        case CAN_MSG_BMS_FAULTS_ID:
        case CAN_MSG_FSM_VITALS_ID:
        case CAN_MSG_FSM_APPS_ID:
        case CAN_MSG_FSM_BRAKE_ID:
        case CAN_MSG_FSM_WARNINGS_ID:
        case CAN_MSG_FSM_FAULTS_ID:
        case CAN_MSG_INVL_MOTOR_POSITION_INFO_ID:
        case CAN_MSG_INVL_INTERNAL_STATES_ID:
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

