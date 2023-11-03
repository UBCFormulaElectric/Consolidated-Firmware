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
 * Update value stored in TX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
void App_CanTx_DCM_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_State in msg DCM_Vitals.
 */
void App_CanTx_DCM_State_Set(DcmState value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterTorqueCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterTorqueCommand_Set(float value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterSpeedCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterSpeedCommand_Set(int value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterDirectionCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterDirectionCommand_Set(InverterDirection value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterEnable in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterEnable_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterEnableDischarge in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterEnableDischarge_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterEnableSpeedMode in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterEnableSpeedMode_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_LeftInverterTorqueLimit in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterTorqueLimit_Set(float value);

/**
 * Update value stored in TX table of signal DCM_RightInverterTorqueCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterTorqueCommand_Set(float value);

/**
 * Update value stored in TX table of signal DCM_RightInverterSpeedCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterSpeedCommand_Set(int value);

/**
 * Update value stored in TX table of signal DCM_RightInverterDirectionCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterDirectionCommand_Set(InverterDirection value);

/**
 * Update value stored in TX table of signal DCM_RightInverterEnable in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterEnable_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_RightInverterEnableDischarge in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterEnableDischarge_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_RightInverterEnableSpeedMode in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterEnableSpeedMode_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_RightInverterTorqueLimit in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterTorqueLimit_Set(float value);

/**
 * Update value stored in TX table of signal DCM_TxOverflowCount in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_RxOverflowCount in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_WatchdogTimeoutTaskName in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal DCM_EllipseGeneralStatusBitmask in msg DCM_EllipseStatus.
 */
void App_CanTx_DCM_EllipseGeneralStatusBitmask_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_EllipseComStatusBitmask in msg DCM_EllipseStatus.
 */
void App_CanTx_DCM_EllipseComStatusBitmask_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_EllipseTimestamp in msg DCM_EllipseTime.
 */
void App_CanTx_DCM_EllipseTimestamp_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_AccelerationForward in msg DCM_EllipseAcceleration.
 */
void App_CanTx_DCM_AccelerationForward_Set(float value);

/**
 * Update value stored in TX table of signal DCM_AccelerationLateral in msg DCM_EllipseAcceleration.
 */
void App_CanTx_DCM_AccelerationLateral_Set(float value);

/**
 * Update value stored in TX table of signal DCM_AccelerationVertical in msg DCM_EllipseAcceleration.
 */
void App_CanTx_DCM_AccelerationVertical_Set(float value);

/**
 * Update value stored in TX table of signal DCM_AngularVelocityRoll in msg DCM_EllipseAngularVelocity.
 */
void App_CanTx_DCM_AngularVelocityRoll_Set(int value);

/**
 * Update value stored in TX table of signal DCM_AngularVelocityPitch in msg DCM_EllipseAngularVelocity.
 */
void App_CanTx_DCM_AngularVelocityPitch_Set(int value);

/**
 * Update value stored in TX table of signal DCM_AngularVelocityYaw in msg DCM_EllipseAngularVelocity.
 */
void App_CanTx_DCM_AngularVelocityYaw_Set(int value);

/**
 * Update value stored in TX table of signal DCM_EulerAnglesRoll in msg DCM_EllipseEulerAngles.
 */
void App_CanTx_DCM_EulerAnglesRoll_Set(float value);

/**
 * Update value stored in TX table of signal DCM_EulerAnglesPitch in msg DCM_EllipseEulerAngles.
 */
void App_CanTx_DCM_EulerAnglesPitch_Set(float value);

/**
 * Update value stored in TX table of signal DCM_EulerAnglesYaw in msg DCM_EllipseEulerAngles.
 */
void App_CanTx_DCM_EulerAnglesYaw_Set(float value);

/**
 * Update value stored in TX table of signal DCM_Hash in msg DCM_CommitInfo.
 */
void App_CanTx_DCM_Hash_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Clean in msg DCM_CommitInfo.
 */
void App_CanTx_DCM_Clean_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask1Hz_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask100Hz_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHz_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_TxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warning_RxOverflow_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
void App_CanTx_DCM_Fault_MissingHeartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
void App_CanTx_DCM_Fault_LeftInverterFault_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
void App_CanTx_DCM_Fault_RightInverterFault_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask1HzCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask100HzCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTask1kHzCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTaskCanRxCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_StackWaterMarkHighTaskCanTxCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_WatchdogTimeoutCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_WatchdogTimeoutCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_TxOverflowCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Warning_RxOverflowCount in msg DCM_WarningsCounts.
 */
void App_CanTx_DCM_Warning_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Fault_MissingHeartbeatCount in msg DCM_FaultsCounts.
 */
void App_CanTx_DCM_Fault_MissingHeartbeatCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Fault_LeftInverterFaultCount in msg DCM_FaultsCounts.
 */
void App_CanTx_DCM_Fault_LeftInverterFaultCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal DCM_Fault_RightInverterFaultCount in msg DCM_FaultsCounts.
 */
void App_CanTx_DCM_Fault_RightInverterFaultCount_Set(uint32_t value);

/**
 * Return value from TX table of signal DCM_Heartbeat in msg DCM_Vitals.
 */
bool App_CanTx_DCM_Heartbeat_Get(void);

/**
 * Return value from TX table of signal DCM_State in msg DCM_Vitals.
 */
DcmState App_CanTx_DCM_State_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterTorqueCommand in msg DCM_LeftInverterCommand.
 */
float App_CanTx_DCM_LeftInverterTorqueCommand_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterSpeedCommand in msg DCM_LeftInverterCommand.
 */
int App_CanTx_DCM_LeftInverterSpeedCommand_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterDirectionCommand in msg DCM_LeftInverterCommand.
 */
InverterDirection App_CanTx_DCM_LeftInverterDirectionCommand_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterEnable in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterEnable_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterEnableDischarge in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterEnableDischarge_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterEnableSpeedMode in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterEnableSpeedMode_Get(void);

/**
 * Return value from TX table of signal DCM_LeftInverterTorqueLimit in msg DCM_LeftInverterCommand.
 */
float App_CanTx_DCM_LeftInverterTorqueLimit_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterTorqueCommand in msg DCM_RightInverterCommand.
 */
float App_CanTx_DCM_RightInverterTorqueCommand_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterSpeedCommand in msg DCM_RightInverterCommand.
 */
int App_CanTx_DCM_RightInverterSpeedCommand_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterDirectionCommand in msg DCM_RightInverterCommand.
 */
InverterDirection App_CanTx_DCM_RightInverterDirectionCommand_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterEnable in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterEnable_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterEnableDischarge in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterEnableDischarge_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterEnableSpeedMode in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterEnableSpeedMode_Get(void);

/**
 * Return value from TX table of signal DCM_RightInverterTorqueLimit in msg DCM_RightInverterCommand.
 */
float App_CanTx_DCM_RightInverterTorqueLimit_Get(void);

/**
 * Return value from TX table of signal DCM_TxOverflowCount in msg DCM_AlertsContext.
 */
uint32_t App_CanTx_DCM_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DCM_RxOverflowCount in msg DCM_AlertsContext.
 */
uint32_t App_CanTx_DCM_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DCM_WatchdogTimeoutTaskName in msg DCM_AlertsContext.
 */
RtosTaskName App_CanTx_DCM_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal DCM_EllipseGeneralStatusBitmask in msg DCM_EllipseStatus.
 */
uint32_t App_CanTx_DCM_EllipseGeneralStatusBitmask_Get(void);

/**
 * Return value from TX table of signal DCM_EllipseComStatusBitmask in msg DCM_EllipseStatus.
 */
uint32_t App_CanTx_DCM_EllipseComStatusBitmask_Get(void);

/**
 * Return value from TX table of signal DCM_EllipseTimestamp in msg DCM_EllipseTime.
 */
uint32_t App_CanTx_DCM_EllipseTimestamp_Get(void);

/**
 * Return value from TX table of signal DCM_AccelerationForward in msg DCM_EllipseAcceleration.
 */
float App_CanTx_DCM_AccelerationForward_Get(void);

/**
 * Return value from TX table of signal DCM_AccelerationLateral in msg DCM_EllipseAcceleration.
 */
float App_CanTx_DCM_AccelerationLateral_Get(void);

/**
 * Return value from TX table of signal DCM_AccelerationVertical in msg DCM_EllipseAcceleration.
 */
float App_CanTx_DCM_AccelerationVertical_Get(void);

/**
 * Return value from TX table of signal DCM_AngularVelocityRoll in msg DCM_EllipseAngularVelocity.
 */
int App_CanTx_DCM_AngularVelocityRoll_Get(void);

/**
 * Return value from TX table of signal DCM_AngularVelocityPitch in msg DCM_EllipseAngularVelocity.
 */
int App_CanTx_DCM_AngularVelocityPitch_Get(void);

/**
 * Return value from TX table of signal DCM_AngularVelocityYaw in msg DCM_EllipseAngularVelocity.
 */
int App_CanTx_DCM_AngularVelocityYaw_Get(void);

/**
 * Return value from TX table of signal DCM_EulerAnglesRoll in msg DCM_EllipseEulerAngles.
 */
float App_CanTx_DCM_EulerAnglesRoll_Get(void);

/**
 * Return value from TX table of signal DCM_EulerAnglesPitch in msg DCM_EllipseEulerAngles.
 */
float App_CanTx_DCM_EulerAnglesPitch_Get(void);

/**
 * Return value from TX table of signal DCM_EulerAnglesYaw in msg DCM_EllipseEulerAngles.
 */
float App_CanTx_DCM_EulerAnglesYaw_Get(void);

/**
 * Return value from TX table of signal DCM_Hash in msg DCM_CommitInfo.
 */
uint32_t App_CanTx_DCM_Hash_Get(void);

/**
 * Return value from TX table of signal DCM_Clean in msg DCM_CommitInfo.
 */
bool App_CanTx_DCM_Clean_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask1Hz in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_StackWaterMarkHighTask1Hz_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask100Hz in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_StackWaterMarkHighTask100Hz_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask1kHz in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHz_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTaskCanRx in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTaskCanTx in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_WatchdogTimeout in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_TxOverflow in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_TxOverflow_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_RxOverflow in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warning_RxOverflow_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_MissingHeartbeat in msg DCM_Faults.
 */
bool App_CanTx_DCM_Fault_MissingHeartbeat_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_LeftInverterFault in msg DCM_Faults.
 */
bool App_CanTx_DCM_Fault_LeftInverterFault_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_RightInverterFault in msg DCM_Faults.
 */
bool App_CanTx_DCM_Fault_RightInverterFault_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask1HzCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask1HzCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask100HzCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask100HzCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTask1kHzCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHzCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTaskCanRxCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRxCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_StackWaterMarkHighTaskCanTxCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTxCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_WatchdogTimeoutCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_WatchdogTimeoutCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_TxOverflowCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DCM_Warning_RxOverflowCount in msg DCM_WarningsCounts.
 */
uint32_t App_CanTx_DCM_Warning_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_MissingHeartbeatCount in msg DCM_FaultsCounts.
 */
uint32_t App_CanTx_DCM_Fault_MissingHeartbeatCount_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_LeftInverterFaultCount in msg DCM_FaultsCounts.
 */
uint32_t App_CanTx_DCM_Fault_LeftInverterFaultCount_Get(void);

/**
 * Return value from TX table of signal DCM_Fault_RightInverterFaultCount in msg DCM_FaultsCounts.
 */
uint32_t App_CanTx_DCM_Fault_RightInverterFaultCount_Get(void);

/**
 * Returns pointer to the struct of msg DCM_Vitals signals in the TX table.
 */
const DCM_Vitals_Signals* App_CanTx_DCM_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_LeftInverterCommand signals in the TX table.
 */
const DCM_LeftInverterCommand_Signals* App_CanTx_DCM_LeftInverterCommand_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_RightInverterCommand signals in the TX table.
 */
const DCM_RightInverterCommand_Signals* App_CanTx_DCM_RightInverterCommand_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_AlertsContext signals in the TX table.
 */
const DCM_AlertsContext_Signals* App_CanTx_DCM_AlertsContext_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseStatus signals in the TX table.
 */
const DCM_EllipseStatus_Signals* App_CanTx_DCM_EllipseStatus_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseTime signals in the TX table.
 */
const DCM_EllipseTime_Signals* App_CanTx_DCM_EllipseTime_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseAcceleration signals in the TX table.
 */
const DCM_EllipseAcceleration_Signals* App_CanTx_DCM_EllipseAcceleration_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseAngularVelocity signals in the TX table.
 */
const DCM_EllipseAngularVelocity_Signals* App_CanTx_DCM_EllipseAngularVelocity_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseEulerAngles signals in the TX table.
 */
const DCM_EllipseEulerAngles_Signals* App_CanTx_DCM_EllipseEulerAngles_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_CommitInfo signals in the TX table.
 */
const DCM_CommitInfo_Signals* App_CanTx_DCM_CommitInfo_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_Warnings signals in the TX table.
 */
const DCM_Warnings_Signals* App_CanTx_DCM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_Faults signals in the TX table.
 */
const DCM_Faults_Signals* App_CanTx_DCM_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_WarningsCounts signals in the TX table.
 */
const DCM_WarningsCounts_Signals* App_CanTx_DCM_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_FaultsCounts signals in the TX table.
 */
const DCM_FaultsCounts_Signals* App_CanTx_DCM_FaultsCounts_GetData(void);

