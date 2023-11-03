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
 * Update value stored in RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
void App_CanRx_DCM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_State in msg DCM_Vitals.
 */
void App_CanRx_DCM_State_Update(DcmState value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_WatchdogTimeout_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_TxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
void App_CanRx_DCM_Warning_RxOverflow_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_MissingHeartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_LeftInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
void App_CanRx_DCM_Fault_RightInverterFault_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
void App_CanRx_DIM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal DIM_State in msg DIM_Vitals.
 */
void App_CanRx_DIM_State_Update(DimState value);

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
 * Update value stored in RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
void App_CanRx_FSM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_State in msg FSM_Vitals.
 */
void App_CanRx_FSM_State_Update(State_Machine value);

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
 * Update value stored in RX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
void App_CanRx_PDM_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal PDM_State in msg PDM_Vitals.
 */
void App_CanRx_PDM_State_Update(PdmState value);

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
 * Return value from RX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
bool App_CanRx_DCM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DCM_State in msg DCM_Vitals.
 */
DcmState App_CanRx_DCM_State_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_TxOverflow_Get(void);

/**
 * Return value from RX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
bool App_CanRx_DCM_Warning_RxOverflow_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_LeftInverterFault_Get(void);

/**
 * Return value from RX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
bool App_CanRx_DCM_Fault_RightInverterFault_Get(void);

/**
 * Return value from RX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
bool App_CanRx_DIM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal DIM_State in msg DIM_Vitals.
 */
DimState App_CanRx_DIM_State_Get(void);

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
 * Return value from RX table of signal FSM_Heartbeat in msg FSM_Vitals.
 */
bool App_CanRx_FSM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal FSM_State in msg FSM_Vitals.
 */
State_Machine App_CanRx_FSM_State_Get(void);

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
 * Return value from RX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
bool App_CanRx_PDM_Heartbeat_Get(void);

/**
 * Return value from RX table of signal PDM_State in msg PDM_Vitals.
 */
PdmState App_CanRx_PDM_State_Get(void);

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

