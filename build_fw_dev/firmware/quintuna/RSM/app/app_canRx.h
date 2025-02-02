/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void app_canRx_init(void);

/**
 * Update value stored in RX table of signal BMS_Fault_DummyFault in msg BMS_Faults.
 */
void app_canRx_BMS_Fault_DummyFault_update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_DummyFault in msg FSM_Faults.
 */
void app_canRx_FSM_Fault_DummyFault_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_StackWaterMarkHighTask1Hz in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_StackWaterMarkHighTask100Hz in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_StackWaterMarkHighTask1kHz in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_StackWaterMarkHighTaskCanRx in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_StackWaterMarkHighTaskCanTx in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_WatchdogTimeout in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_WatchdogTimeout_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_TxOverflow in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_TxOverflow_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Warning_RxOverflow in msg CRIT_Warnings.
 */
void app_canRx_CRIT_Warning_RxOverflow_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Fault_MissingBMSHeartbeat in msg CRIT_Faults.
 */
void app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Fault_MissingFSMHeartbeat in msg CRIT_Faults.
 */
void app_canRx_CRIT_Fault_MissingFSMHeartbeat_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Fault_MissingVCHeartbeat in msg CRIT_Faults.
 */
void app_canRx_CRIT_Fault_MissingVCHeartbeat_update(bool value);

/**
 * Update value stored in RX table of signal CRIT_Fault_MissingRSMHeartbeat in msg CRIT_Faults.
 */
void app_canRx_CRIT_Fault_MissingRSMHeartbeat_update(bool value);

/**
 * Update value stored in RX table of signal VC_Fault_DummyFault in msg VC_Faults.
 */
void app_canRx_VC_Fault_DummyFault_update(bool value);

/**
 * Return value from RX table of signal BMS_Fault_DummyFault in msg BMS_Faults.
 */
bool app_canRx_BMS_Fault_DummyFault_get(void);

/**
 * Return value from RX table of signal FSM_Fault_DummyFault in msg FSM_Faults.
 */
bool app_canRx_FSM_Fault_DummyFault_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_StackWaterMarkHighTask1Hz in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_StackWaterMarkHighTask100Hz in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_StackWaterMarkHighTask1kHz in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_StackWaterMarkHighTaskCanRx in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_StackWaterMarkHighTaskCanTx in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_WatchdogTimeout in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_WatchdogTimeout_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_TxOverflow in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_TxOverflow_get(void);

/**
 * Return value from RX table of signal CRIT_Warning_RxOverflow in msg CRIT_Warnings.
 */
bool app_canRx_CRIT_Warning_RxOverflow_get(void);

/**
 * Return value from RX table of signal CRIT_Fault_MissingBMSHeartbeat in msg CRIT_Faults.
 */
bool app_canRx_CRIT_Fault_MissingBMSHeartbeat_get(void);

/**
 * Return value from RX table of signal CRIT_Fault_MissingFSMHeartbeat in msg CRIT_Faults.
 */
bool app_canRx_CRIT_Fault_MissingFSMHeartbeat_get(void);

/**
 * Return value from RX table of signal CRIT_Fault_MissingVCHeartbeat in msg CRIT_Faults.
 */
bool app_canRx_CRIT_Fault_MissingVCHeartbeat_get(void);

/**
 * Return value from RX table of signal CRIT_Fault_MissingRSMHeartbeat in msg CRIT_Faults.
 */
bool app_canRx_CRIT_Fault_MissingRSMHeartbeat_get(void);

/**
 * Return value from RX table of signal VC_Fault_DummyFault in msg VC_Faults.
 */
bool app_canRx_VC_Fault_DummyFault_get(void);

