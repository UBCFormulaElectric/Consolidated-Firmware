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
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask1HzWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask100HzWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask1kHzWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTaskCanRxWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTaskCanTxWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_WatchdogTimeoutWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_WatchdogTimeoutWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_TxOverflowWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_TxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_RxOverflowWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_RxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_LeftWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_RightWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_SteeringAngleOCSCWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_SteeringAngleOCSCWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_SteeringAngleOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_FrontBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_FlowRateOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_FlowRateOutOfRangeWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_BrakeAppsDisagreementWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_BrakeAppsDisagreementWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_AppsDisagreementWarning in msg FSM_Warnings.
 */
void App_CanTx_FSM_AppsDisagreementWarning_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_MissingHeartbeatFault in msg FSM_Faults.
 */
void App_CanTx_FSM_MissingHeartbeatFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StateMachineFault in msg FSM_Faults.
 */
void App_CanTx_FSM_StateMachineFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_PappsOCSCFault in msg FSM_Faults.
 */
void App_CanTx_FSM_PappsOCSCFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_SappsOCSCFault in msg FSM_Faults.
 */
void App_CanTx_FSM_SappsOCSCFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_FlowMeterUnderflowFault in msg FSM_Faults.
 */
void App_CanTx_FSM_FlowMeterUnderflowFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_TorquePlausabilityFailedFault in msg FSM_Faults.
 */
void App_CanTx_FSM_TorquePlausabilityFailedFault_Set(bool value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask1HzWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask100HzWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTask1kHzWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTaskCanRxWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_StackWaterMarkHighTaskCanTxWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_WatchdogTimeoutWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_WatchdogTimeoutWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_TxOverflowWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_TxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_RxOverflowWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_RxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_LeftWheelSpeedOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_RightWheelSpeedOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_SteeringAngleOCSCWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_SteeringAngleOCSCWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_SteeringAngleOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_FrontBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_RearBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_FlowRateOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_BrakeAppsDisagreementWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_AppsDisagreementWarningCount in msg FSM_WarningsCounts.
 */
void App_CanTx_FSM_AppsDisagreementWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_MissingHeartbeatFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_MissingHeartbeatFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_StateMachineFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_StateMachineFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_PappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_PappsOCSCFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_SappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_SappsOCSCFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_FlowMeterUnderflowFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_FlowMeterUnderflowFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal FSM_TorquePlausabilityFailedFaultCount in msg FSM_FaultsCounts.
 */
void App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Set(uint32_t value);

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
 * Return value from TX table of signal FSM_StackWaterMarkHighTask1HzWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTask100HzWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTask1kHzWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTaskCanRxWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTaskCanTxWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from TX table of signal FSM_WatchdogTimeoutWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from TX table of signal FSM_TxOverflowWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_TxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal FSM_RxOverflowWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_RxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal FSM_LeftWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_RightWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_SteeringAngleOCSCWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_SteeringAngleOCSCWarning_Get(void);

/**
 * Return value from TX table of signal FSM_SteeringAngleOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_FrontBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_FlowRateOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_FlowRateOutOfRangeWarning_Get(void);

/**
 * Return value from TX table of signal FSM_BrakeAppsDisagreementWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_BrakeAppsDisagreementWarning_Get(void);

/**
 * Return value from TX table of signal FSM_AppsDisagreementWarning in msg FSM_Warnings.
 */
bool App_CanTx_FSM_AppsDisagreementWarning_Get(void);

/**
 * Return value from TX table of signal FSM_MissingHeartbeatFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_MissingHeartbeatFault_Get(void);

/**
 * Return value from TX table of signal FSM_StateMachineFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_StateMachineFault_Get(void);

/**
 * Return value from TX table of signal FSM_PappsOCSCFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_PappsOCSCFault_Get(void);

/**
 * Return value from TX table of signal FSM_SappsOCSCFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_SappsOCSCFault_Get(void);

/**
 * Return value from TX table of signal FSM_FlowMeterUnderflowFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_FlowMeterUnderflowFault_Get(void);

/**
 * Return value from TX table of signal FSM_TorquePlausabilityFailedFault in msg FSM_Faults.
 */
bool App_CanTx_FSM_TorquePlausabilityFailedFault_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTask1HzWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTask100HzWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTask1kHzWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTaskCanRxWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_StackWaterMarkHighTaskCanTxWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_WatchdogTimeoutWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_WatchdogTimeoutWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_TxOverflowWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_TxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_RxOverflowWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_RxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_LeftWheelSpeedOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_RightWheelSpeedOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_SteeringAngleOCSCWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_SteeringAngleOCSCWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_SteeringAngleOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_FrontBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_RearBrakePressureOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_FlowRateOutOfRangeWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_BrakeAppsDisagreementWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_AppsDisagreementWarningCount in msg FSM_WarningsCounts.
 */
uint32_t App_CanTx_FSM_AppsDisagreementWarningCount_Get(void);

/**
 * Return value from TX table of signal FSM_MissingHeartbeatFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_MissingHeartbeatFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_StateMachineFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_StateMachineFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_PappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_PappsOCSCFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_SappsOCSCFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_SappsOCSCFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_FlowMeterUnderflowFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_FlowMeterUnderflowFaultCount_Get(void);

/**
 * Return value from TX table of signal FSM_TorquePlausabilityFailedFaultCount in msg FSM_FaultsCounts.
 */
uint32_t App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Get(void);

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

