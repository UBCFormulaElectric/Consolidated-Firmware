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
 * Update value stored in TX table of signal Heartbeat in msg DCM_Vitals.
 */
void App_CanTx_DCM_Vitals_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal CurrentState in msg DCM_Vitals.
 */
void App_CanTx_DCM_Vitals_CurrentState_Set(DcmState value);

/**
 * Update value stored in TX table of signal TorqueCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(float value);

/**
 * Update value stored in TX table of signal SpeedCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_SpeedCommand_Set(int value);

/**
 * Update value stored in TX table of signal DirectionCommand in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Set(InverterDirection value);

/**
 * Update value stored in TX table of signal EnableInverter in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(bool value);

/**
 * Update value stored in TX table of signal EnableDischarge in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_EnableDischarge_Set(bool value);

/**
 * Update value stored in TX table of signal EnableSpeedMode in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_EnableSpeedMode_Set(bool value);

/**
 * Update value stored in TX table of signal TorqueLimit in msg DCM_LeftInverterCommand.
 */
void App_CanTx_DCM_LeftInverterCommand_TorqueLimit_Set(float value);

/**
 * Update value stored in TX table of signal TorqueCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(float value);

/**
 * Update value stored in TX table of signal SpeedCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_SpeedCommand_Set(int value);

/**
 * Update value stored in TX table of signal DirectionCommand in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_DirectionCommand_Set(InverterDirection value);

/**
 * Update value stored in TX table of signal EnableInverter in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(bool value);

/**
 * Update value stored in TX table of signal EnableDischarge in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_EnableDischarge_Set(bool value);

/**
 * Update value stored in TX table of signal EnableSpeedMode in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_EnableSpeedMode_Set(bool value);

/**
 * Update value stored in TX table of signal TorqueLimit in msg DCM_RightInverterCommand.
 */
void App_CanTx_DCM_RightInverterCommand_TorqueLimit_Set(float value);

/**
 * Update value stored in TX table of signal TxOverflowCount in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_AlertsContext_TxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal RxOverflowCount in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_AlertsContext_RxOverflowCount_Set(uint32_t value);

/**
 * Update value stored in TX table of signal WatchdogTimeoutTaskName in msg DCM_AlertsContext.
 */
void App_CanTx_DCM_AlertsContext_WatchdogTimeoutTaskName_Set(RtosTaskName value);

/**
 * Update value stored in TX table of signal GeneralStatusBitmask in msg DCM_EllipseImuStatus.
 */
void App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Set(uint32_t value);

/**
 * Update value stored in TX table of signal ComStatusBitmask in msg DCM_EllipseImuStatus.
 */
void App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Set(uint32_t value);

/**
 * Update value stored in TX table of signal Timestamp in msg DCM_EllipseImuTime.
 */
void App_CanTx_DCM_EllipseImuTime_Timestamp_Set(uint32_t value);

/**
 * Update value stored in TX table of signal Forward in msg DCM_EllipseImuAcceleration.
 */
void App_CanTx_DCM_EllipseImuAcceleration_Forward_Set(float value);

/**
 * Update value stored in TX table of signal Lateral in msg DCM_EllipseImuAcceleration.
 */
void App_CanTx_DCM_EllipseImuAcceleration_Lateral_Set(float value);

/**
 * Update value stored in TX table of signal Vertical in msg DCM_EllipseImuAcceleration.
 */
void App_CanTx_DCM_EllipseImuAcceleration_Vertical_Set(float value);

/**
 * Update value stored in TX table of signal Roll in msg DCM_EllipseImuAngularVelocity.
 */
void App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Set(int value);

/**
 * Update value stored in TX table of signal Pitch in msg DCM_EllipseImuAngularVelocity.
 */
void App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Set(int value);

/**
 * Update value stored in TX table of signal Yaw in msg DCM_EllipseImuAngularVelocity.
 */
void App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Set(int value);

/**
 * Update value stored in TX table of signal Roll in msg DCM_EllipseImuEulerAngles.
 */
void App_CanTx_DCM_EllipseImuEulerAngles_Roll_Set(float value);

/**
 * Update value stored in TX table of signal Pitch in msg DCM_EllipseImuEulerAngles.
 */
void App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Set(float value);

/**
 * Update value stored in TX table of signal Yaw in msg DCM_EllipseImuEulerAngles.
 */
void App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Set(float value);

/**
 * Update value stored in TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_WATCHDOG_TIMEOUT in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_TX_OVERFLOW in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_WARNING_RX_OVERFLOW in msg DCM_Warnings.
 */
void App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_FAULT_STATE_FAULT in msg DCM_Faults.
 */
void App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Set(bool value);

/**
 * Update value stored in TX table of signal DCM_FAULT_MISSING_HEARTBEAT in msg DCM_Faults.
 */
void App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Set(bool value);

/**
 * Return value from TX table of signal Heartbeat in msg DCM_Vitals.
 */
bool App_CanTx_DCM_Vitals_Heartbeat_Get(void);

/**
 * Return value from TX table of signal CurrentState in msg DCM_Vitals.
 */
DcmState App_CanTx_DCM_Vitals_CurrentState_Get(void);

/**
 * Return value from TX table of signal TorqueCommand in msg DCM_LeftInverterCommand.
 */
float App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get(void);

/**
 * Return value from TX table of signal SpeedCommand in msg DCM_LeftInverterCommand.
 */
int App_CanTx_DCM_LeftInverterCommand_SpeedCommand_Get(void);

/**
 * Return value from TX table of signal DirectionCommand in msg DCM_LeftInverterCommand.
 */
InverterDirection App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Get(void);

