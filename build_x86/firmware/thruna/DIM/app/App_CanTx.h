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
 * Update value stored in TX table of signal DIM_Hash in msg DIM_CommitInfo.
 */
void App_CanTx_DIM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Clean in msg DIM_CommitInfo.
 */
void App_CanTx_DIM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_TxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warning_RxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
void App_CanTx_DIM_Fault_MissingHeartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask1HzCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask100HzCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTask1kHzCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTaskCanRxCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_StackWaterMarkHighTaskCanTxCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_WatchdogTimeoutCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_WatchdogTimeoutCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_TxOverflowCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Warning_RxOverflowCount in msg DIM_WarningsCounts.
 */
void App_CanTx_DIM_Warning_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DIM_Fault_MissingHeartbeatCount in msg DIM_FaultsCounts.
 */
void App_CanTx_DIM_Fault_MissingHeartbeatCount_Set(uint32_t value);

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
 * Return value from TX table of signal DIM_Hash in msg DIM_CommitInfo.
 */
uint32_t App_CanTx_DIM_Hash_Get(void);

/**
 * Return value from TX table of signal DIM_Clean in msg DIM_CommitInfo.
 */
bool App_CanTx_DIM_Clean_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask1Hz in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask100Hz in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask1kHz in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTaskCanRx in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTaskCanTx in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_WatchdogTimeout in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_TxOverflow in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_TxOverflow_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_RxOverflow in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warning_RxOverflow_Get(void);

/**
 * Return value from TX table of signal DIM_Fault_MissingHeartbeat in msg DIM_Faults.
 */
bool App_CanTx_DIM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask1HzCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask100HzCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTask1kHzCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTaskCanRxCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_StackWaterMarkHighTaskCanTxCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_WatchdogTimeoutCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_WatchdogTimeoutCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_TxOverflowCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DIM_Warning_RxOverflowCount in msg DIM_WarningsCounts.
 */
uint32_t App_CanTx_DIM_Warning_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DIM_Fault_MissingHeartbeatCount in msg DIM_FaultsCounts.
 */
uint32_t App_CanTx_DIM_Fault_MissingHeartbeatCount_Get(void);

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

