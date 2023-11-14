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
 * Update value stored in TX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
void App_CanTx_BMS_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_State in msg BMS_Vitals.
 */
void App_CanTx_BMS_State_Set(BmsState value);

/**
 * Update value stored in TX table of signal BMS_TxOverflowCount in msg BMS_AlertsContext.
 */
void App_CanTx_BMS_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_RxOverflowCount in msg BMS_AlertsContext.
 */
void App_CanTx_BMS_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_WatchdogTimeoutTaskName in msg BMS_AlertsContext.
 */
void App_CanTx_BMS_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal BMS_ImdOkHs in msg BMS_ImdStatus.
 */
void App_CanTx_BMS_ImdOkHs_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ImdSecondsSincePowerOn in msg BMS_ImdStatus.
 */
void App_CanTx_BMS_ImdSecondsSincePowerOn_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ImdValidDutyCycle in msg BMS_ImdStatus.
 */
void App_CanTx_BMS_ImdValidDutyCycle_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ImdCondition in msg BMS_ImdStatus.
 */
void App_CanTx_BMS_ImdCondition_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ImdActiveFrequency in msg BMS_ImdData.
 */
void App_CanTx_BMS_ImdActiveFrequency_Set(ImdActiveFrequency value);

/**
 * Update value stored in TX table of signal BMS_ImdInsulationMeasurementDcp10Hz in msg BMS_ImdData.
 */
void App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ImdInsulationMeasurementDcp20Hz in msg BMS_ImdData.
 */
void App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ImdSpeedStartStatus30Hz in msg BMS_ImdData.
 */
void App_CanTx_BMS_ImdSpeedStartStatus30Hz_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ImdDutyCycle in msg BMS_ImdPwmOutput.
 */
void App_CanTx_BMS_ImdDutyCycle_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ImdFrequency in msg BMS_ImdPwmOutput.
 */
void App_CanTx_BMS_ImdFrequency_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ChargerConnected in msg BMS_Charger.
 */
void App_CanTx_BMS_ChargerConnected_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ChargingComplete in msg BMS_Charger.
 */
void App_CanTx_BMS_ChargingComplete_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_BmsOk_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_ImdOk_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_BspdOk_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_BmsLatchedFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_ImdLatchedFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
void App_CanTx_BMS_BspdLatchedFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
void App_CanTx_BMS_AirPositive_Set(ContactorState value);

/**
 * Update value stored in TX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
void App_CanTx_BMS_AirNegative_Set(ContactorState value);

/**
 * Update value stored in TX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
void App_CanTx_BMS_PrechargeRelay_Set(ContactorState value);

/**
 * Update value stored in TX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
void App_CanTx_BMS_MinCellTemperature_Set(float value);

/**
 * Update value stored in TX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
void App_CanTx_BMS_MaxCellTemperature_Set(float value);

/**
 * Update value stored in TX table of signal BMS_MinCellVoltage in msg BMS_CellVoltages.
 */
void App_CanTx_BMS_MinCellVoltage_Set(float value);

/**
 * Update value stored in TX table of signal BMS_MaxCellVoltage in msg BMS_CellVoltages.
 */
void App_CanTx_BMS_MaxCellVoltage_Set(float value);

/**
 * Update value stored in TX table of signal BMS_MinCellVoltageSegment in msg BMS_CellStats.
 */
void App_CanTx_BMS_MinCellVoltageSegment_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MinCellVoltageIdx in msg BMS_CellStats.
 */
void App_CanTx_BMS_MinCellVoltageIdx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MaxCellVoltageSegment in msg BMS_CellStats.
 */
void App_CanTx_BMS_MaxCellVoltageSegment_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MaxCellVoltageIdx in msg BMS_CellStats.
 */
void App_CanTx_BMS_MaxCellVoltageIdx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MinTempSegment in msg BMS_CellStats.
 */
void App_CanTx_BMS_MinTempSegment_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MinTempIdx in msg BMS_CellStats.
 */
void App_CanTx_BMS_MinTempIdx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MaxTempSegment in msg BMS_CellStats.
 */
void App_CanTx_BMS_MaxTempSegment_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MaxTempIdx in msg BMS_CellStats.
 */
void App_CanTx_BMS_MaxTempIdx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
void App_CanTx_BMS_TractiveSystemVoltage_Set(float value);

/**
 * Update value stored in TX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
void App_CanTx_BMS_TractiveSystemCurrent_Set(float value);

/**
 * Update value stored in TX table of signal BMS_PackVoltage in msg BMS_PackVoltage.
 */
