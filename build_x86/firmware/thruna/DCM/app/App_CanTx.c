/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanTx.h"


/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages transmitted by DCM (i.e. the TX table).
 */
typedef struct
{
    DCM_Vitals_Signals DCM_Vitals_signals;
    DCM_LeftInverterCommand_Signals DCM_LeftInverterCommand_signals;
    DCM_RightInverterCommand_Signals DCM_RightInverterCommand_signals;
    DCM_AlertsContext_Signals DCM_AlertsContext_signals;
    DCM_EllipseStatus_Signals DCM_EllipseStatus_signals;
    DCM_EllipseTime_Signals DCM_EllipseTime_signals;
    DCM_EllipseAcceleration_Signals DCM_EllipseAcceleration_signals;
    DCM_EllipseAngularVelocity_Signals DCM_EllipseAngularVelocity_signals;
    DCM_EllipseEulerAngles_Signals DCM_EllipseEulerAngles_signals;
    DCM_CommitInfo_Signals DCM_CommitInfo_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
    DCM_WarningsCounts_Signals DCM_WarningsCounts_signals;
    DCM_FaultsCounts_Signals DCM_FaultsCounts_signals;
} DCM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static DCM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(DCM_TxMsgs));
    App_CanTx_DCM_Heartbeat_Set(CANSIG_DCM_VITALS_DCM_HEARTBEAT_START_VAL);
    App_CanTx_DCM_State_Set(CANSIG_DCM_VITALS_DCM_STATE_START_VAL);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterSpeedCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterDirectionCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterEnable_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_START_VAL);
    App_CanTx_DCM_LeftInverterEnableDischarge_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    App_CanTx_DCM_LeftInverterEnableSpeedMode_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    App_CanTx_DCM_LeftInverterTorqueLimit_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_START_VAL);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterSpeedCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterDirectionCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterEnable_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_START_VAL);
    App_CanTx_DCM_RightInverterEnableDischarge_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_START_VAL);
    App_CanTx_DCM_RightInverterEnableSpeedMode_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_START_VAL);
    App_CanTx_DCM_RightInverterTorqueLimit_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_START_VAL);
    App_CanTx_DCM_TxOverflowCount_Set(CANSIG_DCM_ALERTS_CONTEXT_DCM_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_RxOverflowCount_Set(CANSIG_DCM_ALERTS_CONTEXT_DCM_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_WatchdogTimeoutTaskName_Set(CANSIG_DCM_ALERTS_CONTEXT_DCM_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_DCM_EllipseGeneralStatusBitmask_Set(CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_GENERAL_STATUS_BITMASK_START_VAL);
    App_CanTx_DCM_EllipseComStatusBitmask_Set(CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_COM_STATUS_BITMASK_START_VAL);
    App_CanTx_DCM_EllipseTimestamp_Set(CANSIG_DCM_ELLIPSE_TIME_DCM_ELLIPSE_TIMESTAMP_START_VAL);
    App_CanTx_DCM_AccelerationForward_Set(CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_START_VAL);
    App_CanTx_DCM_AccelerationLateral_Set(CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_START_VAL);
    App_CanTx_DCM_AccelerationVertical_Set(CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_START_VAL);
    App_CanTx_DCM_AngularVelocityRoll_Set(CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_START_VAL);
    App_CanTx_DCM_AngularVelocityPitch_Set(CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_START_VAL);
    App_CanTx_DCM_AngularVelocityYaw_Set(CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_START_VAL);
    App_CanTx_DCM_EulerAnglesRoll_Set(CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_START_VAL);
    App_CanTx_DCM_EulerAnglesPitch_Set(CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_START_VAL);
    App_CanTx_DCM_EulerAnglesYaw_Set(CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_START_VAL);
    App_CanTx_DCM_Hash_Set(CANSIG_DCM_COMMIT_INFO_DCM_HASH_START_VAL);
    App_CanTx_DCM_Clean_Set(CANSIG_DCM_COMMIT_INFO_DCM_CLEAN_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask1Hz_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask100Hz_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHz_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRx_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTx_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanTx_DCM_Warning_WatchdogTimeout_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_DCM_Warning_TxOverflow_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_DCM_Warning_RxOverflow_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_DCM_Fault_MissingHeartbeat_Set(CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanTx_DCM_Fault_LeftInverterFault_Set(CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_START_VAL);
    App_CanTx_DCM_Fault_RightInverterFault_Set(CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask1HzCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask100HzCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHzCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRxCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTxCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    App_CanTx_DCM_Warning_WatchdogTimeoutCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    App_CanTx_DCM_Warning_TxOverflowCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_Warning_RxOverflowCount_Set(CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_Fault_MissingHeartbeatCount_Set(CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_MISSING_HEARTBEAT_COUNT_START_VAL);
    App_CanTx_DCM_Fault_LeftInverterFaultCount_Set(CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_LEFT_INVERTER_FAULT_COUNT_START_VAL);
    App_CanTx_DCM_Fault_RightInverterFaultCount_Set(CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_RIGHT_INVERTER_FAULT_COUNT_START_VAL);
}

void App_CanTx_DCM_Heartbeat_Set(bool value)
{
    tx_table.DCM_Vitals_signals.DCM_Heartbeat_value = value;
}

void App_CanTx_DCM_State_Set(DcmState value)
{
    tx_table.DCM_Vitals_signals.DCM_State_value = (value > CANSIG_DCM_VITALS_DCM_STATE_MAX) ? CANSIG_DCM_VITALS_DCM_STATE_MAX : value;
}

void App_CanTx_DCM_LeftInverterTorqueCommand_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_LeftInverterSpeedCommand_Set(int value)
{
    const int tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterSpeedCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_LeftInverterDirectionCommand_Set(InverterDirection value)
{
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterDirectionCommand_value = (value > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_MAX : value;
}

void App_CanTx_DCM_LeftInverterEnable_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnable_value = value;
}

void App_CanTx_DCM_LeftInverterEnableDischarge_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableDischarge_value = value;
}

void App_CanTx_DCM_LeftInverterEnableSpeedMode_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableSpeedMode_value = value;
}

void App_CanTx_DCM_LeftInverterTorqueLimit_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueLimit_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_MAX : tmp;
}

void App_CanTx_DCM_RightInverterTorqueCommand_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_RightInverterSpeedCommand_Set(int value)
{
    const int tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterSpeedCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_RightInverterDirectionCommand_Set(InverterDirection value)
{
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterDirectionCommand_value = (value > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_MAX : value;
}

void App_CanTx_DCM_RightInverterEnable_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnable_value = value;
}

void App_CanTx_DCM_RightInverterEnableDischarge_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableDischarge_value = value;
}

void App_CanTx_DCM_RightInverterEnableSpeedMode_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableSpeedMode_value = value;
}

void App_CanTx_DCM_RightInverterTorqueLimit_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueLimit_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_MAX : tmp;
}

void App_CanTx_DCM_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_AlertsContext_signals.DCM_TxOverflowCount_value = (value > CANSIG_DCM_ALERTS_CONTEXT_DCM_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_DCM_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_AlertsContext_signals.DCM_RxOverflowCount_value = (value > CANSIG_DCM_ALERTS_CONTEXT_DCM_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_DCM_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.DCM_AlertsContext_signals.DCM_WatchdogTimeoutTaskName_value = (value > CANSIG_DCM_ALERTS_CONTEXT_DCM_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_DCM_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_DCM_EllipseGeneralStatusBitmask_Set(uint32_t value)
{
    tx_table.DCM_EllipseStatus_signals.DCM_EllipseGeneralStatusBitmask_value = (value > CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_GENERAL_STATUS_BITMASK_MAX) ? CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_GENERAL_STATUS_BITMASK_MAX : value;
}

void App_CanTx_DCM_EllipseComStatusBitmask_Set(uint32_t value)
{
    tx_table.DCM_EllipseStatus_signals.DCM_EllipseComStatusBitmask_value = value;
}

void App_CanTx_DCM_EllipseTimestamp_Set(uint32_t value)
{
    tx_table.DCM_EllipseTime_signals.DCM_EllipseTimestamp_value = value;
}

void App_CanTx_DCM_AccelerationForward_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_MIN ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_MIN : value;
    tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationForward_value = tmp > CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_MAX ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_MAX : tmp;
}

void App_CanTx_DCM_AccelerationLateral_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_MIN ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_MIN : value;
    tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationLateral_value = tmp > CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_MAX ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_MAX : tmp;
}

void App_CanTx_DCM_AccelerationVertical_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_MIN ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_MIN : value;
    tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationVertical_value = tmp > CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_MAX ? CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_MAX : tmp;
}

void App_CanTx_DCM_AngularVelocityRoll_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_MIN ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_MIN : value;
    tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityRoll_value = tmp > CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_MAX ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_MAX : tmp;
}

void App_CanTx_DCM_AngularVelocityPitch_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_MIN ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_MIN : value;
    tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityPitch_value = tmp > CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_MAX ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_MAX : tmp;
}

