/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void App_CanRx_Init(void);

/**
 * Update value stored in RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
void App_CanRx_DIM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_State in msg DIM_Vitals.
 */
void App_CanRx_DIM_State_Update(DimState value);

/**
 * Update value stored in RX table of signal DIM_StartSwitch in msg DIM_Switches.
 */
void App_CanRx_DIM_StartSwitch_Update(SwitchState value);

/**
 * Update value stored in RX table of signal DIM_AuxSwitch in msg DIM_Switches.
 */
void App_CanRx_DIM_AuxSwitch_Update(SwitchState value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
void App_CanRx_DIM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
void App_CanRx_DIM_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_MotorAngle in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorAngle_Update(float value);

/**
 * Update value stored in RX table of signal INVR_MotorSpeed in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_MotorSpeed_Update(int value);

/**
 * Update value stored in RX table of signal INVR_ElectricalOutputFrequency in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_ElectricalOutputFrequency_Update(float value);

/**
 * Update value stored in RX table of signal INVR_DeltaResolverFiltered in msg INVR_MotorPositionInfo.
 */
void App_CanRx_INVR_DeltaResolverFiltered_Update(float value);

/**
 * Update value stored in RX table of signal INVR_VsmState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_VsmState_Update(InverterVsmState value);

/**
 * Update value stored in RX table of signal INVR_PwmFrequency in msg INVR_InternalStates.
 */
void App_CanRx_INVR_PwmFrequency_Update(uint32_t value);

/**
 * Update value stored in RX table of signal INVR_InverterState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterState_Update(InverterState value);

/**
 * Update value stored in RX table of signal INVR_Relay1State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay1State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_Relay2State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay2State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_Relay3State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay3State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_Relay4State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay4State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_Relay5State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay5State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_Relay6State in msg INVR_InternalStates.
 */
void App_CanRx_INVR_Relay6State_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_InverterRunMode in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterRunMode_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_InverterActiveDischargeState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterActiveDischargeState_Update(InverterActiveDischargeState value);

/**
 * Update value stored in RX table of signal INVR_InverterCommandMode in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterCommandMode_Update(InverterCommandMode value);

/**
 * Update value stored in RX table of signal INVR_RollingCounter in msg INVR_InternalStates.
 */
void App_CanRx_INVR_RollingCounter_Update(uint32_t value);

/**
 * Update value stored in RX table of signal INVR_InverterEnableState in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterEnableState_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_StartModeActive in msg INVR_InternalStates.
 */
void App_CanRx_INVR_StartModeActive_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_InverterLockoutEnabled in msg INVR_InternalStates.
 */
void App_CanRx_INVR_InverterLockoutEnabled_Update(InverterLockout value);

/**
 * Update value stored in RX table of signal INVR_DirectionCommand in msg INVR_InternalStates.
 */
void App_CanRx_INVR_DirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal INVR_BmsActive in msg INVR_InternalStates.
 */
void App_CanRx_INVR_BmsActive_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_BmsTorqueLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_BmsTorqueLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_MaxSpeedLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_MaxSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_HotSpotLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_HotSpotLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_LowSpeedLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_LowSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVR_CoolantTemperatureLimiting in msg INVR_InternalStates.
 */
void App_CanRx_INVR_CoolantTemperatureLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void App_CanRx_BMS_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
void App_CanRx_BMS_State_Update(BmsState value);

/**
 * Update value stored in RX table of signal BMS_AvailablePower in msg BMS_AvailablePower.
 */
void App_CanRx_BMS_AvailablePower_Update(float value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
void App_CanRx_BMS_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_StateMachine_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellUndervoltage_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellOvervoltage_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ModuleCommunicationError_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellUndertemp_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_CellOvertemp_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_Charger_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_ChargerExternalShutdown_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
void App_CanRx_BMS_Fault_PrechargeFailure_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
void App_CanRx_FSM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_State in msg FSM_Vitals.
 */
void App_CanRx_FSM_State_Update(State_Machine value);

/**
 * Update value stored in RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_PappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_SappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_PappsRawPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_PappsRawPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_SappsRawPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_SappsRawPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_TorqueLimit in msg FSM_Apps.
 */
void App_CanRx_FSM_TorqueLimit_Update(float value);

/**
 * Update value stored in RX table of signal FSM_TorquePlausabilityFailed in msg FSM_Apps.
 */
void App_CanRx_FSM_TorquePlausabilityFailed_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FrontBrakePressure in msg FSM_Brake.
 */
void App_CanRx_FSM_FrontBrakePressure_Update(uint32_t value);

/**
 * Update value stored in RX table of signal FSM_RearBrakePressure in msg FSM_Brake.
 */
void App_CanRx_FSM_RearBrakePressure_Update(uint32_t value);

/**
 * Update value stored in RX table of signal FSM_BrakePedalPercentage in msg FSM_Brake.
 */
void App_CanRx_FSM_BrakePedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_BrakeActuated in msg FSM_Brake.
 */
void App_CanRx_FSM_BrakeActuated_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_BrakePressureSensorOCSC in msg FSM_Brake.
 */
void App_CanRx_FSM_BrakePressureSensorOCSC_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_BrakePedalSensorOCSC in msg FSM_Brake.
 */
void App_CanRx_FSM_BrakePedalSensorOCSC_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_SteeringAngleOCSC_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_FlowRateOutOfRange in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_FlowRateOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_BrakeAppsDisagreement in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_BrakeAppsDisagreement_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_AppsDisagreement_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_MissingHeartbeat in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_StateMachine in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_StateMachine_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_PappsOCSC_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_SappsOCSCFault in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_SappsOCSCFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_FlowMeterUnderflow in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_FlowMeterUnderflow_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_TorquePlausabilityFailed in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_TorquePlausabilityFailed_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_MotorAngle in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorAngle_Update(float value);

/**
 * Update value stored in RX table of signal INVL_MotorSpeed in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_MotorSpeed_Update(int value);

/**
 * Update value stored in RX table of signal INVL_ElectricalOutputFrequency in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_ElectricalOutputFrequency_Update(float value);

/**
 * Update value stored in RX table of signal INVL_DeltaResolverFiltered in msg INVL_MotorPositionInfo.
 */
void App_CanRx_INVL_DeltaResolverFiltered_Update(float value);

/**
 * Update value stored in RX table of signal INVL_VsmState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_VsmState_Update(InverterVsmState value);

/**
 * Update value stored in RX table of signal INVL_PwmFrequency in msg INVL_InternalStates.
 */
void App_CanRx_INVL_PwmFrequency_Update(uint32_t value);

/**
 * Update value stored in RX table of signal INVL_InverterState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterState_Update(InverterState value);

/**
 * Update value stored in RX table of signal INVL_Relay1State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay1State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_Relay2State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay2State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_Relay3State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay3State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_Relay4State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay4State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_Relay5State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay5State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_Relay6State in msg INVL_InternalStates.
 */
void App_CanRx_INVL_Relay6State_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_InverterRunMode in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterRunMode_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_InverterActiveDischargeState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterActiveDischargeState_Update(InverterActiveDischargeState value);

/**
 * Update value stored in RX table of signal INVL_InverterCommandMode in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterCommandMode_Update(InverterCommandMode value);

/**
 * Update value stored in RX table of signal INVL_RollingCounter in msg INVL_InternalStates.
 */
void App_CanRx_INVL_RollingCounter_Update(uint32_t value);

/**
 * Update value stored in RX table of signal INVL_InverterEnableState in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterEnableState_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_StartModeActive in msg INVL_InternalStates.
 */
void App_CanRx_INVL_StartModeActive_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_InverterLockoutEnabled in msg INVL_InternalStates.
 */
void App_CanRx_INVL_InverterLockoutEnabled_Update(InverterLockout value);

/**
 * Update value stored in RX table of signal INVL_DirectionCommand in msg INVL_InternalStates.
 */
void App_CanRx_INVL_DirectionCommand_Update(InverterDirection value);

/**
 * Update value stored in RX table of signal INVL_BmsActive in msg INVL_InternalStates.
 */
void App_CanRx_INVL_BmsActive_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_BmsTorqueLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_BmsTorqueLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_MaxSpeedLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_MaxSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_HotSpotLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_HotSpotLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_LowSpeedLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_LowSpeedLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal INVL_CoolantTemperatureLimiting in msg INVL_InternalStates.
 */
void App_CanRx_INVL_CoolantTemperatureLimiting_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
void App_CanRx_Debug_EnableDebugMode_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
void App_CanRx_PDM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
void App_CanRx_PDM_Fault_DummyFault_Update(bool value);

/**
 * Return value from RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
bool App_CanRx_DIM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DIM_State in msg DIM_Vitals.
 */
DimState App_CanRx_DIM_State_Get(void);

/**
 * Return value from RX table of signal DIM_StartSwitch in msg DIM_Switches.
 */
SwitchState App_CanRx_DIM_StartSwitch_Get(void);

/**
 * Return value from RX table of signal DIM_AuxSwitch in msg DIM_Switches.
 */
SwitchState App_CanRx_DIM_AuxSwitch_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
bool App_CanRx_DIM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
bool App_CanRx_DIM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal INVR_MotorAngle in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_MotorAngle_Get(void);

/**
 * Return value from RX table of signal INVR_MotorSpeed in msg INVR_MotorPositionInfo.
 */
int App_CanRx_INVR_MotorSpeed_Get(void);

/**
 * Return value from RX table of signal INVR_ElectricalOutputFrequency in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_ElectricalOutputFrequency_Get(void);

/**
 * Return value from RX table of signal INVR_DeltaResolverFiltered in msg INVR_MotorPositionInfo.
 */
float App_CanRx_INVR_DeltaResolverFiltered_Get(void);

/**
 * Return value from RX table of signal INVR_VsmState in msg INVR_InternalStates.
 */
InverterVsmState App_CanRx_INVR_VsmState_Get(void);

/**
 * Return value from RX table of signal INVR_PwmFrequency in msg INVR_InternalStates.
 */
uint32_t App_CanRx_INVR_PwmFrequency_Get(void);

/**
 * Return value from RX table of signal INVR_InverterState in msg INVR_InternalStates.
 */
InverterState App_CanRx_INVR_InverterState_Get(void);

/**
 * Return value from RX table of signal INVR_Relay1State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay1State_Get(void);

/**
 * Return value from RX table of signal INVR_Relay2State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay2State_Get(void);

/**
 * Return value from RX table of signal INVR_Relay3State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay3State_Get(void);

/**
 * Return value from RX table of signal INVR_Relay4State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay4State_Get(void);

/**
 * Return value from RX table of signal INVR_Relay5State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay5State_Get(void);

/**
 * Return value from RX table of signal INVR_Relay6State in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_Relay6State_Get(void);

/**
 * Return value from RX table of signal INVR_InverterRunMode in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InverterRunMode_Get(void);

/**
 * Return value from RX table of signal INVR_InverterActiveDischargeState in msg INVR_InternalStates.
 */
InverterActiveDischargeState App_CanRx_INVR_InverterActiveDischargeState_Get(void);

/**
 * Return value from RX table of signal INVR_InverterCommandMode in msg INVR_InternalStates.
 */
InverterCommandMode App_CanRx_INVR_InverterCommandMode_Get(void);

/**
 * Return value from RX table of signal INVR_RollingCounter in msg INVR_InternalStates.
 */
uint32_t App_CanRx_INVR_RollingCounter_Get(void);

/**
 * Return value from RX table of signal INVR_InverterEnableState in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_InverterEnableState_Get(void);

/**
 * Return value from RX table of signal INVR_StartModeActive in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_StartModeActive_Get(void);

/**
 * Return value from RX table of signal INVR_InverterLockoutEnabled in msg INVR_InternalStates.
 */
InverterLockout App_CanRx_INVR_InverterLockoutEnabled_Get(void);

/**
 * Return value from RX table of signal INVR_DirectionCommand in msg INVR_InternalStates.
 */
InverterDirection App_CanRx_INVR_DirectionCommand_Get(void);

/**
 * Return value from RX table of signal INVR_BmsActive in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_BmsActive_Get(void);

/**
 * Return value from RX table of signal INVR_BmsTorqueLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_BmsTorqueLimiting_Get(void);

/**
 * Return value from RX table of signal INVR_MaxSpeedLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_MaxSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal INVR_HotSpotLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_HotSpotLimiting_Get(void);

/**
 * Return value from RX table of signal INVR_LowSpeedLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_LowSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal INVR_CoolantTemperatureLimiting in msg INVR_InternalStates.
 */
bool App_CanRx_INVR_CoolantTemperatureLimiting_Get(void);

/**
 * Return value from RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool App_CanRx_BMS_Heartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState App_CanRx_BMS_State_Get(void);

/**
 * Return value from RX table of signal BMS_AvailablePower in msg BMS_AvailablePower.
 */
float App_CanRx_BMS_AvailablePower_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
bool App_CanRx_BMS_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_StateMachine_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellUndervoltage_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellOvervoltage_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ModuleCommunicationError_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellUndertemp_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_CellOvertemp_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_Charger_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ChargerDisconnectedDuringCharge_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_ChargerExternalShutdown_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_TractiveSystemOvercurrent_Get(void);

/**
 * Return value from RX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
bool App_CanRx_BMS_Fault_PrechargeFailure_Get(void);

/**
 * Return value from RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
bool App_CanRx_FSM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal FSM_State in msg FSM_Vitals.
 */
State_Machine App_CanRx_FSM_State_Get(void);

/**
 * Return value from RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_PappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_SappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_PappsRawPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_PappsRawPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_SappsRawPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_SappsRawPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_TorqueLimit in msg FSM_Apps.
 */
float App_CanRx_FSM_TorqueLimit_Get(void);

/**
 * Return value from RX table of signal FSM_TorquePlausabilityFailed in msg FSM_Apps.
 */
bool App_CanRx_FSM_TorquePlausabilityFailed_Get(void);

/**
 * Return value from RX table of signal FSM_FrontBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanRx_FSM_FrontBrakePressure_Get(void);

/**
 * Return value from RX table of signal FSM_RearBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanRx_FSM_RearBrakePressure_Get(void);

/**
 * Return value from RX table of signal FSM_BrakePedalPercentage in msg FSM_Brake.
 */
float App_CanRx_FSM_BrakePedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_BrakeActuated in msg FSM_Brake.
 */
bool App_CanRx_FSM_BrakeActuated_Get(void);

/**
 * Return value from RX table of signal FSM_BrakePressureSensorOCSC in msg FSM_Brake.
 */
bool App_CanRx_FSM_BrakePressureSensorOCSC_Get(void);

/**
 * Return value from RX table of signal FSM_BrakePedalSensorOCSC in msg FSM_Brake.
 */
bool App_CanRx_FSM_BrakePedalSensorOCSC_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_LeftWheelSpeedOutOfRange_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_RightWheelSpeedOutOfRange_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_SteeringAngleOCSC_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_SteeringAngleOutOfRange_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_FrontBrakePressureOutOfRange_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_FlowRateOutOfRange in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_FlowRateOutOfRange_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_BrakeAppsDisagreement in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_BrakeAppsDisagreement_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_AppsDisagreement_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_MissingHeartbeat in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_StateMachine in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_StateMachine_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_PappsOCSC_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_SappsOCSCFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_SappsOCSCFault_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_FlowMeterUnderflow in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_FlowMeterUnderflow_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_TorquePlausabilityFailed in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_TorquePlausabilityFailed_Get(void);

/**
 * Return value from RX table of signal INVL_MotorAngle in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_MotorAngle_Get(void);

/**
 * Return value from RX table of signal INVL_MotorSpeed in msg INVL_MotorPositionInfo.
 */
int App_CanRx_INVL_MotorSpeed_Get(void);

/**
 * Return value from RX table of signal INVL_ElectricalOutputFrequency in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_ElectricalOutputFrequency_Get(void);

/**
 * Return value from RX table of signal INVL_DeltaResolverFiltered in msg INVL_MotorPositionInfo.
 */
float App_CanRx_INVL_DeltaResolverFiltered_Get(void);

/**
 * Return value from RX table of signal INVL_VsmState in msg INVL_InternalStates.
 */
InverterVsmState App_CanRx_INVL_VsmState_Get(void);

/**
 * Return value from RX table of signal INVL_PwmFrequency in msg INVL_InternalStates.
 */
uint32_t App_CanRx_INVL_PwmFrequency_Get(void);

/**
 * Return value from RX table of signal INVL_InverterState in msg INVL_InternalStates.
 */
InverterState App_CanRx_INVL_InverterState_Get(void);

/**
 * Return value from RX table of signal INVL_Relay1State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay1State_Get(void);

/**
 * Return value from RX table of signal INVL_Relay2State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay2State_Get(void);

/**
 * Return value from RX table of signal INVL_Relay3State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay3State_Get(void);

/**
 * Return value from RX table of signal INVL_Relay4State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay4State_Get(void);

/**
 * Return value from RX table of signal INVL_Relay5State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay5State_Get(void);

/**
 * Return value from RX table of signal INVL_Relay6State in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_Relay6State_Get(void);

/**
 * Return value from RX table of signal INVL_InverterRunMode in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InverterRunMode_Get(void);

/**
 * Return value from RX table of signal INVL_InverterActiveDischargeState in msg INVL_InternalStates.
 */
InverterActiveDischargeState App_CanRx_INVL_InverterActiveDischargeState_Get(void);

/**
 * Return value from RX table of signal INVL_InverterCommandMode in msg INVL_InternalStates.
 */
InverterCommandMode App_CanRx_INVL_InverterCommandMode_Get(void);

/**
 * Return value from RX table of signal INVL_RollingCounter in msg INVL_InternalStates.
 */
uint32_t App_CanRx_INVL_RollingCounter_Get(void);

/**
 * Return value from RX table of signal INVL_InverterEnableState in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_InverterEnableState_Get(void);

/**
 * Return value from RX table of signal INVL_StartModeActive in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_StartModeActive_Get(void);

/**
 * Return value from RX table of signal INVL_InverterLockoutEnabled in msg INVL_InternalStates.
 */
InverterLockout App_CanRx_INVL_InverterLockoutEnabled_Get(void);

/**
 * Return value from RX table of signal INVL_DirectionCommand in msg INVL_InternalStates.
 */
InverterDirection App_CanRx_INVL_DirectionCommand_Get(void);

/**
 * Return value from RX table of signal INVL_BmsActive in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_BmsActive_Get(void);

/**
 * Return value from RX table of signal INVL_BmsTorqueLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_BmsTorqueLimiting_Get(void);

/**
 * Return value from RX table of signal INVL_MaxSpeedLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_MaxSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal INVL_HotSpotLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_HotSpotLimiting_Get(void);

/**
 * Return value from RX table of signal INVL_LowSpeedLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_LowSpeedLimiting_Get(void);

/**
 * Return value from RX table of signal INVL_CoolantTemperatureLimiting in msg INVL_InternalStates.
 */
bool App_CanRx_INVL_CoolantTemperatureLimiting_Get(void);

/**
 * Return value from RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
bool App_CanRx_Debug_EnableDebugMode_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
bool App_CanRx_PDM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
bool App_CanRx_PDM_Fault_DummyFault_Get(void);