void App_CanTx_BMS_PackVoltage_Set(float value);

/**
 * Update value stored in TX table of signal BMS_AvailablePower in msg BMS_AvailablePower.
 */
void App_CanTx_BMS_AvailablePower_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp0 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp0_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp1 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp1_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp2 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp2_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp3 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp3_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp4 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp4_Set(float value);

/**
 * Update value stored in TX table of signal BMS_ThermTemp5 in msg BMS_AuxThermistors.
 */
void App_CanTx_BMS_ThermTemp5_Set(float value);

/**
 * Update value stored in TX table of signal BMS_Hash in msg BMS_CommitInfo.
 */
void App_CanTx_BMS_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Clean in msg BMS_CommitInfo.
 */
void App_CanTx_BMS_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackRemainingTask1Hz in msg BMS_StackRemaining.
 */
void App_CanTx_BMS_StackRemainingTask1Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackRemainingTask100Hz in msg BMS_StackRemaining.
 */
void App_CanTx_BMS_StackRemainingTask100Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackRemainingTask1kHz in msg BMS_StackRemaining.
 */
void App_CanTx_BMS_StackRemainingTask1kHz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackRemainingTaskCanRx in msg BMS_StackRemaining.
 */
void App_CanTx_BMS_StackRemainingTaskCanRx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackRemainingTaskCanTx in msg BMS_StackRemaining.
 */
void App_CanTx_BMS_StackRemainingTaskCanTx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask1HzWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_StackWaterMarkHighTask1HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask100HzWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_StackWaterMarkHighTask100HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask1kHzWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_StackWaterMarkHighTask1kHzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTaskCanRxWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTaskCanTxWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_WatchdogTimeoutWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_WatchdogTimeoutWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_TxOverflowWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_TxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_RxOverflowWarning in msg BMS_Warnings.
 */
void App_CanTx_BMS_RxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_MissingHeartbeatFault in msg BMS_Faults.
 */
void App_CanTx_BMS_MissingHeartbeatFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StateMachineFault in msg BMS_Faults.
 */
void App_CanTx_BMS_StateMachineFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_CellUndervoltageFault in msg BMS_Faults.
 */
void App_CanTx_BMS_CellUndervoltageFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_CellOvervoltageFault in msg BMS_Faults.
 */
void App_CanTx_BMS_CellOvervoltageFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ModuleCommunicationFault in msg BMS_Faults.
 */
void App_CanTx_BMS_ModuleCommunicationFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_CellUndertempFault in msg BMS_Faults.
 */
void App_CanTx_BMS_CellUndertempFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_CellOvertempFault in msg BMS_Faults.
 */
void App_CanTx_BMS_CellOvertempFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ChargerFault in msg BMS_Faults.
 */
void App_CanTx_BMS_ChargerFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ChargerDisconnectedDuringChargeFault in msg BMS_Faults.
 */
void App_CanTx_BMS_ChargerDisconnectedDuringChargeFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_ChargerExternalShutdownFault in msg BMS_Faults.
 */
void App_CanTx_BMS_ChargerExternalShutdownFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_TractiveSystemOvercurrentFault in msg BMS_Faults.
 */
void App_CanTx_BMS_TractiveSystemOvercurrentFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_PrechargeFailureFault in msg BMS_Faults.
 */
void App_CanTx_BMS_PrechargeFailureFault_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask1HzWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask100HzWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTask1kHzWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTaskCanRxWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StackWaterMarkHighTaskCanTxWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_WatchdogTimeoutWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_WatchdogTimeoutWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_TxOverflowWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_TxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_RxOverflowWarningCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_RxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_MissingHeartbeatFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_MissingHeartbeatFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_StateMachineFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_StateMachineFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_CellUndervoltageFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_CellUndervoltageFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_CellOvervoltageFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_CellOvervoltageFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ModuleCommunicationFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_ModuleCommunicationFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_CellUndertempFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_CellUndertempFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_CellOvertempFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_CellOvertempFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ChargerFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_ChargerFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ChargerDisconnectedDuringChargeFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_ChargerDisconnectedDuringChargeFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_ChargerExternalShutdownFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_ChargerExternalShutdownFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_TractiveSystemOvercurrentFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_TractiveSystemOvercurrentFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_PrechargeFailureFaultCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_PrechargeFailureFaultCount_Set(uint32_t value);

/**
 * Return value from TX table of signal BMS_Heartbeat in msg BMS_Vitals.
 */
bool App_CanTx_BMS_Heartbeat_Get(void);

