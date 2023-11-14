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
 * Update value stored in TX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
void App_CanTx_PDM_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_State in msg PDM_Vitals.
 */
void App_CanTx_PDM_State_Set(PdmState value);

/**
 * Update value stored in TX table of signal PDM_StackWatermarkAboveThresholdTask1Hz in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_StackWatermarkAboveThresholdTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWatermarkAboveThresholdTask100Hz in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_StackWatermarkAboveThresholdTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWatermarkAboveThresholdTask1kHz in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_StackWatermarkAboveThresholdTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWatermarkAboveThresholdTaskCanTx in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWatermarkAboveThresholdTaskCanRx in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_WatchdogTimeout in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_TxOverflowCount in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_RxOverflowCount in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_WatchdogTimeoutTaskName in msg PDM_AlertsContext.
 */
void App_CanTx_PDM_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal PDM_BatVoltage in msg PDM_Voltages.
 */
void App_CanTx_PDM_BatVoltage_Set(float value);

/**
 * Update value stored in TX table of signal PDM_BoostVoltage in msg PDM_Voltages.
 */
void App_CanTx_PDM_BoostVoltage_Set(float value);

/**
 * Update value stored in TX table of signal PDM_AccVoltage in msg PDM_Voltages.
 */
void App_CanTx_PDM_AccVoltage_Set(float value);

/**
 * Update value stored in TX table of signal PDM_StackRemainingTask1Hz in msg PDM_StackRemaining.
 */
void App_CanTx_PDM_StackRemainingTask1Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackRemainingTask100Hz in msg PDM_StackRemaining.
 */
void App_CanTx_PDM_StackRemainingTask100Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackRemainingTask1kHz in msg PDM_StackRemaining.
 */
void App_CanTx_PDM_StackRemainingTask1kHz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackRemainingTaskCanRx in msg PDM_StackRemaining.
 */
void App_CanTx_PDM_StackRemainingTaskCanRx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackRemainingTaskCanTx in msg PDM_StackRemaining.
 */
void App_CanTx_PDM_StackRemainingTaskCanTx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Hash in msg PDM_CommitInfo.
 */
void App_CanTx_PDM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Clean in msg PDM_CommitInfo.
 */
void App_CanTx_PDM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask1HzWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_StackWaterMarkHighTask1HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask100HzWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_StackWaterMarkHighTask100HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask1kHzWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_StackWaterMarkHighTask1kHzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTaskCanRxWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTaskCanTxWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_WatchdogTimeoutWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_WatchdogTimeoutWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_TxOverflowWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_TxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_RxOverflowWarning in msg PDM_Warnings.
 */
void App_CanTx_PDM_RxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_DummyFault in msg PDM_Faults.
 */
void App_CanTx_PDM_DummyFault_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask1HzWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask100HzWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTask1kHzWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTaskCanRxWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_StackWaterMarkHighTaskCanTxWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_WatchdogTimeoutWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_WatchdogTimeoutWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_TxOverflowWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_TxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_RxOverflowWarningCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_RxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_DummyFaultCount in msg PDM_FaultsCounts.
 */
void App_CanTx_PDM_DummyFaultCount_Set(uint32_t value);

/**
 * Return value from TX table of signal PDM_Heartbeat in msg PDM_Vitals.
 */
bool App_CanTx_PDM_Heartbeat_Get(void);

/**
 * Return value from TX table of signal PDM_State in msg PDM_Vitals.
 */
PdmState App_CanTx_PDM_State_Get(void);

/**
 * Return value from TX table of signal PDM_StackWatermarkAboveThresholdTask1Hz in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_StackWatermarkAboveThresholdTask1Hz_Get(void);

/**
 * Return value from TX table of signal PDM_StackWatermarkAboveThresholdTask100Hz in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_StackWatermarkAboveThresholdTask100Hz_Get(void);

/**
 * Return value from TX table of signal PDM_StackWatermarkAboveThresholdTask1kHz in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_StackWatermarkAboveThresholdTask1kHz_Get(void);

/**
 * Return value from TX table of signal PDM_StackWatermarkAboveThresholdTaskCanTx in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanTx_Get(void);

/**
 * Return value from TX table of signal PDM_StackWatermarkAboveThresholdTaskCanRx in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanRx_Get(void);

/**
 * Return value from TX table of signal PDM_WatchdogTimeout in msg PDM_AlertsContext.
 */
