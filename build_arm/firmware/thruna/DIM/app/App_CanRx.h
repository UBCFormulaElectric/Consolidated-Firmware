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
 * Update value stored in RX table of signal INVR_ModuleATemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_ModuleATemperature_Update(float value);

/**
 * Update value stored in RX table of signal INVR_ModuleBTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_ModuleBTemperature_Update(float value);

/**
 * Update value stored in RX table of signal INVR_ModuleCTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_ModuleCTemperature_Update(float value);

/**
 * Update value stored in RX table of signal INVR_GateDriverBoardTemperature in msg INVR_Temperatures1.
 */
void App_CanRx_INVR_GateDriverBoardTemperature_Update(float value);

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
 * Update value stored in RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void App_CanRx_BMS_Heartbeat_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_State in msg BMS_Vitals.
 */
void App_CanRx_BMS_State_Update(BmsState value);

/**
 * Update value stored in RX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_BmsOk_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_ImdOk_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_BspdOk_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_BmsLatchedFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_ImdLatchedFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
void App_CanRx_BMS_BspdLatchedFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
void App_CanRx_BMS_AirPositive_Update(ContactorState value);

/**
 * Update value stored in RX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
void App_CanRx_BMS_AirNegative_Update(ContactorState value);

/**
 * Update value stored in RX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
void App_CanRx_BMS_PrechargeRelay_Update(ContactorState value);

/**
 * Update value stored in RX table of signal BMS_MinCellVoltage in msg BMS_CellVoltages.
 */
void App_CanRx_BMS_MinCellVoltage_Update(float value);

/**
 * Update value stored in RX table of signal BMS_MaxCellVoltage in msg BMS_CellVoltages.
 */
void App_CanRx_BMS_MaxCellVoltage_Update(float value);

/**
 * Update value stored in RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
void App_CanRx_BMS_TractiveSystemVoltage_Update(float value);

/**
 * Update value stored in RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
void App_CanRx_BMS_TractiveSystemCurrent_Update(float value);

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
 * Return value from RX table of signal INVR_ModuleATemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_ModuleATemperature_Get(void);

/**
 * Return value from RX table of signal INVR_ModuleBTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_ModuleBTemperature_Get(void);

/**
 * Return value from RX table of signal INVR_ModuleCTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_ModuleCTemperature_Get(void);

/**
 * Return value from RX table of signal INVR_GateDriverBoardTemperature in msg INVR_Temperatures1.
 */
float App_CanRx_INVR_GateDriverBoardTemperature_Get(void);

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
 * Return value from RX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool App_CanRx_BMS_Heartbeat_Get(void);

/**
 * Return value from RX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState App_CanRx_BMS_State_Get(void);

/**
 * Return value from RX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_BmsOk_Get(void);

/**
 * Return value from RX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_ImdOk_Get(void);

/**
 * Return value from RX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_BspdOk_Get(void);

/**
 * Return value from RX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_BmsLatchedFault_Get(void);

/**
 * Return value from RX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_ImdLatchedFault_Get(void);

/**
 * Return value from RX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanRx_BMS_BspdLatchedFault_Get(void);

/**
 * Return value from RX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_AirPositive_Get(void);

/**
 * Return value from RX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_AirNegative_Get(void);

/**
 * Return value from RX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
ContactorState App_CanRx_BMS_PrechargeRelay_Get(void);

/**
 * Return value from RX table of signal BMS_MinCellVoltage in msg BMS_CellVoltages.
 */
float App_CanRx_BMS_MinCellVoltage_Get(void);

/**
 * Return value from RX table of signal BMS_MaxCellVoltage in msg BMS_CellVoltages.
 */
float App_CanRx_BMS_MaxCellVoltage_Get(void);

/**
 * Return value from RX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
float App_CanRx_BMS_TractiveSystemVoltage_Get(void);

/**
 * Return value from RX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
float App_CanRx_BMS_TractiveSystemCurrent_Get(void);

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