/**
 * Return value from TX table of signal BMS_State in msg BMS_Vitals.
 */
BmsState App_CanTx_BMS_State_Get(void);

/**
 * Return value from TX table of signal BMS_TxOverflowCount in msg BMS_AlertsContext.
 */
uint32_t App_CanTx_BMS_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal BMS_RxOverflowCount in msg BMS_AlertsContext.
 */
uint32_t App_CanTx_BMS_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal BMS_WatchdogTimeoutTaskName in msg BMS_AlertsContext.
 */
RtosTaskName App_CanTx_BMS_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal BMS_ImdOkHs in msg BMS_ImdStatus.
 */
bool App_CanTx_BMS_ImdOkHs_Get(void);

/**
 * Return value from TX table of signal BMS_ImdSecondsSincePowerOn in msg BMS_ImdStatus.
 */
uint32_t App_CanTx_BMS_ImdSecondsSincePowerOn_Get(void);

/**
 * Return value from TX table of signal BMS_ImdValidDutyCycle in msg BMS_ImdStatus.
 */
bool App_CanTx_BMS_ImdValidDutyCycle_Get(void);

/**
 * Return value from TX table of signal BMS_ImdCondition in msg BMS_ImdStatus.
 */
uint32_t App_CanTx_BMS_ImdCondition_Get(void);

/**
 * Return value from TX table of signal BMS_ImdActiveFrequency in msg BMS_ImdData.
 */
ImdActiveFrequency App_CanTx_BMS_ImdActiveFrequency_Get(void);

/**
 * Return value from TX table of signal BMS_ImdInsulationMeasurementDcp10Hz in msg BMS_ImdData.
 */
float App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Get(void);

/**
 * Return value from TX table of signal BMS_ImdInsulationMeasurementDcp20Hz in msg BMS_ImdData.
 */
float App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Get(void);

/**
 * Return value from TX table of signal BMS_ImdSpeedStartStatus30Hz in msg BMS_ImdData.
 */
float App_CanTx_BMS_ImdSpeedStartStatus30Hz_Get(void);

/**
 * Return value from TX table of signal BMS_ImdDutyCycle in msg BMS_ImdPwmOutput.
 */
float App_CanTx_BMS_ImdDutyCycle_Get(void);

/**
 * Return value from TX table of signal BMS_ImdFrequency in msg BMS_ImdPwmOutput.
 */
float App_CanTx_BMS_ImdFrequency_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerConnected in msg BMS_Charger.
 */
bool App_CanTx_BMS_ChargerConnected_Get(void);

/**
 * Return value from TX table of signal BMS_ChargingComplete in msg BMS_Charger.
 */
bool App_CanTx_BMS_ChargingComplete_Get(void);

/**
 * Return value from TX table of signal BMS_BmsOk in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_BmsOk_Get(void);

/**
 * Return value from TX table of signal BMS_ImdOk in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_ImdOk_Get(void);

/**
 * Return value from TX table of signal BMS_BspdOk in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_BspdOk_Get(void);

/**
 * Return value from TX table of signal BMS_BmsLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_BmsLatchedFault_Get(void);

/**
 * Return value from TX table of signal BMS_ImdLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_ImdLatchedFault_Get(void);

/**
 * Return value from TX table of signal BMS_BspdLatchedFault in msg BMS_OkStatuses.
 */
bool App_CanTx_BMS_BspdLatchedFault_Get(void);

/**
 * Return value from TX table of signal BMS_AirPositive in msg BMS_Contactors.
 */
ContactorState App_CanTx_BMS_AirPositive_Get(void);

/**
 * Return value from TX table of signal BMS_AirNegative in msg BMS_Contactors.
 */
ContactorState App_CanTx_BMS_AirNegative_Get(void);

/**
 * Return value from TX table of signal BMS_PrechargeRelay in msg BMS_Contactors.
 */
ContactorState App_CanTx_BMS_PrechargeRelay_Get(void);

/**
 * Return value from TX table of signal BMS_MinCellTemperature in msg BMS_CellTemperatures.
 */
float App_CanTx_BMS_MinCellTemperature_Get(void);

/**
 * Return value from TX table of signal BMS_MaxCellTemperature in msg BMS_CellTemperatures.
 */
float App_CanTx_BMS_MaxCellTemperature_Get(void);

/**
 * Return value from TX table of signal BMS_MinCellVoltage in msg BMS_CellVoltages.
 */
float App_CanTx_BMS_MinCellVoltage_Get(void);

/**
 * Return value from TX table of signal BMS_MaxCellVoltage in msg BMS_CellVoltages.
 */
