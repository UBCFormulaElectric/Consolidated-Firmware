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
 * Update value stored in TX table of signal GSM_Heartbeat in msg GSM_GSM_Vitals.
 */
void App_CanTx_GSM_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_StackWatermarkAboveThresholdTask1Hz in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_StackWatermarkAboveThresholdTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_StackWatermarkAboveThresholdTask100Hz in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_StackWatermarkAboveThresholdTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_StackWatermarkAboveThresholdTask1kHz in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_StackWatermarkAboveThresholdTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_StackWatermarkAboveThresholdTaskCanTx in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_StackWatermarkAboveThresholdTaskCanRx in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_WatchdogTimeout in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal GSM_TxOverflowCount in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_RxOverflowCount in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_WatchdogTimeoutTaskName in msg GSM_GSM_Warnings.
 */
void App_CanTx_GSM_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal GSM_StackRemainingTask1Hz in msg GSM_StackRemaining.
 */
void App_CanTx_GSM_StackRemainingTask1Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_StackRemainingTask100Hz in msg GSM_StackRemaining.
 */
void App_CanTx_GSM_StackRemainingTask100Hz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_StackRemainingTask1kHz in msg GSM_StackRemaining.
 */
void App_CanTx_GSM_StackRemainingTask1kHz_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_StackRemainingTaskCanRx in msg GSM_StackRemaining.
 */
void App_CanTx_GSM_StackRemainingTaskCanRx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_StackRemainingTaskCanTx in msg GSM_StackRemaining.
 */
void App_CanTx_GSM_StackRemainingTaskCanTx_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_Hash in msg GSM_CommitInfo.
 */
void App_CanTx_GSM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal GSM_Clean in msg GSM_CommitInfo.
 */
void App_CanTx_GSM_Clean_Set(bool value);

/**
 * Return value from TX table of signal GSM_Heartbeat in msg GSM_GSM_Vitals.
 */
bool App_CanTx_GSM_Heartbeat_Get(void);

/**
 * Return value from TX table of signal GSM_StackWatermarkAboveThresholdTask1Hz in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_StackWatermarkAboveThresholdTask1Hz_Get(void);

/**
 * Return value from TX table of signal GSM_StackWatermarkAboveThresholdTask100Hz in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_StackWatermarkAboveThresholdTask100Hz_Get(void);

/**
 * Return value from TX table of signal GSM_StackWatermarkAboveThresholdTask1kHz in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_StackWatermarkAboveThresholdTask1kHz_Get(void);

/**
 * Return value from TX table of signal GSM_StackWatermarkAboveThresholdTaskCanTx in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanTx_Get(void);

/**
 * Return value from TX table of signal GSM_StackWatermarkAboveThresholdTaskCanRx in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanRx_Get(void);

/**
 * Return value from TX table of signal GSM_WatchdogTimeout in msg GSM_GSM_Warnings.
 */
bool App_CanTx_GSM_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal GSM_TxOverflowCount in msg GSM_GSM_Warnings.
 */
uint32_t App_CanTx_GSM_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal GSM_RxOverflowCount in msg GSM_GSM_Warnings.
 */
uint32_t App_CanTx_GSM_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal GSM_WatchdogTimeoutTaskName in msg GSM_GSM_Warnings.
 */
RtosTaskName App_CanTx_GSM_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal GSM_StackRemainingTask1Hz in msg GSM_StackRemaining.
 */
uint32_t App_CanTx_GSM_StackRemainingTask1Hz_Get(void);

/**
 * Return value from TX table of signal GSM_StackRemainingTask100Hz in msg GSM_StackRemaining.
 */
uint32_t App_CanTx_GSM_StackRemainingTask100Hz_Get(void);

/**
 * Return value from TX table of signal GSM_StackRemainingTask1kHz in msg GSM_StackRemaining.
 */
uint32_t App_CanTx_GSM_StackRemainingTask1kHz_Get(void);

/**
 * Return value from TX table of signal GSM_StackRemainingTaskCanRx in msg GSM_StackRemaining.
 */
uint32_t App_CanTx_GSM_StackRemainingTaskCanRx_Get(void);

/**
 * Return value from TX table of signal GSM_StackRemainingTaskCanTx in msg GSM_StackRemaining.
 */
uint32_t App_CanTx_GSM_StackRemainingTaskCanTx_Get(void);

/**
 * Return value from TX table of signal GSM_Hash in msg GSM_CommitInfo.
 */
uint32_t App_CanTx_GSM_Hash_Get(void);

/**
 * Return value from TX table of signal GSM_Clean in msg GSM_CommitInfo.
 */
bool App_CanTx_GSM_Clean_Get(void);

/**
 * Returns pointer to the struct of msg GSM_GSM_Vitals signals in the TX table.
 */
const GSM_GSM_Vitals_Signals* App_CanTx_GSM_GSM_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg GSM_GSM_Warnings signals in the TX table.
 */
const GSM_GSM_Warnings_Signals* App_CanTx_GSM_GSM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg GSM_StackRemaining signals in the TX table.
 */
const GSM_StackRemaining_Signals* App_CanTx_GSM_StackRemaining_GetData(void);

/**
 * Returns pointer to the struct of msg GSM_CommitInfo signals in the TX table.
 */
const GSM_CommitInfo_Signals* App_CanTx_GSM_CommitInfo_GetData(void);