void App_CanTx_DCM_AngularVelocityYaw_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_MIN ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_MIN : value;
    tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityYaw_value = tmp > CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_MAX ? CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_MAX : tmp;
}

void App_CanTx_DCM_EulerAnglesRoll_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_MIN ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_MIN : value;
    tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesRoll_value = tmp > CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_MAX ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_MAX : tmp;
}

void App_CanTx_DCM_EulerAnglesPitch_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_MIN ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_MIN : value;
    tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesPitch_value = tmp > CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_MAX ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_MAX : tmp;
}

void App_CanTx_DCM_EulerAnglesYaw_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_MIN ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_MIN : value;
    tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesYaw_value = tmp > CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_MAX ? CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_MAX : tmp;
}

void App_CanTx_DCM_Hash_Set(uint32_t value)
{
    tx_table.DCM_CommitInfo_signals.DCM_Hash_value = value;
}

void App_CanTx_DCM_Clean_Set(bool value)
{
    tx_table.DCM_CommitInfo_signals.DCM_Clean_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask1Hz_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask100Hz_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHz_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanTx_DCM_Warning_WatchdogTimeout_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_WatchdogTimeout_value = value;
}

void App_CanTx_DCM_Warning_TxOverflow_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_TxOverflow_value = value;
}

