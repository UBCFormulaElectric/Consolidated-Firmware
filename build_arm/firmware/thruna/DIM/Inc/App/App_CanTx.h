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
 * Update value stored in TX table of signal Heartbeat in msg DIM_Vitals.
 */
void App_CanTx_DIM_Vitals_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal State in msg DIM_Vitals.
 */
void App_CanTx_DIM_Vitals_State_Set(DimState value);

/**
 * Update value stored in TX table of signal StartSwitch in msg DIM_Switches.
 */
void App_CanTx_DIM_Switches_StartSwitch_Set(SwitchState value);

/**
 * Update value stored in TX table of signal AuxSwitch in msg DIM_Switches.
 */
void App_CanTx_DIM_Switches_AuxSwitch_Set(SwitchState value);

/**
 * Update value stored in TX table of signal TxOverflowCount in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_AlertsContext_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal RxOverflowCount in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_AlertsContext_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal WatchdogTimeoutTaskName in msg DIM_AlertsContext.
 */
void App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_WATCHDOG_TIMEOUT in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_TX_OVERFLOW in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_WARNING_RX_OVERFLOW in msg DIM_Warnings.
 */
void App_CanTx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Set(bool value);

/**
 * Update value stored in TX table of signal DIM_FAULT_MISSING_HEARTBEAT in msg DIM_Faults.
 */
void App_CanTx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Set(bool value);

/**
 * Return value from TX table of signal Heartbeat in msg DIM_Vitals.
 */
bool App_CanTx_DIM_Vitals_Heartbeat_Get(void);

/**
 * Return value from TX table of signal State in msg DIM_Vitals.
 */
DimState App_CanTx_DIM_Vitals_State_Get(void);

/**
 * Return value from TX table of signal StartSwitch in msg DIM_Switches.
 */
SwitchState App_CanTx_DIM_Switches_StartSwitch_Get(void);

/**
 * Return value from TX table of signal AuxSwitch in msg DIM_Switches.
 */
SwitchState App_CanTx_DIM_Switches_AuxSwitch_Get(void);

/**
 * Return value from TX table of signal TxOverflowCount in msg DIM_AlertsContext.
 */
uint32_t App_CanTx_DIM_AlertsContext_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal RxOverflowCount in msg DIM_AlertsContext.
 */
uint32_t App_CanTx_DIM_AlertsContext_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal WatchdogTimeoutTaskName in msg DIM_AlertsContext.
 */
RtosTaskName App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_WATCHDOG_TIMEOUT in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_TX_OVERFLOW in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from TX table of signal DIM_WARNING_RX_OVERFLOW in msg DIM_Warnings.
 */
bool App_CanTx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from TX table of signal DIM_FAULT_MISSING_HEARTBEAT in msg DIM_Faults.
 */
bool App_CanTx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Get(void);

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
 * Returns pointer to the struct of msg DIM_Warnings signals in the TX table.
 */
const DIM_Warnings_Signals* App_CanTx_DIM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg DIM_Faults signals in the TX table.
 */
const DIM_Faults_Signals* App_CanTx_DIM_Faults_GetData(void);