/**
 * Return value from TX table of signal EnableInverter in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterCommand_EnableInverter_Get(void);

/**
 * Return value from TX table of signal EnableDischarge in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterCommand_EnableDischarge_Get(void);

/**
 * Return value from TX table of signal EnableSpeedMode in msg DCM_LeftInverterCommand.
 */
bool App_CanTx_DCM_LeftInverterCommand_EnableSpeedMode_Get(void);

/**
 * Return value from TX table of signal TorqueLimit in msg DCM_LeftInverterCommand.
 */
float App_CanTx_DCM_LeftInverterCommand_TorqueLimit_Get(void);

/**
 * Return value from TX table of signal TorqueCommand in msg DCM_RightInverterCommand.
 */
float App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get(void);

/**
 * Return value from TX table of signal SpeedCommand in msg DCM_RightInverterCommand.
 */
int App_CanTx_DCM_RightInverterCommand_SpeedCommand_Get(void);

/**
 * Return value from TX table of signal DirectionCommand in msg DCM_RightInverterCommand.
 */
InverterDirection App_CanTx_DCM_RightInverterCommand_DirectionCommand_Get(void);

/**
 * Return value from TX table of signal EnableInverter in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterCommand_EnableInverter_Get(void);

/**
 * Return value from TX table of signal EnableDischarge in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterCommand_EnableDischarge_Get(void);

/**
 * Return value from TX table of signal EnableSpeedMode in msg DCM_RightInverterCommand.
 */
bool App_CanTx_DCM_RightInverterCommand_EnableSpeedMode_Get(void);

/**
 * Return value from TX table of signal TorqueLimit in msg DCM_RightInverterCommand.
 */
float App_CanTx_DCM_RightInverterCommand_TorqueLimit_Get(void);

/**
 * Return value from TX table of signal TxOverflowCount in msg DCM_AlertsContext.
 */
uint32_t App_CanTx_DCM_AlertsContext_TxOverflowCount_Get(void);

/**
 * Return value from TX table of signal RxOverflowCount in msg DCM_AlertsContext.
 */
uint32_t App_CanTx_DCM_AlertsContext_RxOverflowCount_Get(void);

/**
 * Return value from TX table of signal WatchdogTimeoutTaskName in msg DCM_AlertsContext.
 */
RtosTaskName App_CanTx_DCM_AlertsContext_WatchdogTimeoutTaskName_Get(void);

/**
 * Return value from TX table of signal GeneralStatusBitmask in msg DCM_EllipseImuStatus.
 */
uint32_t App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Get(void);

/**
 * Return value from TX table of signal ComStatusBitmask in msg DCM_EllipseImuStatus.
 */
uint32_t App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Get(void);

/**
 * Return value from TX table of signal Timestamp in msg DCM_EllipseImuTime.
 */
uint32_t App_CanTx_DCM_EllipseImuTime_Timestamp_Get(void);

/**
 * Return value from TX table of signal Forward in msg DCM_EllipseImuAcceleration.
 */
float App_CanTx_DCM_EllipseImuAcceleration_Forward_Get(void);

/**
 * Return value from TX table of signal Lateral in msg DCM_EllipseImuAcceleration.
 */
float App_CanTx_DCM_EllipseImuAcceleration_Lateral_Get(void);

/**
 * Return value from TX table of signal Vertical in msg DCM_EllipseImuAcceleration.
 */
float App_CanTx_DCM_EllipseImuAcceleration_Vertical_Get(void);

/**
 * Return value from TX table of signal Roll in msg DCM_EllipseImuAngularVelocity.
 */
int App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Get(void);

/**
 * Return value from TX table of signal Pitch in msg DCM_EllipseImuAngularVelocity.
 */
int App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Get(void);

/**
 * Return value from TX table of signal Yaw in msg DCM_EllipseImuAngularVelocity.
 */
int App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Get(void);

/**
 * Return value from TX table of signal Roll in msg DCM_EllipseImuEulerAngles.
 */
float App_CanTx_DCM_EllipseImuEulerAngles_Roll_Get(void);

/**
 * Return value from TX table of signal Pitch in msg DCM_EllipseImuEulerAngles.
 */
float App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Get(void);

/**
 * Return value from TX table of signal Yaw in msg DCM_EllipseImuEulerAngles.
 */
float App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_WATCHDOG_TIMEOUT in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_TX_OVERFLOW in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get(void);

/**
 * Return value from TX table of signal DCM_WARNING_RX_OVERFLOW in msg DCM_Warnings.
 */
bool App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get(void);

/**
 * Return value from TX table of signal DCM_FAULT_STATE_FAULT in msg DCM_Faults.
 */
bool App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get(void);

/**
 * Return value from TX table of signal DCM_FAULT_MISSING_HEARTBEAT in msg DCM_Faults.
 */
bool App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get(void);

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
 * Returns pointer to the struct of msg DCM_EllipseImuStatus signals in the TX table.
 */
const DCM_EllipseImuStatus_Signals* App_CanTx_DCM_EllipseImuStatus_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseImuTime signals in the TX table.
 */
const DCM_EllipseImuTime_Signals* App_CanTx_DCM_EllipseImuTime_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseImuAcceleration signals in the TX table.
 */
const DCM_EllipseImuAcceleration_Signals* App_CanTx_DCM_EllipseImuAcceleration_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseImuAngularVelocity signals in the TX table.
 */
const DCM_EllipseImuAngularVelocity_Signals* App_CanTx_DCM_EllipseImuAngularVelocity_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_EllipseImuEulerAngles signals in the TX table.
 */
const DCM_EllipseImuEulerAngles_Signals* App_CanTx_DCM_EllipseImuEulerAngles_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_Warnings signals in the TX table.
 */
const DCM_Warnings_Signals* App_CanTx_DCM_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg DCM_Faults signals in the TX table.
 */
const DCM_Faults_Signals* App_CanTx_DCM_Faults_GetData(void);

