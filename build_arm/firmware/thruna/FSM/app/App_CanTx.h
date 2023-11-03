/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize TX signals to their starting values.
 */
void App_CanTx_Init(void);

/**
 * Update value stored in TX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
void App_CanTx_FSM_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_State in msg FSM_Vitals.
 */
void App_CanTx_FSM_State_Set(State_Machine value);

/**
 * Update value stored in TX table of signal FSM_WatchdogTimeoutTaskName in msg FSM_AlertsContext.
 */
void App_CanTx_FSM_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal FSM_TxOverflowCount in msg FSM_AlertsContext.
 */
void App_CanTx_FSM_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_RxOverflowCount in msg FSM_AlertsContext.
 */
void App_CanTx_FSM_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanTx_FSM_PappsMappedPedalPercentage_Set(float value);

/**
 * Update value stored in TX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanTx_FSM_SappsMappedPedalPercentage_Set(float value);

/**
 * Update value stored in TX table of signal FSM_PappsRawPedalPercentage in msg FSM_Apps.
 */
void App_CanTx_FSM_PappsRawPedalPercentage_Set(float value);

/**
 * Update value stored in TX table of signal FSM_SappsRawPedalPercentage in msg FSM_Apps.
 */
void App_CanTx_FSM_SappsRawPedalPercentage_Set(float value);

/**
 * Update value stored in TX table of signal FSM_TorqueLimit in msg FSM_Apps.
 */
void App_CanTx_FSM_TorqueLimit_Set(float value);

/**
 * Update value stored in TX table of signal FSM_TorquePlausabilityFailed in msg FSM_Apps.
 */
void App_CanTx_FSM_TorquePlausabilityFailed_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_FrontBrakePressure in msg FSM_Brake.
 */
void App_CanTx_FSM_FrontBrakePressure_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_RearBrakePressure in msg FSM_Brake.
 */
void App_CanTx_FSM_RearBrakePressure_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_BrakePedalPercentage in msg FSM_Brake.
 */
void App_CanTx_FSM_BrakePedalPercentage_Set(float value);

/**
 * Update value stored in TX table of signal FSM_BrakeActuated in msg FSM_Brake.
 */
void App_CanTx_FSM_BrakeActuated_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_BrakePressureSensorOCSC in msg FSM_Brake.
 */
void App_CanTx_FSM_BrakePressureSensorOCSC_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_BrakePedalSensorOCSC in msg FSM_Brake.
 */
void App_CanTx_FSM_BrakePedalSensorOCSC_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_CoolantFlowRate in msg FSM_Coolant.
 */
void App_CanTx_FSM_CoolantFlowRate_Set(float value);

/**
 * Update value stored in TX table of signal FSM_CoolantTemperatureA in msg FSM_Coolant.
 */
void App_CanTx_FSM_CoolantTemperatureA_Set(float value);

/**
 * Update value stored in TX table of signal FSM_CoolantTemperatureB in msg FSM_Coolant.
 */
void App_CanTx_FSM_CoolantTemperatureB_Set(float value);

/**
 * Update value stored in TX table of signal FSM_CoolantPressureA in msg FSM_Coolant.
 */
void App_CanTx_FSM_CoolantPressureA_Set(float value);

/**
 * Update value stored in TX table of signal FSM_CoolantPressureB in msg FSM_Coolant.
 */
void App_CanTx_FSM_CoolantPressureB_Set(float value);

/**
 * Update value stored in TX table of signal FSM_SteeringAngle in msg FSM_Steering.
 */
void App_CanTx_FSM_SteeringAngle_Set(float value);

/**
 * Update value stored in TX table of signal FSM_LeftWheelSpeed in msg FSM_Wheels.
 */
void App_CanTx_FSM_LeftWheelSpeed_Set(float value);

/**
 * Update value stored in TX table of signal FSM_RightWheelSpeed in msg FSM_Wheels.
 */
void App_CanTx_FSM_RightWheelSpeed_Set(float value);

/**
 * Update value stored in TX table of signal FSM_Hash in msg FSM_CommitInfo.
 */
void App_CanTx_FSM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Clean in msg FSM_CommitInfo.
 */
void App_CanTx_FSM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_TxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_RxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_SteeringAngleOCSC_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_FlowRateOutOfRange in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_FlowRateOutOfRange_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_BrakeAppsDisagreement in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_BrakeAppsDisagreement_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
void App_CanTx_FSM_Warning_AppsDisagreement_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_MissingHeartbeat in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_MissingHeartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_StateMachine in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_StateMachine_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_PappsOCSC_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_SappsOCSCFault in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_SappsOCSCFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_FlowMeterUnderflow in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_FlowMeterUnderflow_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Fault_TorquePlausabilityFailed in msg FSM_Faults.
 */
