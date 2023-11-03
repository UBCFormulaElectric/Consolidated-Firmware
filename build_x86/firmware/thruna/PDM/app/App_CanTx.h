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
 * Update value stored in TX table of signal PDM_Hash in msg PDM_CommitInfo.
 */
void App_CanTx_PDM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Clean in msg PDM_CommitInfo.
 */
void App_CanTx_PDM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_TxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
void App_CanTx_PDM_Warning_RxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
void App_CanTx_PDM_Fault_DummyFault_Set(bool value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask1HzCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask100HzCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTask1kHzCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTaskCanRxCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_StackWaterMarkHighTaskCanTxCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_WatchdogTimeoutCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_WatchdogTimeoutCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_TxOverflowCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Warning_RxOverflowCount in msg PDM_WarningsCounts.
 */
void App_CanTx_PDM_Warning_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal PDM_Fault_DummyFaultCount in msg PDM_FaultsCounts.
 */
void App_CanTx_PDM_Fault_DummyFaultCount_Set(uint32_t value);

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
 * Return value from TX table of signal PDM_Hash in msg PDM_CommitInfo.
 */
uint32_t App_CanTx_PDM_Hash_Get(void);

/**
 * Return value from TX table of signal PDM_Clean in msg PDM_CommitInfo.
 */
bool App_CanTx_PDM_Clean_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask1Hz in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask100Hz in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask1kHz in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTaskCanRx in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTaskCanTx in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_WatchdogTimeout in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_TxOverflow in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_TxOverflow_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_RxOverflow in msg PDM_Warnings.
 */
bool App_CanTx_PDM_Warning_RxOverflow_Get(void);

/**
 * Return value from TX table of signal PDM_Fault_DummyFault in msg PDM_Faults.
 */
bool App_CanTx_PDM_Fault_DummyFault_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask1HzCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_StackWaterMarkHighTask1HzCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask100HzCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_StackWaterMarkHighTask100HzCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTask1kHzCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_StackWaterMarkHighTask1kHzCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTaskCanRxCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanRxCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_StackWaterMarkHighTaskCanTxCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_StackWaterMarkHighTaskCanTxCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_WatchdogTimeoutCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_WatchdogTimeoutCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_TxOverflowCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal PDM_Warning_RxOverflowCount in msg PDM_WarningsCounts.
 */
uint32_t App_CanTx_PDM_Warning_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal PDM_Fault_DummyFaultCount in msg PDM_FaultsCounts.
 */
uint32_t App_CanTx_PDM_Fault_DummyFaultCount_Get(void);

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