void App_CanTx_DCM_Warning_RxOverflow_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_Warning_RxOverflow_value = value;
}

void App_CanTx_DCM_Fault_MissingHeartbeat_Set(bool value)
{
    tx_table.DCM_Faults_signals.DCM_Fault_MissingHeartbeat_value = value;
}

void App_CanTx_DCM_Fault_LeftInverterFault_Set(bool value)
{
    tx_table.DCM_Faults_signals.DCM_Fault_LeftInverterFault_value = value;
}

void App_CanTx_DCM_Fault_RightInverterFault_Set(bool value)
{
    tx_table.DCM_Faults_signals.DCM_Fault_RightInverterFault_value = value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask1HzCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask100HzCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask1kHzCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTaskCanRxCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTaskCanTxCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_WatchdogTimeoutCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_WatchdogTimeoutCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_TxOverflowCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_Warning_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_WarningsCounts_signals.DCM_Warning_RxOverflowCount_value = (value > CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_Fault_MissingHeartbeatCount_Set(uint32_t value)
{
    tx_table.DCM_FaultsCounts_signals.DCM_Fault_MissingHeartbeatCount_value = (value > CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_MISSING_HEARTBEAT_COUNT_MAX) ? CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_MISSING_HEARTBEAT_COUNT_MAX : value;
}

void App_CanTx_DCM_Fault_LeftInverterFaultCount_Set(uint32_t value)
{
    tx_table.DCM_FaultsCounts_signals.DCM_Fault_LeftInverterFaultCount_value = (value > CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_LEFT_INVERTER_FAULT_COUNT_MAX) ? CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_LEFT_INVERTER_FAULT_COUNT_MAX : value;
}

void App_CanTx_DCM_Fault_RightInverterFaultCount_Set(uint32_t value)
{
    tx_table.DCM_FaultsCounts_signals.DCM_Fault_RightInverterFaultCount_value = (value > CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_RIGHT_INVERTER_FAULT_COUNT_MAX) ? CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_RIGHT_INVERTER_FAULT_COUNT_MAX : value;
}

bool App_CanTx_DCM_Heartbeat_Get()
{
    return tx_table.DCM_Vitals_signals.DCM_Heartbeat_value;
}

DcmState App_CanTx_DCM_State_Get()
{
    return tx_table.DCM_Vitals_signals.DCM_State_value;
}

float App_CanTx_DCM_LeftInverterTorqueCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueCommand_value;
}

int App_CanTx_DCM_LeftInverterSpeedCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterSpeedCommand_value;
}

