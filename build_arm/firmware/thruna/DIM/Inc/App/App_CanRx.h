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
 * Update value stored in RX table of signal BMS_StackWaterMarkHighTask1HzWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_StackWaterMarkHighTask100HzWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_StackWaterMarkHighTask1kHzWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_StackWaterMarkHighTaskCanRxWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_StackWaterMarkHighTaskCanTxWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_WatchdogTimeoutWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_WatchdogTimeoutWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_TxOverflowWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_TxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_RxOverflowWarning in msg BMS_Warnings.
 */
void App_CanRx_BMS_RxOverflowWarning_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_MissingHeartbeatFault in msg BMS_Faults.
 */
void App_CanRx_BMS_MissingHeartbeatFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_StateMachineFault in msg BMS_Faults.
 */
void App_CanRx_BMS_StateMachineFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_CellUndervoltageFault in msg BMS_Faults.
 */
void App_CanRx_BMS_CellUndervoltageFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_CellOvervoltageFault in msg BMS_Faults.
 */
void App_CanRx_BMS_CellOvervoltageFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ModuleCommunicationFault in msg BMS_Faults.
 */
void App_CanRx_BMS_ModuleCommunicationFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_CellUndertempFault in msg BMS_Faults.
 */
void App_CanRx_BMS_CellUndertempFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_CellOvertempFault in msg BMS_Faults.
 */
void App_CanRx_BMS_CellOvertempFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ChargerFault in msg BMS_Faults.
 */
void App_CanRx_BMS_ChargerFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ChargerDisconnectedDuringChargeFault in msg BMS_Faults.
 */
void App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_ChargerExternalShutdownFault in msg BMS_Faults.
 */
void App_CanRx_BMS_ChargerExternalShutdownFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_TractiveSystemOvercurrentFault in msg BMS_Faults.
 */
void App_CanRx_BMS_TractiveSystemOvercurrentFault_Update(bool value);

/**
 * Update value stored in RX table of signal BMS_PrechargeFailureFault in msg BMS_Faults.
 */
void App_CanRx_BMS_PrechargeFailureFault_Update(bool value);

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
 * Return value from RX table of signal Debug_EnableDebugMode in msg Debug_CanMode.
 */
bool App_CanRx_Debug_EnableDebugMode_Get(void);

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
 * Return value from RX table of signal BMS_StackWaterMarkHighTask1HzWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from RX table of signal BMS_StackWaterMarkHighTask100HzWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from RX table of signal BMS_StackWaterMarkHighTask1kHzWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from RX table of signal BMS_StackWaterMarkHighTaskCanRxWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from RX table of signal BMS_StackWaterMarkHighTaskCanTxWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from RX table of signal BMS_WatchdogTimeoutWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from RX table of signal BMS_TxOverflowWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_TxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal BMS_RxOverflowWarning in msg BMS_Warnings.
 */
bool App_CanRx_BMS_RxOverflowWarning_Get(void);

/**
 * Return value from RX table of signal BMS_MissingHeartbeatFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_MissingHeartbeatFault_Get(void);

/**
 * Return value from RX table of signal BMS_StateMachineFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_StateMachineFault_Get(void);

/**
 * Return value from RX table of signal BMS_CellUndervoltageFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_CellUndervoltageFault_Get(void);

/**
 * Return value from RX table of signal BMS_CellOvervoltageFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_CellOvervoltageFault_Get(void);

/**
 * Return value from RX table of signal BMS_ModuleCommunicationFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_ModuleCommunicationFault_Get(void);

/**
 * Return value from RX table of signal BMS_CellUndertempFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_CellUndertempFault_Get(void);

/**
 * Return value from RX table of signal BMS_CellOvertempFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_CellOvertempFault_Get(void);

/**
 * Return value from RX table of signal BMS_ChargerFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_ChargerFault_Get(void);

/**
 * Return value from RX table of signal BMS_ChargerDisconnectedDuringChargeFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_ChargerDisconnectedDuringChargeFault_Get(void);

/**
 * Return value from RX table of signal BMS_ChargerExternalShutdownFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_ChargerExternalShutdownFault_Get(void);

/**
 * Return value from RX table of signal BMS_TractiveSystemOvercurrentFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_TractiveSystemOvercurrentFault_Get(void);

/**
 * Return value from RX table of signal BMS_PrechargeFailureFault in msg BMS_Faults.
 */
bool App_CanRx_BMS_PrechargeFailureFault_Get(void);

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
