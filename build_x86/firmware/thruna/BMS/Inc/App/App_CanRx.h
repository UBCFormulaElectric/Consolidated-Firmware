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
 * Update value stored in RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
void App_CanRx_Debug_EnableDebugMode_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_StartCharging in msg Debug_Charging.
 */
void App_CanRx_Debug_StartCharging_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingRequest in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingRequest_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingOverrideTarget in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingOverrideTarget_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingOverrideTargetValue in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingOverrideTargetValue_Update(float value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingOverridePWM in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingOverridePWM_Update(bool value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingOverridePWMFrequency in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingOverridePWMFrequency_Update(float value);

/**
 * Update value stored in RX table of signal Debug_CellBalancingOverridePWMDuty in msg Debug_CellBalancing.
 */
void App_CanRx_Debug_CellBalancingOverridePWMDuty_Update(uint32_t value);

/**
 * Update value stored in RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
void App_CanRx_DCM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_State in msg DCM_Vitals.
 */
void App_CanRx_DCM_State_Update(DcmState value);

/**
 * Update value stored in RX table of signal DCM_StackWaterMarkHighTask1HzWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_StackWaterMarkHighTask100HzWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_StackWaterMarkHighTask1kHzWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_StackWaterMarkHighTaskCanRxWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_StackWaterMarkHighTaskCanTxWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_WatchdogTimeoutWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_WatchdogTimeoutWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_TxOverflowWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_TxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_RxOverflowWarning in msg DCM_Warnings.
 */
void App_CanRx_DCM_RxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_MissingHeartbeatFault in msg DCM_Faults.
 */
void App_CanRx_DCM_MissingHeartbeatFault_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_LeftInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_LeftInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_RightInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_RightInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
void App_CanRx_FSM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_State in msg FSM_Vitals.
 */
void App_CanRx_FSM_State_Update(State_Machine value);

/**
 * Update value stored in RX table of signal FSM_StackWaterMarkHighTask1HzWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_StackWaterMarkHighTask100HzWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_StackWaterMarkHighTask1kHzWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_StackWaterMarkHighTaskCanRxWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_StackWaterMarkHighTaskCanTxWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_WatchdogTimeoutWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_WatchdogTimeoutWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_TxOverflowWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_TxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_RxOverflowWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_RxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_LeftWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_RightWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_SteeringAngleOCSCWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_SteeringAngleOCSCWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_SteeringAngleOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FrontBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FlowRateOutOfRangeWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_FlowRateOutOfRangeWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_BrakeAppsDisagreementWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_BrakeAppsDisagreementWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_AppsDisagreementWarning in msg FSM_Warnings.
 */
void App_CanRx_FSM_AppsDisagreementWarning_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_MissingHeartbeatFault in msg FSM_Faults.
 */
void App_CanRx_FSM_MissingHeartbeatFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_StateMachineFault in msg FSM_Faults.
 */
void App_CanRx_FSM_StateMachineFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_PappsOCSCFault in msg FSM_Faults.
 */
void App_CanRx_FSM_PappsOCSCFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_SappsOCSCFault in msg FSM_Faults.
 */
void App_CanRx_FSM_SappsOCSCFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FlowMeterUnderflowFault in msg FSM_Faults.
 */
void App_CanRx_FSM_FlowMeterUnderflowFault_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_TorquePlausabilityFailedFault in msg FSM_Faults.
 */
void App_CanRx_FSM_TorquePlausabilityFailedFault_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
void App_CanRx_PDM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_State in msg PDM_Vitals.
 */
void App_CanRx_PDM_State_Update(PdmState value);

/**
 * Update value stored in RX table of signal PDM_StackWaterMarkHighTask1HzWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_StackWaterMarkHighTask100HzWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_StackWaterMarkHighTask1kHzWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_StackWaterMarkHighTaskCanRxWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_StackWaterMarkHighTaskCanTxWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_WatchdogTimeoutWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_WatchdogTimeoutWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_TxOverflowWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_TxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_RxOverflowWarning in msg PDM_Warnings.
 */
void App_CanRx_PDM_RxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_DummyFault in msg PDM_Faults.
 */
void App_CanRx_PDM_DummyFault_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
void App_CanRx_DIM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_State in msg DIM_Vitals.
 */
void App_CanRx_DIM_State_Update(DimState value);

/**
 * Update value stored in RX table of signal DIM_StackWaterMarkHighTask1HzWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_StackWaterMarkHighTask100HzWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_StackWaterMarkHighTask1kHzWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_StackWaterMarkHighTaskCanRxWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_StackWaterMarkHighTaskCanTxWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_WatchdogTimeoutWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_WatchdogTimeoutWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_TxOverflowWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_TxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_RxOverflowWarning in msg DIM_Warnings.
 */
void App_CanRx_DIM_RxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_MissingHeartbeatFault in msg DIM_Faults.
 */
void App_CanRx_DIM_MissingHeartbeatFault_Update(bool value);

/**
 * Return value from RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
bool App_CanRx_Debug_EnableDebugMode_Get(void);

/**
 * Return value from RX table of signal Debug_StartCharging in msg Debug_Charging.
 */
bool App_CanRx_Debug_StartCharging_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingRequest in msg Debug_CellBalancing.
 */
bool App_CanRx_Debug_CellBalancingRequest_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingOverrideTarget in msg Debug_CellBalancing.
 */
bool App_CanRx_Debug_CellBalancingOverrideTarget_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingOverrideTargetValue in msg Debug_CellBalancing.
 */
float App_CanRx_Debug_CellBalancingOverrideTargetValue_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingOverridePWM in msg Debug_CellBalancing.
 */
bool App_CanRx_Debug_CellBalancingOverridePWM_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingOverridePWMFrequency in msg Debug_CellBalancing.
 */
float App_CanRx_Debug_CellBalancingOverridePWMFrequency_Get(void);

/**
 * Return value from RX table of signal Debug_CellBalancingOverridePWMDuty in msg Debug_CellBalancing.
 */
uint32_t App_CanRx_Debug_CellBalancingOverridePWMDuty_Get(void);

/**
 * Return value from RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
bool App_CanRx_DCM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DCM_State in msg DCM_Vitals.
 */
DcmState App_CanRx_DCM_State_Get(void);

/**
 * Return value from RX table of signal DCM_StackWaterMarkHighTask1HzWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from RX table of signal DCM_StackWaterMarkHighTask100HzWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from RX table of signal DCM_StackWaterMarkHighTask1kHzWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from RX table of signal DCM_StackWaterMarkHighTaskCanRxWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from RX table of signal DCM_StackWaterMarkHighTaskCanTxWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from RX table of signal DCM_WatchdogTimeoutWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from RX table of signal DCM_TxOverflowWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_TxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal DCM_RxOverflowWarning in msg DCM_Warnings.
 */
bool App_CanRx_DCM_RxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal DCM_MissingHeartbeatFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_MissingHeartbeatFault_Get(void);

/**
 * Return value from RX table of signal DCM_LeftInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_LeftInverterFault_Get(void);

/**
 * Return value from RX table of signal DCM_RightInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_RightInverterFault_Get(void);

/**
 * Return value from RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
bool App_CanRx_FSM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal FSM_State in msg FSM_Vitals.
 */
State_Machine App_CanRx_FSM_State_Get(void);

/**
 * Return value from RX table of signal FSM_StackWaterMarkHighTask1HzWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from RX table of signal FSM_StackWaterMarkHighTask100HzWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from RX table of signal FSM_StackWaterMarkHighTask1kHzWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from RX table of signal FSM_StackWaterMarkHighTaskCanRxWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from RX table of signal FSM_StackWaterMarkHighTaskCanTxWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from RX table of signal FSM_WatchdogTimeoutWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from RX table of signal FSM_TxOverflowWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_TxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal FSM_RxOverflowWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_RxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal FSM_LeftWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_LeftWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_RightWheelSpeedOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_RightWheelSpeedOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_SteeringAngleOCSCWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_SteeringAngleOCSCWarning_Get(void);

/**
 * Return value from RX table of signal FSM_SteeringAngleOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_SteeringAngleOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_FrontBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_FrontBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_RearBrakePressureOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_RearBrakePressureOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_FlowRateOutOfRangeWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_FlowRateOutOfRangeWarning_Get(void);

/**
 * Return value from RX table of signal FSM_BrakeAppsDisagreementWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_BrakeAppsDisagreementWarning_Get(void);

/**
 * Return value from RX table of signal FSM_AppsDisagreementWarning in msg FSM_Warnings.
 */
bool App_CanRx_FSM_AppsDisagreementWarning_Get(void);

/**
 * Return value from RX table of signal FSM_MissingHeartbeatFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_MissingHeartbeatFault_Get(void);

/**
 * Return value from RX table of signal FSM_StateMachineFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_StateMachineFault_Get(void);

/**
 * Return value from RX table of signal FSM_PappsOCSCFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_PappsOCSCFault_Get(void);

/**
 * Return value from RX table of signal FSM_SappsOCSCFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_SappsOCSCFault_Get(void);

/**
 * Return value from RX table of signal FSM_FlowMeterUnderflowFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_FlowMeterUnderflowFault_Get(void);

/**
 * Return value from RX table of signal FSM_TorquePlausabilityFailedFault in msg FSM_Faults.
 */
bool App_CanRx_FSM_TorquePlausabilityFailedFault_Get(void);

/**
 * Return value from RX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
bool App_CanRx_PDM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal PDM_State in msg PDM_Vitals.
 */
PdmState App_CanRx_PDM_State_Get(void);

/**
 * Return value from RX table of signal PDM_StackWaterMarkHighTask1HzWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from RX table of signal PDM_StackWaterMarkHighTask100HzWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from RX table of signal PDM_StackWaterMarkHighTask1kHzWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from RX table of signal PDM_StackWaterMarkHighTaskCanRxWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from RX table of signal PDM_StackWaterMarkHighTaskCanTxWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from RX table of signal PDM_WatchdogTimeoutWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from RX table of signal PDM_TxOverflowWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_TxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal PDM_RxOverflowWarning in msg PDM_Warnings.
 */
bool App_CanRx_PDM_RxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal PDM_DummyFault in msg PDM_Faults.
 */
bool App_CanRx_PDM_DummyFault_Get(void);

/**
 * Return value from RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
bool App_CanRx_DIM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DIM_State in msg DIM_Vitals.
 */
DimState App_CanRx_DIM_State_Get(void);

/**
 * Return value from RX table of signal DIM_StackWaterMarkHighTask1HzWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from RX table of signal DIM_StackWaterMarkHighTask100HzWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from RX table of signal DIM_StackWaterMarkHighTask1kHzWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from RX table of signal DIM_StackWaterMarkHighTaskCanRxWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from RX table of signal DIM_StackWaterMarkHighTaskCanTxWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from RX table of signal DIM_WatchdogTimeoutWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from RX table of signal DIM_TxOverflowWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_TxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal DIM_RxOverflowWarning in msg DIM_Warnings.
 */
bool App_CanRx_DIM_RxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal DIM_MissingHeartbeatFault in msg DIM_Faults.
 */
bool App_CanRx_DIM_MissingHeartbeatFault_Get(void);