InverterDirection App_CanTx_DCM_LeftInverterDirectionCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterDirectionCommand_value;
}

bool App_CanTx_DCM_LeftInverterEnable_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnable_value;
}

bool App_CanTx_DCM_LeftInverterEnableDischarge_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableDischarge_value;
}

bool App_CanTx_DCM_LeftInverterEnableSpeedMode_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterEnableSpeedMode_value;
}

float App_CanTx_DCM_LeftInverterTorqueLimit_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DCM_LeftInverterTorqueLimit_value;
}

float App_CanTx_DCM_RightInverterTorqueCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueCommand_value;
}

int App_CanTx_DCM_RightInverterSpeedCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterSpeedCommand_value;
}

InverterDirection App_CanTx_DCM_RightInverterDirectionCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterDirectionCommand_value;
}

bool App_CanTx_DCM_RightInverterEnable_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnable_value;
}

bool App_CanTx_DCM_RightInverterEnableDischarge_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableDischarge_value;
}

bool App_CanTx_DCM_RightInverterEnableSpeedMode_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterEnableSpeedMode_value;
}

float App_CanTx_DCM_RightInverterTorqueLimit_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DCM_RightInverterTorqueLimit_value;
}

uint32_t App_CanTx_DCM_TxOverflowCount_Get()
{
    return tx_table.DCM_AlertsContext_signals.DCM_TxOverflowCount_value;
}

uint32_t App_CanTx_DCM_RxOverflowCount_Get()
{
    return tx_table.DCM_AlertsContext_signals.DCM_RxOverflowCount_value;
}

RtosTaskName App_CanTx_DCM_WatchdogTimeoutTaskName_Get()
{
    return tx_table.DCM_AlertsContext_signals.DCM_WatchdogTimeoutTaskName_value;
}

uint32_t App_CanTx_DCM_EllipseGeneralStatusBitmask_Get()
{
    return tx_table.DCM_EllipseStatus_signals.DCM_EllipseGeneralStatusBitmask_value;
}

uint32_t App_CanTx_DCM_EllipseComStatusBitmask_Get()
{
    return tx_table.DCM_EllipseStatus_signals.DCM_EllipseComStatusBitmask_value;
}

uint32_t App_CanTx_DCM_EllipseTimestamp_Get()
{
    return tx_table.DCM_EllipseTime_signals.DCM_EllipseTimestamp_value;
}

float App_CanTx_DCM_AccelerationForward_Get()
{
    return tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationForward_value;
}

float App_CanTx_DCM_AccelerationLateral_Get()
{
    return tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationLateral_value;
}

float App_CanTx_DCM_AccelerationVertical_Get()
{
    return tx_table.DCM_EllipseAcceleration_signals.DCM_AccelerationVertical_value;
}

int App_CanTx_DCM_AngularVelocityRoll_Get()
{
    return tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityRoll_value;
}

int App_CanTx_DCM_AngularVelocityPitch_Get()
{
    return tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityPitch_value;
}

int App_CanTx_DCM_AngularVelocityYaw_Get()
{
    return tx_table.DCM_EllipseAngularVelocity_signals.DCM_AngularVelocityYaw_value;
}

float App_CanTx_DCM_EulerAnglesRoll_Get()
{
    return tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesRoll_value;
}

float App_CanTx_DCM_EulerAnglesPitch_Get()
{
    return tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesPitch_value;
}

float App_CanTx_DCM_EulerAnglesYaw_Get()
{
    return tx_table.DCM_EllipseEulerAngles_signals.DCM_EulerAnglesYaw_value;
}

uint32_t App_CanTx_DCM_Hash_Get()
{
    return tx_table.DCM_CommitInfo_signals.DCM_Hash_value;
}

bool App_CanTx_DCM_Clean_Get()
{
    return tx_table.DCM_CommitInfo_signals.DCM_Clean_value;
}

bool App_CanTx_DCM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanTx_DCM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanTx_DCM_Warning_WatchdogTimeout_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_WatchdogTimeout_value;
}

bool App_CanTx_DCM_Warning_TxOverflow_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_TxOverflow_value;
}

bool App_CanTx_DCM_Warning_RxOverflow_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_Warning_RxOverflow_value;
}

bool App_CanTx_DCM_Fault_MissingHeartbeat_Get()
{
    return tx_table.DCM_Faults_signals.DCM_Fault_MissingHeartbeat_value;
}