float App_CanTx_BMS_MaxCellVoltage_Get(void);

/**
 * Return value from TX table of signal BMS_MinCellVoltageSegment in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MinCellVoltageSegment_Get(void);

/**
 * Return value from TX table of signal BMS_MinCellVoltageIdx in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MinCellVoltageIdx_Get(void);

/**
 * Return value from TX table of signal BMS_MaxCellVoltageSegment in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MaxCellVoltageSegment_Get(void);

/**
 * Return value from TX table of signal BMS_MaxCellVoltageIdx in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MaxCellVoltageIdx_Get(void);

/**
 * Return value from TX table of signal BMS_MinTempSegment in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MinTempSegment_Get(void);

/**
 * Return value from TX table of signal BMS_MinTempIdx in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MinTempIdx_Get(void);

/**
 * Return value from TX table of signal BMS_MaxTempSegment in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MaxTempSegment_Get(void);

/**
 * Return value from TX table of signal BMS_MaxTempIdx in msg BMS_CellStats.
 */
uint32_t App_CanTx_BMS_MaxTempIdx_Get(void);

/**
 * Return value from TX table of signal BMS_TractiveSystemVoltage in msg BMS_TractiveSystem.
 */
float App_CanTx_BMS_TractiveSystemVoltage_Get(void);

/**
 * Return value from TX table of signal BMS_TractiveSystemCurrent in msg BMS_TractiveSystem.
 */
float App_CanTx_BMS_TractiveSystemCurrent_Get(void);

/**
 * Return value from TX table of signal BMS_PackVoltage in msg BMS_PackVoltage.
 */
float App_CanTx_BMS_PackVoltage_Get(void);

/**
 * Return value from TX table of signal BMS_AvailablePower in msg BMS_AvailablePower.
 */
float App_CanTx_BMS_AvailablePower_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp0 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp0_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp1 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp1_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp2 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp2_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp3 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp3_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp4 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp4_Get(void);

/**
 * Return value from TX table of signal BMS_ThermTemp5 in msg BMS_AuxThermistors.
 */
float App_CanTx_BMS_ThermTemp5_Get(void);

/**
 * Return value from TX table of signal BMS_Hash in msg BMS_CommitInfo.
 */
uint32_t App_CanTx_BMS_Hash_Get(void);

/**
 * Return value from TX table of signal BMS_Clean in msg BMS_CommitInfo.
 */
bool App_CanTx_BMS_Clean_Get(void);

/**
 * Return value from TX table of signal BMS_StackRemainingTask1Hz in msg BMS_StackRemaining.
 */
uint32_t App_CanTx_BMS_StackRemainingTask1Hz_Get(void);

/**
 * Return value from TX table of signal BMS_StackRemainingTask100Hz in msg BMS_StackRemaining.
 */
uint32_t App_CanTx_BMS_StackRemainingTask100Hz_Get(void);

/**
 * Return value from TX table of signal BMS_StackRemainingTask1kHz in msg BMS_StackRemaining.
 */
uint32_t App_CanTx_BMS_StackRemainingTask1kHz_Get(void);

/**
 * Return value from TX table of signal BMS_StackRemainingTaskCanRx in msg BMS_StackRemaining.
 */
uint32_t App_CanTx_BMS_StackRemainingTaskCanRx_Get(void);

/**
 * Return value from TX table of signal BMS_StackRemainingTaskCanTx in msg BMS_StackRemaining.
 */
uint32_t App_CanTx_BMS_StackRemainingTaskCanTx_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask1HzWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask100HzWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask1kHzWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTaskCanRxWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTaskCanTxWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from TX table of signal BMS_WatchdogTimeoutWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from TX table of signal BMS_TxOverflowWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_TxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal BMS_RxOverflowWarning in msg BMS_Warnings.
 */
bool App_CanTx_BMS_RxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal BMS_MissingHeartbeatFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_MissingHeartbeatFault_Get(void);

/**
 * Return value from TX table of signal BMS_StateMachineFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_StateMachineFault_Get(void);

/**
 * Return value from TX table of signal BMS_CellUndervoltageFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_CellUndervoltageFault_Get(void);

/**
 * Return value from TX table of signal BMS_CellOvervoltageFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_CellOvervoltageFault_Get(void);

/**
 * Return value from TX table of signal BMS_ModuleCommunicationFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_ModuleCommunicationFault_Get(void);

/**
 * Return value from TX table of signal BMS_CellUndertempFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_CellUndertempFault_Get(void);

/**
 * Return value from TX table of signal BMS_CellOvertempFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_CellOvertempFault_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_ChargerFault_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerDisconnectedDuringChargeFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_ChargerDisconnectedDuringChargeFault_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerExternalShutdownFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_ChargerExternalShutdownFault_Get(void);

/**
 * Return value from TX table of signal BMS_TractiveSystemOvercurrentFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_TractiveSystemOvercurrentFault_Get(void);

/**
 * Return value from TX table of signal BMS_PrechargeFailureFault in msg BMS_Faults.
 */