void App_CanTx_FSM_Fault_TorquePlausabilityFailed_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask1HzCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask100HzCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTask1kHzCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTaskCanRxCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_StackWaterMarkHighTaskCanTxCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_WatchdogTimeoutCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_WatchdogTimeoutCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_TxOverflowCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_RxOverflowCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_LeftWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_RightWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_RightWheelSpeedOutOfRangeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_SteeringAngleOCSCCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_SteeringAngleOCSCCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_SteeringAngleOutOfRangeCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_SteeringAngleOutOfRangeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_FrontBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_FrontBrakePressureOutOfRangeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_FlowRateOutOfRangeCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_FlowRateOutOfRangeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_BrakeAppsDisagreementCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_BrakeAppsDisagreementCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Warning_AppsDisagreementCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_Warning_AppsDisagreementCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_MissingHeartbeatCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_MissingHeartbeatCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_StateMachineCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_StateMachineCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_PappsOCSCCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_PappsOCSCCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_SappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_SappsOCSCFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_FlowMeterUnderflowCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_FlowMeterUnderflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_Fault_TorquePlausabilityFailedCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_Fault_TorquePlausabilityFailedCount_Set(uint32_t value);

/**
 * Return value from TX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
bool App_CanTx_FSM_Heartbeat_Get(void);

/**
 * Return value from TX table of signal FSM_State in msg FSM_Vitals.
 */
State_Machine App_CanTx_FSM_State_Get(void);

/**
 * Return value from TX table of signal FSM_WatchdogTimeoutTaskName in msg FSM_AlertsContext.
 */
RtosTaskName App_CanTx_FSM_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal FSM_TxOverflowCount in msg FSM_AlertsContext.
 */
uint32_t App_CanTx_FSM_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal FSM_RxOverflowCount in msg FSM_AlertsContext.
 */
uint32_t App_CanTx_FSM_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanTx_FSM_PappsMappedPedalPercentage_Get(void);

/**
 * Return value from TX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanTx_FSM_SappsMappedPedalPercentage_Get(void);

/**
 * Return value from TX table of signal FSM_PappsRawPedalPercentage in msg FSM_Apps.
 */
float App_CanTx_FSM_PappsRawPedalPercentage_Get(void);

/**
 * Return value from TX table of signal FSM_SappsRawPedalPercentage in msg FSM_Apps.
 */
float App_CanTx_FSM_SappsRawPedalPercentage_Get(void);

/**
 * Return value from TX table of signal FSM_TorqueLimit in msg FSM_Apps.
 */
float App_CanTx_FSM_TorqueLimit_Get(void);

/**
 * Return value from TX table of signal FSM_TorquePlausabilityFailed in msg FSM_Apps.
 */
bool App_CanTx_FSM_TorquePlausabilityFailed_Get(void);

/**
 * Return value from TX table of signal FSM_FrontBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanTx_FSM_FrontBrakePressure_Get(void);

/**
 * Return value from TX table of signal FSM_RearBrakePressure in msg FSM_Brake.
 */
uint32_t App_CanTx_FSM_RearBrakePressure_Get(void);

/**
 * Return value from TX table of signal FSM_BrakePedalPercentage in msg FSM_Brake.
 */
float App_CanTx_FSM_BrakePedalPercentage_Get(void);

/**
 * Return value from TX table of signal FSM_BrakeActuated in msg FSM_Brake.
 */
bool App_CanTx_FSM_BrakeActuated_Get(void);

/**
 * Return value from TX table of signal FSM_BrakePressureSensorOCSC in msg FSM_Brake.
 */
bool App_CanTx_FSM_BrakePressureSensorOCSC_Get(void);

/**
 * Return value from TX table of signal FSM_BrakePedalSensorOCSC in msg FSM_Brake.
 */
bool App_CanTx_FSM_BrakePedalSensorOCSC_Get(void);

/**
 * Return value from TX table of signal FSM_CoolantFlowRate in msg FSM_Coolant.
 */
float App_CanTx_FSM_CoolantFlowRate_Get(void);

/**
 * Return value from TX table of signal FSM_CoolantTemperatureA in msg FSM_Coolant.
 */
float App_CanTx_FSM_CoolantTemperatureA_Get(void);

/**
 * Return value from TX table of signal FSM_CoolantTemperatureB in msg FSM_Coolant.
 */
float App_CanTx_FSM_CoolantTemperatureB_Get(void);

/**
 * Return value from TX table of signal FSM_CoolantPressureA in msg FSM_Coolant.
 */
float App_CanTx_FSM_CoolantPressureA_Get(void);

/**
 * Return value from TX table of signal FSM_CoolantPressureB in msg FSM_Coolant.
 */
float App_CanTx_FSM_CoolantPressureB_Get(void);