bool App_CanTx_DCM_Fault_LeftInverterFault_Get()
{
    return tx_table.DCM_Faults_signals.DCM_Fault_LeftInverterFault_value;
}

bool App_CanTx_DCM_Fault_RightInverterFault_Get()
{
    return tx_table.DCM_Faults_signals.DCM_Fault_RightInverterFault_value;
}

uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask1HzCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask1HzCount_value;
}

uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask100HzCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask100HzCount_value;
}

uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTask1kHzCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTask1kHzCount_value;
}

uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanRxCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTaskCanRxCount_value;
}

uint32_t App_CanTx_DCM_Warning_StackWaterMarkHighTaskCanTxCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_StackWaterMarkHighTaskCanTxCount_value;
}

uint32_t App_CanTx_DCM_Warning_WatchdogTimeoutCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_WatchdogTimeoutCount_value;
}

uint32_t App_CanTx_DCM_Warning_TxOverflowCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_TxOverflowCount_value;
}

uint32_t App_CanTx_DCM_Warning_RxOverflowCount_Get()
{
    return tx_table.DCM_WarningsCounts_signals.DCM_Warning_RxOverflowCount_value;
}

uint32_t App_CanTx_DCM_Fault_MissingHeartbeatCount_Get()
{
    return tx_table.DCM_FaultsCounts_signals.DCM_Fault_MissingHeartbeatCount_value;
}

uint32_t App_CanTx_DCM_Fault_LeftInverterFaultCount_Get()
{
    return tx_table.DCM_FaultsCounts_signals.DCM_Fault_LeftInverterFaultCount_value;
}

uint32_t App_CanTx_DCM_Fault_RightInverterFaultCount_Get()
{
    return tx_table.DCM_FaultsCounts_signals.DCM_Fault_RightInverterFaultCount_value;
}

const DCM_Vitals_Signals* App_CanTx_DCM_Vitals_GetData()
{
    return &tx_table.DCM_Vitals_signals;
}

const DCM_LeftInverterCommand_Signals* App_CanTx_DCM_LeftInverterCommand_GetData()
{
    return &tx_table.DCM_LeftInverterCommand_signals;
}

const DCM_RightInverterCommand_Signals* App_CanTx_DCM_RightInverterCommand_GetData()
{
    return &tx_table.DCM_RightInverterCommand_signals;
}

const DCM_AlertsContext_Signals* App_CanTx_DCM_AlertsContext_GetData()
{
    return &tx_table.DCM_AlertsContext_signals;
}

const DCM_EllipseStatus_Signals* App_CanTx_DCM_EllipseStatus_GetData()
{
    return &tx_table.DCM_EllipseStatus_signals;
}

const DCM_EllipseTime_Signals* App_CanTx_DCM_EllipseTime_GetData()
{
    return &tx_table.DCM_EllipseTime_signals;
}

const DCM_EllipseAcceleration_Signals* App_CanTx_DCM_EllipseAcceleration_GetData()
{
    return &tx_table.DCM_EllipseAcceleration_signals;
}

const DCM_EllipseAngularVelocity_Signals* App_CanTx_DCM_EllipseAngularVelocity_GetData()
{
    return &tx_table.DCM_EllipseAngularVelocity_signals;
}

const DCM_EllipseEulerAngles_Signals* App_CanTx_DCM_EllipseEulerAngles_GetData()
{
    return &tx_table.DCM_EllipseEulerAngles_signals;
}

const DCM_CommitInfo_Signals* App_CanTx_DCM_CommitInfo_GetData()
{
    return &tx_table.DCM_CommitInfo_signals;
}

const DCM_Warnings_Signals* App_CanTx_DCM_Warnings_GetData()
{
    return &tx_table.DCM_Warnings_signals;
}

const DCM_Faults_Signals* App_CanTx_DCM_Faults_GetData()
{
    return &tx_table.DCM_Faults_signals;
}

const DCM_WarningsCounts_Signals* App_CanTx_DCM_WarningsCounts_GetData()
{
    return &tx_table.DCM_WarningsCounts_signals;
}

const DCM_FaultsCounts_Signals* App_CanTx_DCM_FaultsCounts_GetData()
{
    return &tx_table.DCM_FaultsCounts_signals;
}

