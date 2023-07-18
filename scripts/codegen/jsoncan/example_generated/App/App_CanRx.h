/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void App_CanRx_Init(void);

/**
 * Update value stored in RX table of signal PappsMappedPedalPercentage in msg FsmApps.
 */
void App_CanRx_FsmApps_PappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal SappsMappedPedalPercentage in msg FsmApps.
 */
void App_CanRx_FsmApps_SappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal PappsOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_PappsOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal SappsOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_SappsOutOfRange_Update(bool value);

/**
 * Update value stored in RX table of signal StackWatermarkAboveThresholdTask1Hz in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Update(bool value);

/**
 * Update value stored in RX table of signal StackWatermarkAboveThresholdTask1kHz in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Update(bool value);

/**
 * Update value stored in RX table of signal StackWatermarkAboveThresholdTaskCanRx in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Update(bool value);

/**
 * Update value stored in RX table of signal StackWatermarkAboveThresholdTaskCanTx in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Update(bool value);

/**
 * Update value stored in RX table of signal WatchdogFault in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_WatchdogFault_Update(bool value);

/**
 * Update value stored in RX table of signal BspdFault in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_BspdFault_Update(bool value);

/**
 * Update value stored in RX table of signal LeftWheelSpeedOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Update(uint32_t value);

/**
 * Update value stored in RX table of signal RightWheelSpeedOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Update(uint32_t value);

/**
 * Update value stored in RX table of signal FlowRateOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_FlowRateOutOfRange_Update(uint32_t value);

/**
 * Update value stored in RX table of signal SteeringAngleOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Update(uint32_t value);

/**
 * Update value stored in RX table of signal BrakePressureOutOfRange in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_BrakePressureOutOfRange_Update(uint32_t value);

/**
 * Update value stored in RX table of signal BrakePressureSc in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_BrakePressureSc_Update(bool value);

/**
 * Update value stored in RX table of signal BrakePressureOc in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_BrakePressureOc_Update(bool value);

/**
 * Update value stored in RX table of signal SteeringWheelBroke in msg FsmWarnings.
 */
void App_CanRx_FsmWarnings_SteeringWheelBroke_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_TEST1 in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_TEST1_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_TEST2 in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warnings_FSM_TEST2_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_FAULT_TEST3 in msg FSM_Faults.
 */
void App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Update(bool value);

/**
 * Return value from RX table of signal PappsMappedPedalPercentage in msg FsmApps.
 */
float App_CanRx_FsmApps_PappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal SappsMappedPedalPercentage in msg FsmApps.
 */
float App_CanRx_FsmApps_SappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal PappsOutOfRange in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_PappsOutOfRange_Get(void);

/**
 * Return value from RX table of signal SappsOutOfRange in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_SappsOutOfRange_Get(void);

/**
 * Return value from RX table of signal StackWatermarkAboveThresholdTask1Hz in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Get(void);

/**
 * Return value from RX table of signal StackWatermarkAboveThresholdTask1kHz in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Get(void);

/**
 * Return value from RX table of signal StackWatermarkAboveThresholdTaskCanRx in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Get(void);

/**
 * Return value from RX table of signal StackWatermarkAboveThresholdTaskCanTx in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Get(void);

/**
 * Return value from RX table of signal WatchdogFault in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_WatchdogFault_Get(void);

/**
 * Return value from RX table of signal BspdFault in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_BspdFault_Get(void);

/**
 * Return value from RX table of signal LeftWheelSpeedOutOfRange in msg FsmWarnings.
 */
uint32_t App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Get(void);

/**
 * Return value from RX table of signal RightWheelSpeedOutOfRange in msg FsmWarnings.
 */
uint32_t App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Get(void);

/**
 * Return value from RX table of signal FlowRateOutOfRange in msg FsmWarnings.
 */
uint32_t App_CanRx_FsmWarnings_FlowRateOutOfRange_Get(void);

/**
 * Return value from RX table of signal SteeringAngleOutOfRange in msg FsmWarnings.
 */
uint32_t App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Get(void);

/**
 * Return value from RX table of signal BrakePressureOutOfRange in msg FsmWarnings.
 */
uint32_t App_CanRx_FsmWarnings_BrakePressureOutOfRange_Get(void);

/**
 * Return value from RX table of signal BrakePressureSc in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_BrakePressureSc_Get(void);

/**
 * Return value from RX table of signal BrakePressureOc in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_BrakePressureOc_Get(void);

/**
 * Return value from RX table of signal SteeringWheelBroke in msg FsmWarnings.
 */
bool App_CanRx_FsmWarnings_SteeringWheelBroke_Get(void);

/**
 * Return value from RX table of signal FSM_TEST1 in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_TEST1_Get(void);

/**
 * Return value from RX table of signal FSM_TEST2 in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warnings_FSM_TEST2_Get(void);

/**
 * Return value from RX table of signal FSM_FAULT_TEST3 in msg FSM_Faults.
 */
bool App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Get(void);

