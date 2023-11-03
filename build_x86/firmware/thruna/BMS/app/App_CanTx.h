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
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_TxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
void App_CanTx_BMS_Warning_RxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_MissingHeartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_StateMachine_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_CellUndervoltage_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_CellOvervoltage_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_ModuleCommunicationError_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_CellUndertemp_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_CellOvertemp_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_Charger_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_ChargerDisconnectedDuringCharge_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_ChargerExternalShutdown_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_TractiveSystemOvercurrent_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
void App_CanTx_BMS_Fault_PrechargeFailure_Set(bool value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_WatchdogTimeoutCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
void App_CanTx_BMS_Warning_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_MissingHeartbeatCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_MissingHeartbeatCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_StateMachineCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_CellUndervoltageCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_CellOvervoltageCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_ModuleCommunicationErrorCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_CellUndertempCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_CellOvertempCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_ChargerCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_ChargerCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_ChargerExternalShutdownCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_ChargerExternalShutdownCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_TractiveSystemOvercurrentCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
void App_CanTx_BMS_Fault_PrechargeFailureCount_Set(uint32_t value);

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
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask1Hz in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask100Hz in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask1kHz in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRx in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTx in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_WatchdogTimeout in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_TxOverflow in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_TxOverflow_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_RxOverflow in msg BMS_Warnings.
 */
bool App_CanTx_BMS_Warning_RxOverflow_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_MissingHeartbeat in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_StateMachine in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_StateMachine_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellUndervoltage in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_CellUndervoltage_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellOvervoltage in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_CellOvervoltage_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ModuleCommunicationError in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_ModuleCommunicationError_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellUndertemp in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_CellUndertemp_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellOvertemp in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_CellOvertemp_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_Charger in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_Charger_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ChargerDisconnectedDuringCharge in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_ChargerDisconnectedDuringCharge_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ChargerExternalShutdown in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_ChargerExternalShutdown_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_TractiveSystemOvercurrent in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_TractiveSystemOvercurrent_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_PrechargeFailure in msg BMS_Faults.
 */
bool App_CanTx_BMS_Fault_PrechargeFailure_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask1HzCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask1HzCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask100HzCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask100HzCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTask1kHzCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTaskCanRxCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_StackWaterMarkHighTaskCanTxCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_WatchdogTimeoutCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_WatchdogTimeoutCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_TxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal BMS_Warning_RxOverflowCount in msg BMS_WarningsCounts.
 */
uint32_t App_CanTx_BMS_Warning_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_MissingHeartbeatCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_MissingHeartbeatCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_StateMachineCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_StateMachineCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellUndervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_CellUndervoltageCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellOvervoltageCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_CellOvervoltageCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ModuleCommunicationErrorCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_ModuleCommunicationErrorCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellUndertempCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_CellUndertempCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_CellOvertempCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_CellOvertempCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ChargerCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_ChargerCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ChargerDisconnectedDuringChargeCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_ChargerExternalShutdownCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_ChargerExternalShutdownCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_TractiveSystemOvercurrentCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_TractiveSystemOvercurrentCount_Get(void);

/**
 * Return value from TX table of signal BMS_Fault_PrechargeFailureCount in msg BMS_FaultsCounts.
 */
uint32_t App_CanTx_BMS_Fault_PrechargeFailureCount_Get(void);

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