bool App_CanTx_PDM_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal PDM_TxOverflowCount in msg PDM_AlertsContext.
 */
uint32_t App_CanTx_PDM_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal PDM_RxOverflowCount in msg PDM_AlertsContext.
 */
uint32_t App_CanTx_PDM_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal PDM_WatchdogTimeoutTaskName in msg PDM_AlertsContext.
 */
RtosTaskName App_CanTx_PDM_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal PDM_BatVoltage in msg PDM_Voltages.
 */
float App_CanTx_PDM_BatVoltage_Get(void);

/**
 * Return value from TX table of signal PDM_BoostVoltage in msg PDM_Voltages.
 */
float App_CanTx_PDM_BoostVoltage_Get(void);

/**
 * Return value from TX table of signal PDM_AccVoltage in msg PDM_Voltages.
 */
float App_CanTx_PDM_AccVoltage_Get(void);

/**
 * Return value from TX table of signal PDM_StackRemainingTask1Hz in msg PDM_StackRemaining.
 */
uint32_t App_CanTx_PDM_StackRemainingTask1Hz_Get(void);

/**
 * Return value from TX table of signal PDM_StackRemainingTask100Hz in msg PDM_StackRemaining.
 */
uint32_t App_CanTx_PDM_StackRemainingTask100Hz_Get(void);

/**
 * Return value from TX table of signal PDM_StackRemainingTask1kHz in msg PDM_StackRemaining.
 */
uint32_t App_CanTx_PDM_StackRemainingTask1kHz_Get(void);

/**
 * Return value from TX table of signal PDM_StackRemainingTaskCanRx in msg PDM_StackRemaining.
 */
uint32_t App_CanTx_PDM_StackRemainingTaskCanRx_Get(void);

/**
 * Return value from TX table of signal PDM_StackRemainingTaskCanTx in msg PDM_StackRemaining.
 */
uint32_t App_CanTx_PDM_StackRemainingTaskCanTx_Get(void);

/**
 * Return value from TX table of signal PDM_Hash in msg PDM_CommitInfo.
 */
uint32_t App_CanTx_PDM_Hash_Get(void);

/**
 * Return value from TX table of signal PDM_Clean in msg PDM_CommitInfo.
 */
bool App_CanTx_PDM_Clean_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask1HzWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask100HzWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask1kHzWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTaskCanRxWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTaskCanTxWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from TX table of signal PDM_WatchdogTimeoutWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from TX table of signal PDM_TxOverflowWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_TxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal PDM_RxOverflowWarning in msg PDM_Warnings.
 */
bool App_CanTx_PDM_RxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal PDM_DummyFault in msg PDM_Faults.
 */
bool App_CanTx_PDM_DummyFault_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask1HzWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_StackWaterMarkHighTask1HzWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask100HzWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_StackWaterMarkHighTask100HzWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTask1kHzWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_StackWaterMarkHighTask1kHzWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTaskCanRxWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_StackWaterMarkHighTaskCanTxWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_WatchdogTimeoutWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_WatchdogTimeoutWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_TxOverflowWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_TxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_RxOverflowWarningCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_RxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal PDM_DummyFaultCount in msg PDM_FaultsCounts.
 */
uint32_t App_CanTx_PDM_DummyFaultCount_Get(void);

/**
 * Returns pointer to the struct of msg PDM_Vitals signals in the TX table.
 */
const PDM_Vitals_Signals* App_CanTx_PDM_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_AlertsContext signals in the TX table.
 */
const PDM_AlertsContext_Signals* App_CanTx_PDM_AlertsContext_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_Voltages signals in the TX table.
 */
const PDM_Voltages_Signals* App_CanTx_PDM_Voltages_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_StackRemaining signals in the TX table.
 */
const PDM_StackRemaining_Signals* App_CanTx_PDM_StackRemaining_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_CommitInfo signals in the TX table.
 */
const PDM_CommitInfo_Signals* App_CanTx_PDM_CommitInfo_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_Warnings signals in the TX table.
 */
const PDM_Warnings_Signals* App_CanTx_PDM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_Faults signals in the TX table.
 */
const PDM_Faults_Signals* App_CanTx_PDM_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_WarningsCounts signals in the TX table.
 */
const PDM_WarningsCounts_Signals* App_CanTx_PDM_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg PDM_FaultsCounts signals in the TX table.
 */
const PDM_FaultsCounts_Signals* App_CanTx_PDM_FaultsCounts_GetData(void);