/**
 * Return value from TX table of signal FSM_SteeringAngle in msg FSM_Steering.
 */
float App_CanTx_FSM_SteeringAngle_Get(void);

/**
 * Return value from TX table of signal FSM_LeftWheelSpeed in msg FSM_Wheels.
 */
float App_CanTx_FSM_LeftWheelSpeed_Get(void);

/**
 * Return value from TX table of signal FSM_RightWheelSpeed in msg FSM_Wheels.
 */
float App_CanTx_FSM_RightWheelSpeed_Get(void);

/**
 * Return value from TX table of signal FSM_Hash in msg FSM_CommitInfo.
 */
uint32_t App_CanTx_FSM_Hash_Get(void);

/**
 * Return value from TX table of signal FSM_Clean in msg FSM_CommitInfo.
 */
bool App_CanTx_FSM_Clean_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask1Hz in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask100Hz in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask1kHz in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTaskCanRx in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTaskCanTx in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_WatchdogTimeout in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_TxOverflow in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_TxOverflow_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RxOverflow in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_RxOverflow_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_LeftWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RightWheelSpeedOutOfRange in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_SteeringAngleOCSC in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_SteeringAngleOCSC_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_SteeringAngleOutOfRange in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_FrontBrakePressureOutOfRange in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_FlowRateOutOfRange in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_FlowRateOutOfRange_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_BrakeAppsDisagreement in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_BrakeAppsDisagreement_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_AppsDisagreement in msg FSM_Warnings.
 */
bool App_CanTx_FSM_Warning_AppsDisagreement_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_MissingHeartbeat in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_StateMachine in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_StateMachine_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_PappsOCSC in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_PappsOCSC_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_SappsOCSCFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_SappsOCSCFault_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_FlowMeterUnderflow in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_FlowMeterUnderflow_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_TorquePlausabilityFailed in msg FSM_Faults.
 */
bool App_CanTx_FSM_Fault_TorquePlausabilityFailed_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask1HzCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask1HzCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask100HzCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask100HzCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTask1kHzCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHzCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTaskCanRxCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_StackWaterMarkHighTaskCanTxCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_WatchdogTimeoutCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_WatchdogTimeoutCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_TxOverflowCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RxOverflowCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_LeftWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RightWheelSpeedOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_RightWheelSpeedOutOfRangeCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_SteeringAngleOCSCCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_SteeringAngleOCSCCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_SteeringAngleOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_SteeringAngleOutOfRangeCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_FrontBrakePressureOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_FrontBrakePressureOutOfRangeCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_RearBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_FlowRateOutOfRangeCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_FlowRateOutOfRangeCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_BrakeAppsDisagreementCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_BrakeAppsDisagreementCount_Get(void);

/**
 * Return value from TX table of signal FSM_Warning_AppsDisagreementCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_Warning_AppsDisagreementCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_MissingHeartbeatCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_MissingHeartbeatCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_StateMachineCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_StateMachineCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_PappsOCSCCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_PappsOCSCCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_SappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_SappsOCSCFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_FlowMeterUnderflowCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_FlowMeterUnderflowCount_Get(void);

/**
 * Return value from TX table of signal FSM_Fault_TorquePlausabilityFailedCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_Fault_TorquePlausabilityFailedCount_Get(void);

/**
 * Returns pointer to the struct of msg FSM_Vitals signals in the TX table.
 */
const FSM_Vitals_Signals* App_CanTx_FSM_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_AlertsContext signals in the TX table.
 */
const FSM_AlertsContext_Signals* App_CanTx_FSM_AlertsContext_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Apps signals in the TX table.
 */
const FSM_Apps_Signals* App_CanTx_FSM_Apps_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Brake signals in the TX table.
 */
const FSM_Brake_Signals* App_CanTx_FSM_Brake_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Coolant signals in the TX table.
 */
const FSM_Coolant_Signals* App_CanTx_FSM_Coolant_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Steering signals in the TX table.
 */
const FSM_Steering_Signals* App_CanTx_FSM_Steering_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Wheels signals in the TX table.
 */
const FSM_Wheels_Signals* App_CanTx_FSM_Wheels_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_CommitInfo signals in the TX table.
 */
const FSM_CommitInfo_Signals* App_CanTx_FSM_CommitInfo_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Warnings signals in the TX table.
 */
const FSM_Warnings_Signals* App_CanTx_FSM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_Faults signals in the TX table.
 */
const FSM_Faults_Signals* App_CanTx_FSM_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_WarningsCounts signals in the TX table.
 */
const FSM_WarningsCounts_Signals* App_CanTx_FSM_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg FSM_FaultsCounts signals in the TX table.
 */
const FSM_FaultsCounts_Signals* App_CanTx_FSM_FaultsCounts_GetData(void);

