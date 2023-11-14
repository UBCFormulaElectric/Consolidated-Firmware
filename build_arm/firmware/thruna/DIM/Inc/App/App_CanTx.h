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
 * Update value stored in TX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
void App_CanTx_DIM_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_State in msg DIM_Vitals.
 */
void App_CanTx_DIM_State_Set(DimState value);

/**
 * Update value stored in TX table of signal DIM_StartSwitch in msg DIM_Switches.
 */
void App_CanTx_DIM_StartSwitch_Set(SwitchState value);

/**
 * Update value stored in TX table of signal DIM_AuxSwitch in msg DIM_Switches.
 */
void App_CanTx_DIM_AuxSwitch_Set(SwitchState value);

/**
 * Update value stored in TX table of signal DIM_TxOverflowCount in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_RxOverflowCount in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_WatchdogTimeoutTaskName in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal DIM_StackRemainingTask1Hz in msg DIM_StackRemaining.
 */
void App_CanTx_DIM_StackRemainingTask1Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackRemainingTask100Hz in msg DIM_StackRemaining.
 */
void App_CanTx_DIM_StackRemainingTask100Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackRemainingTask1kHz in msg DIM_StackRemaining.
 */
void App_CanTx_DIM_StackRemainingTask1kHz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackRemainingTaskCanRx in msg DIM_StackRemaining.
 */
void App_CanTx_DIM_StackRemainingTaskCanRx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackRemainingTaskCanTx in msg DIM_StackRemaining.
 */
void App_CanTx_DIM_StackRemainingTaskCanTx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Hash in msg DIM_CommitInfo.
 */
void App_CanTx_DIM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Clean in msg DIM_CommitInfo.
 */
void App_CanTx_DIM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask1HzWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_StackWaterMarkHighTask1HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask100HzWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_StackWaterMarkHighTask100HzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask1kHzWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_StackWaterMarkHighTask1kHzWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTaskCanRxWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTaskCanTxWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WatchdogTimeoutWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_WatchdogTimeoutWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_TxOverflowWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_TxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_RxOverflowWarning in msg DIM_Warnings.
 */
void App_CanTx_DIM_RxOverflowWarning_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_MissingHeartbeatFault in msg DIM_Faults.
 */
void App_CanTx_DIM_MissingHeartbeatFault_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask1HzWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask100HzWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTask1kHzWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTaskCanRxWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_StackWaterMarkHighTaskCanTxWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_WatchdogTimeoutWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_WatchdogTimeoutWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_TxOverflowWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_TxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_RxOverflowWarningCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_RxOverflowWarningCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_MissingHeartbeatFaultCount in msg DIM_FaultsCounts.
 */
void App_CanTx_DIM_MissingHeartbeatFaultCount_Set(uint32_t value);

/**
 * Return value from TX table of signal DIM_Heartbeat in msg DIM_Vitals.
 */
bool App_CanTx_DIM_Heartbeat_Get(void);

/**
 * Return value from TX table of signal DIM_State in msg DIM_Vitals.
 */
DimState App_CanTx_DIM_State_Get(void);

/**
 * Return value from TX table of signal DIM_StartSwitch in msg DIM_Switches.
 */
SwitchState App_CanTx_DIM_StartSwitch_Get(void);

/**
 * Return value from TX table of signal DIM_AuxSwitch in msg DIM_Switches.
 */
SwitchState App_CanTx_DIM_AuxSwitch_Get(void);

/**
 * Return value from TX table of signal DIM_TxOverflowCount in msg DIM_AlertsContext.
 */
uint32_t App_CanTx_DIM_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DIM_RxOverflowCount in msg DIM_AlertsContext.
 */
uint32_t App_CanTx_DIM_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DIM_WatchdogTimeoutTaskName in msg DIM_AlertsContext.
 */
RtosTaskName App_CanTx_DIM_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal DIM_StackRemainingTask1Hz in msg DIM_StackRemaining.
 */