bool App_CanTx_BMS_PrechargeFailureFault_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask1HzWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_StackWaterMarkHighTask1HzWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask100HzWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_StackWaterMarkHighTask100HzWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTask1kHzWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_StackWaterMarkHighTask1kHzWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTaskCanRxWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_StackWaterMarkHighTaskCanTxWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_WatchdogTimeoutWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_WatchdogTimeoutWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_TxOverflowWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_TxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_RxOverflowWarningCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_RxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal BMS_MissingHeartbeatFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_MissingHeartbeatFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_StateMachineFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_StateMachineFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_CellUndervoltageFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_CellUndervoltageFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_CellOvervoltageFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_CellOvervoltageFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_ModuleCommunicationFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_ModuleCommunicationFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_CellUndertempFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_CellUndertempFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_CellOvertempFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_CellOvertempFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_ChargerFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerDisconnectedDuringChargeFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_ChargerDisconnectedDuringChargeFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_ChargerExternalShutdownFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_ChargerExternalShutdownFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_TractiveSystemOvercurrentFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_TractiveSystemOvercurrentFaultCount_Get(void);

/**
 * Return value from TX table of signal BMS_PrechargeFailureFaultCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_PrechargeFailureFaultCount_Get(void);

/**
 * Returns pointer to the struct of msg BMS_Vitals signals in the TX table.
 */
const BMS_Vitals_Signals* App_CanTx_BMS_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_AlertsContext signals in the TX table.
 */
const BMS_AlertsContext_Signals* App_CanTx_BMS_AlertsContext_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_ImdStatus signals in the TX table.
 */
const BMS_ImdStatus_Signals* App_CanTx_BMS_ImdStatus_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_ImdData signals in the TX table.
 */
const BMS_ImdData_Signals* App_CanTx_BMS_ImdData_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_ImdPwmOutput signals in the TX table.
 */
const BMS_ImdPwmOutput_Signals* App_CanTx_BMS_ImdPwmOutput_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_Charger signals in the TX table.
 */
const BMS_Charger_Signals* App_CanTx_BMS_Charger_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_OkStatuses signals in the TX table.
 */
const BMS_OkStatuses_Signals* App_CanTx_BMS_OkStatuses_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_Contactors signals in the TX table.
 */
const BMS_Contactors_Signals* App_CanTx_BMS_Contactors_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_CellTemperatures signals in the TX table.
 */
const BMS_CellTemperatures_Signals* App_CanTx_BMS_CellTemperatures_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_CellVoltages signals in the TX table.
 */
const BMS_CellVoltages_Signals* App_CanTx_BMS_CellVoltages_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_CellStats signals in the TX table.
 */
const BMS_CellStats_Signals* App_CanTx_BMS_CellStats_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_TractiveSystem signals in the TX table.
 */
const BMS_TractiveSystem_Signals* App_CanTx_BMS_TractiveSystem_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_PackVoltage signals in the TX table.
 */
const BMS_PackVoltage_Signals* App_CanTx_BMS_PackVoltage_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_AvailablePower signals in the TX table.
 */
const BMS_AvailablePower_Signals* App_CanTx_BMS_AvailablePower_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_AuxThermistors signals in the TX table.
 */
const BMS_AuxThermistors_Signals* App_CanTx_BMS_AuxThermistors_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_CommitInfo signals in the TX table.
 */
const BMS_CommitInfo_Signals* App_CanTx_BMS_CommitInfo_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_StackRemaining signals in the TX table.
 */
const BMS_StackRemaining_Signals* App_CanTx_BMS_StackRemaining_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_Warnings signals in the TX table.
 */
const BMS_Warnings_Signals* App_CanTx_BMS_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_Faults signals in the TX table.
 */
const BMS_Faults_Signals* App_CanTx_BMS_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_WarningsCounts signals in the TX table.
 */
const BMS_WarningsCounts_Signals* App_CanTx_BMS_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg BMS_FaultsCounts signals in the TX table.
 */
const BMS_FaultsCounts_Signals* App_CanTx_BMS_FaultsCounts_GetData(void);

