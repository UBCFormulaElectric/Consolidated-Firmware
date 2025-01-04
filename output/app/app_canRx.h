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
 * Update value stored in RX table of signal BRUSA_IsConnected in msg BRUSA_Vitals.
 */
void app_canRx_BRUSA_IsConnected_update(bool value);

/**
 * Update value stored in RX table of signal VC_Heartbeat in msg VC_Vitals.
 */
void app_canRx_VC_Vitals_VC_Heartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_State in msg VC_Vitals.
 */
void app_canRx_VC_Vitals_VC_State_update(VcState value);/**
 * Update value stored in RX table of signal VC_BuzzerOn in msg VC_Vitals.
 */
void app_canRx_VC_Vitals_VC_BuzzerOn_update(bool value);/**
 * Update value stored in RX table of signal VC_NumberOfCanDataLogs in msg VC_Vitals.
 */
void app_canRx_VC_Vitals_VC_NumberOfCanDataLogs_update(uint32_t value);/**
 * Update value stored in RX table of signal VC_CanLoggingRemainingErrors in msg VC_Vitals.
 */
void app_canRx_VC_Vitals_VC_CanLoggingRemainingErrors_update(uint32_t value);
/**
 * Update value stored in RX table of signal VC_Warning_StackWaterMarkHighTask1Hz in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_StackWaterMarkHighTask1Hz_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_StackWaterMarkHighTask100Hz in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_StackWaterMarkHighTask100Hz_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_StackWaterMarkHighTask1kHz in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_StackWaterMarkHighTask1kHz_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_StackWaterMarkHighTaskCanRx in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_StackWaterMarkHighTaskCanRx_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_StackWaterMarkHighTaskCanTx in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_StackWaterMarkHighTaskCanTx_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_WatchdogTimeout in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_WatchdogTimeout_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_TxOverflow in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_TxOverflow_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_RxOverflow in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_RxOverflow_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_RegenNotAvailable in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_RegenNotAvailable_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_ImuInitFailed in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_ImuInitFailed_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_FlowRateInputOutOfRange in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_FlowRateInputOutOfRange_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_BatteryRailOvercurrentFault in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_BatteryRailOvercurrentFault_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_AccumulatorRailOvercurrentFault in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_AccumulatorRailOvercurrentFault_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_LvChargeFault in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_LvChargeFault_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_BoostControllerFault in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_BoostControllerFault_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_HighNumberOfCanDataLogs in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_HighNumberOfCanDataLogs_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_CanLoggingSdCardNotPresent in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_CanLoggingSdCardNotPresent_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_CanLoggingErrored in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_CanLoggingErrored_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_BrakeAppsDisagreement in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_BrakeAppsDisagreement_update(bool value);/**
 * Update value stored in RX table of signal VC_Warning_SbgInitFailed in msg VC_Warnings.
 */
void app_canRx_VC_Warnings_VC_Warning_SbgInitFailed_update(bool value);
/**
 * Update value stored in RX table of signal VC_Fault_MissingBMSHeartbeat in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_MissingBMSHeartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_Fault_MissingFSMHeartbeat in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_MissingFSMHeartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_Fault_MissingRSMHeartbeat in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_MissingRSMHeartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_Fault_MissingCRITHeartbeat in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_MissingCRITHeartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_Fault_LeftInverterFault in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_LeftInverterFault_update(bool value);/**
 * Update value stored in RX table of signal VC_Fault_RightInverterFault in msg VC_Faults.
 */
void app_canRx_VC_Faults_VC_Fault_RightInverterFault_update(bool value);