uint32_t App_CanTx_DIM_StackRemainingTask1Hz_Get(void);

/**
 * Return value from TX table of signal DIM_StackRemainingTask100Hz in msg DIM_StackRemaining.
 */
uint32_t App_CanTx_DIM_StackRemainingTask100Hz_Get(void);

/**
 * Return value from TX table of signal DIM_StackRemainingTask1kHz in msg DIM_StackRemaining.
 */
uint32_t App_CanTx_DIM_StackRemainingTask1kHz_Get(void);

/**
 * Return value from TX table of signal DIM_StackRemainingTaskCanRx in msg DIM_StackRemaining.
 */
uint32_t App_CanTx_DIM_StackRemainingTaskCanRx_Get(void);

/**
 * Return value from TX table of signal DIM_StackRemainingTaskCanTx in msg DIM_StackRemaining.
 */
uint32_t App_CanTx_DIM_StackRemainingTaskCanTx_Get(void);

/**
 * Return value from TX table of signal DIM_Hash in msg DIM_CommitInfo.
 */
uint32_t App_CanTx_DIM_Hash_Get(void);

/**
 * Return value from TX table of signal DIM_Clean in msg DIM_CommitInfo.
 */
bool App_CanTx_DIM_Clean_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask1HzWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_StackWaterMarkHighTask1HzWarning_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask100HzWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_StackWaterMarkHighTask100HzWarning_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask1kHzWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_StackWaterMarkHighTask1kHzWarning_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTaskCanRxWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarning_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTaskCanTxWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarning_Get(void);

/**
 * Return value from TX table of signal DIM_WatchdogTimeoutWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_WatchdogTimeoutWarning_Get(void);

/**
 * Return value from TX table of signal DIM_TxOverflowWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_TxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal DIM_RxOverflowWarning in msg DIM_Warnings.
 */
bool App_CanTx_DIM_RxOverflowWarning_Get(void);

/**
 * Return value from TX table of signal DIM_MissingHeartbeatFault in msg DIM_Faults.
 */
bool App_CanTx_DIM_MissingHeartbeatFault_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask1HzWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_StackWaterMarkHighTask1HzWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask100HzWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_StackWaterMarkHighTask100HzWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTask1kHzWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_StackWaterMarkHighTask1kHzWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTaskCanRxWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_StackWaterMarkHighTaskCanTxWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_WatchdogTimeoutWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_WatchdogTimeoutWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_TxOverflowWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_TxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_RxOverflowWarningCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_RxOverflowWarningCount_Get(void);

/**
 * Return value from TX table of signal DIM_MissingHeartbeatFaultCount in msg DIM_FaultsCounts.
 */
uint32_t App_CanTx_DIM_MissingHeartbeatFaultCount_Get(void);

/**
 * Returns pointer to the struct of msg DIM_Vitals signals in the TX table.
 */
const DIM_Vitals_Signals* App_CanTx_DIM_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_Switches signals in the TX table.
 */
const DIM_Switches_Signals* App_CanTx_DIM_Switches_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_AlertsContext signals in the TX table.
 */
const DIM_AlertsContext_Signals* App_CanTx_DIM_AlertsContext_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_StackRemaining signals in the TX table.
 */
const DIM_StackRemaining_Signals* App_CanTx_DIM_StackRemaining_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_CommitInfo signals in the TX table.
 */
const DIM_CommitInfo_Signals* App_CanTx_DIM_CommitInfo_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_Warnings signals in the TX table.
 */
const DIM_Warnings_Signals* App_CanTx_DIM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_Faults signals in the TX table.
 */
const DIM_Faults_Signals* App_CanTx_DIM_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_WarningsCounts signals in the TX table.
 */
const DIM_WarningsCounts_Signals* App_CanTx_DIM_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_FaultsCounts signals in the TX table.
 */
const DIM_FaultsCounts_Signals* App_CanTx_DIM_FaultsCounts_GetData(void);

