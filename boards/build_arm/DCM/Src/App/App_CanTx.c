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
    DCM_EllipseImuStatus_Signals DCM_EllipseImuStatus_signals;
    DCM_EllipseImuTime_Signals DCM_EllipseImuTime_signals;
    DCM_EllipseImuAcceleration_Signals DCM_EllipseImuAcceleration_signals;
    DCM_EllipseImuAngularVelocity_Signals DCM_EllipseImuAngularVelocity_signals;
    DCM_EllipseImuEulerAngles_Signals DCM_EllipseImuEulerAngles_signals;
    DCM_Warnings_Signals DCM_Warnings_signals;
    DCM_Faults_Signals DCM_Faults_signals;
} DCM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static DCM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(DCM_TxMsgs));
    App_CanTx_DCM_Vitals_Heartbeat_Set(CANSIG_DCM_VITALS_HEARTBEAT_START_VAL);
    App_CanTx_DCM_Vitals_CurrentState_Set(CANSIG_DCM_VITALS_CURRENT_STATE_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_SpeedCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_DIRECTION_COMMAND_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_INVERTER_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_EnableDischarge_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_DISCHARGE_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_EnableSpeedMode_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_SPEED_MODE_START_VAL);
    App_CanTx_DCM_LeftInverterCommand_TorqueLimit_Set(CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_START_VAL);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterCommand_SpeedCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterCommand_DirectionCommand_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_DIRECTION_COMMAND_START_VAL);
    App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_INVERTER_START_VAL);
    App_CanTx_DCM_RightInverterCommand_EnableDischarge_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_DISCHARGE_START_VAL);
    App_CanTx_DCM_RightInverterCommand_EnableSpeedMode_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_SPEED_MODE_START_VAL);
    App_CanTx_DCM_RightInverterCommand_TorqueLimit_Set(CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_START_VAL);
    App_CanTx_DCM_AlertsContext_TxOverflowCount_Set(CANSIG_DCM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_AlertsContext_RxOverflowCount_Set(CANSIG_DCM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DCM_AlertsContext_WatchdogTimeoutTaskName_Set(CANSIG_DCM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Set(CANSIG_DCM_ELLIPSE_IMU_STATUS_GENERAL_STATUS_BITMASK_START_VAL);
    App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Set(CANSIG_DCM_ELLIPSE_IMU_STATUS_COM_STATUS_BITMASK_START_VAL);
    App_CanTx_DCM_EllipseImuTime_Timestamp_Set(CANSIG_DCM_ELLIPSE_IMU_TIME_TIMESTAMP_START_VAL);
    App_CanTx_DCM_EllipseImuAcceleration_Forward_Set(CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_START_VAL);
    App_CanTx_DCM_EllipseImuAcceleration_Lateral_Set(CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_START_VAL);
    App_CanTx_DCM_EllipseImuAcceleration_Vertical_Set(CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_START_VAL);
    App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Set(CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_START_VAL);
    App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Set(CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_START_VAL);
    App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Set(CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_START_VAL);
    App_CanTx_DCM_EllipseImuEulerAngles_Roll_Set(CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_START_VAL);
    App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Set(CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_START_VAL);
    App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Set(CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Set(CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Set(CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_START_VAL);
    App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Set(CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_START_VAL);
}

void App_CanTx_DCM_Vitals_Heartbeat_Set(bool value)
{
    tx_table.DCM_Vitals_signals.Heartbeat_value = value;
}

void App_CanTx_DCM_Vitals_CurrentState_Set(DcmState value)
{
    tx_table.DCM_Vitals_signals.CurrentState_value = (value > CANSIG_DCM_VITALS_CURRENT_STATE_MAX) ? CANSIG_DCM_VITALS_CURRENT_STATE_MAX : value;
}

void App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.TorqueCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_LeftInverterCommand_SpeedCommand_Set(int value)
{
    const int tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.SpeedCommand_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Set(InverterDirection value)
{
    tx_table.DCM_LeftInverterCommand_signals.DirectionCommand_value = (value > CANSIG_DCM_LEFT_INVERTER_COMMAND_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_LEFT_INVERTER_COMMAND_DIRECTION_COMMAND_MAX : value;
}

void App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.EnableInverter_value = value;
}

void App_CanTx_DCM_LeftInverterCommand_EnableDischarge_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.EnableDischarge_value = value;
}

void App_CanTx_DCM_LeftInverterCommand_EnableSpeedMode_Set(bool value)
{
    tx_table.DCM_LeftInverterCommand_signals.EnableSpeedMode_value = value;
}

void App_CanTx_DCM_LeftInverterCommand_TorqueLimit_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_MIN ? CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_MIN : value;
    tx_table.DCM_LeftInverterCommand_signals.TorqueLimit_value = tmp > CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_MAX ? CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_MAX : tmp;
}

void App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.TorqueCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_RightInverterCommand_SpeedCommand_Set(int value)
{
    const int tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.SpeedCommand_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_MAX : tmp;
}

void App_CanTx_DCM_RightInverterCommand_DirectionCommand_Set(InverterDirection value)
{
    tx_table.DCM_RightInverterCommand_signals.DirectionCommand_value = (value > CANSIG_DCM_RIGHT_INVERTER_COMMAND_DIRECTION_COMMAND_MAX) ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_DIRECTION_COMMAND_MAX : value;
}

void App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.EnableInverter_value = value;
}

void App_CanTx_DCM_RightInverterCommand_EnableDischarge_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.EnableDischarge_value = value;
}

void App_CanTx_DCM_RightInverterCommand_EnableSpeedMode_Set(bool value)
{
    tx_table.DCM_RightInverterCommand_signals.EnableSpeedMode_value = value;
}

void App_CanTx_DCM_RightInverterCommand_TorqueLimit_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_MIN ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_MIN : value;
    tx_table.DCM_RightInverterCommand_signals.TorqueLimit_value = tmp > CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_MAX ? CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_MAX : tmp;
}

void App_CanTx_DCM_AlertsContext_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_AlertsContext_signals.TxOverflowCount_value = (value > CANSIG_DCM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_AlertsContext_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DCM_AlertsContext_signals.RxOverflowCount_value = (value > CANSIG_DCM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DCM_AlertsContext_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.DCM_AlertsContext_signals.WatchdogTimeoutTaskName_value = (value > CANSIG_DCM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_DCM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Set(uint32_t value)
{
    tx_table.DCM_EllipseImuStatus_signals.GeneralStatusBitmask_value = (value > CANSIG_DCM_ELLIPSE_IMU_STATUS_GENERAL_STATUS_BITMASK_MAX) ? CANSIG_DCM_ELLIPSE_IMU_STATUS_GENERAL_STATUS_BITMASK_MAX : value;
}

void App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Set(uint32_t value)
{
    tx_table.DCM_EllipseImuStatus_signals.ComStatusBitmask_value = value;
}

void App_CanTx_DCM_EllipseImuTime_Timestamp_Set(uint32_t value)
{
    tx_table.DCM_EllipseImuTime_signals.Timestamp_value = value;
}

void App_CanTx_DCM_EllipseImuAcceleration_Forward_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_MIN ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_MIN : value;
    tx_table.DCM_EllipseImuAcceleration_signals.Forward_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_MAX ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuAcceleration_Lateral_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_MIN ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_MIN : value;
    tx_table.DCM_EllipseImuAcceleration_signals.Lateral_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_MAX ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuAcceleration_Vertical_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_MIN ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_MIN : value;
    tx_table.DCM_EllipseImuAcceleration_signals.Vertical_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_MAX ? CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_MIN ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_MIN : value;
    tx_table.DCM_EllipseImuAngularVelocity_signals.Roll_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_MAX ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_MIN ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_MIN : value;
    tx_table.DCM_EllipseImuAngularVelocity_signals.Pitch_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_MAX ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Set(int value)
{
    const int tmp = value < CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_MIN ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_MIN : value;
    tx_table.DCM_EllipseImuAngularVelocity_signals.Yaw_value = tmp > CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_MAX ? CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuEulerAngles_Roll_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_MIN ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_MIN : value;
    tx_table.DCM_EllipseImuEulerAngles_signals.Roll_value = tmp > CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_MAX ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_MIN ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_MIN : value;
    tx_table.DCM_EllipseImuEulerAngles_signals.Pitch_value = tmp > CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_MAX ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_MAX : tmp;
}

void App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_MIN ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_MIN : value;
    tx_table.DCM_EllipseImuEulerAngles_signals.Yaw_value = tmp > CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_MAX ? CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_MAX : tmp;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Set(bool value)
{
    tx_table.DCM_Warnings_signals.DCM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Set(bool value)
{
    tx_table.DCM_Faults_signals.DCM_FAULT_STATE_FAULT_value = value;
}

void App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Set(bool value)
{
    tx_table.DCM_Faults_signals.DCM_FAULT_MISSING_HEARTBEAT_value = value;
}

bool App_CanTx_DCM_Vitals_Heartbeat_Get()
{
    return tx_table.DCM_Vitals_signals.Heartbeat_value;
}

DcmState App_CanTx_DCM_Vitals_CurrentState_Get()
{
    return tx_table.DCM_Vitals_signals.CurrentState_value;
}

float App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.TorqueCommand_value;
}

int App_CanTx_DCM_LeftInverterCommand_SpeedCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.SpeedCommand_value;
}

InverterDirection App_CanTx_DCM_LeftInverterCommand_DirectionCommand_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.DirectionCommand_value;
}

bool App_CanTx_DCM_LeftInverterCommand_EnableInverter_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.EnableInverter_value;
}

bool App_CanTx_DCM_LeftInverterCommand_EnableDischarge_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.EnableDischarge_value;
}

bool App_CanTx_DCM_LeftInverterCommand_EnableSpeedMode_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.EnableSpeedMode_value;
}

float App_CanTx_DCM_LeftInverterCommand_TorqueLimit_Get()
{
    return tx_table.DCM_LeftInverterCommand_signals.TorqueLimit_value;
}

float App_CanTx_DCM_RightInverterCommand_TorqueCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.TorqueCommand_value;
}

int App_CanTx_DCM_RightInverterCommand_SpeedCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.SpeedCommand_value;
}

InverterDirection App_CanTx_DCM_RightInverterCommand_DirectionCommand_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.DirectionCommand_value;
}

bool App_CanTx_DCM_RightInverterCommand_EnableInverter_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.EnableInverter_value;
}

bool App_CanTx_DCM_RightInverterCommand_EnableDischarge_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.EnableDischarge_value;
}

bool App_CanTx_DCM_RightInverterCommand_EnableSpeedMode_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.EnableSpeedMode_value;
}

float App_CanTx_DCM_RightInverterCommand_TorqueLimit_Get()
{
    return tx_table.DCM_RightInverterCommand_signals.TorqueLimit_value;
}

uint32_t App_CanTx_DCM_AlertsContext_TxOverflowCount_Get()
{
    return tx_table.DCM_AlertsContext_signals.TxOverflowCount_value;
}

uint32_t App_CanTx_DCM_AlertsContext_RxOverflowCount_Get()
{
    return tx_table.DCM_AlertsContext_signals.RxOverflowCount_value;
}

RtosTaskName App_CanTx_DCM_AlertsContext_WatchdogTimeoutTaskName_Get()
{
    return tx_table.DCM_AlertsContext_signals.WatchdogTimeoutTaskName_value;
}

uint32_t App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Get()
{
    return tx_table.DCM_EllipseImuStatus_signals.GeneralStatusBitmask_value;
}

uint32_t App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Get()
{
    return tx_table.DCM_EllipseImuStatus_signals.ComStatusBitmask_value;
}

uint32_t App_CanTx_DCM_EllipseImuTime_Timestamp_Get()
{
    return tx_table.DCM_EllipseImuTime_signals.Timestamp_value;
}

float App_CanTx_DCM_EllipseImuAcceleration_Forward_Get()
{
    return tx_table.DCM_EllipseImuAcceleration_signals.Forward_value;
}

float App_CanTx_DCM_EllipseImuAcceleration_Lateral_Get()
{
    return tx_table.DCM_EllipseImuAcceleration_signals.Lateral_value;
}

float App_CanTx_DCM_EllipseImuAcceleration_Vertical_Get()
{
    return tx_table.DCM_EllipseImuAcceleration_signals.Vertical_value;
}

int App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Get()
{
    return tx_table.DCM_EllipseImuAngularVelocity_signals.Roll_value;
}

int App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Get()
{
    return tx_table.DCM_EllipseImuAngularVelocity_signals.Pitch_value;
}

int App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Get()
{
    return tx_table.DCM_EllipseImuAngularVelocity_signals.Yaw_value;
}

float App_CanTx_DCM_EllipseImuEulerAngles_Roll_Get()
{
    return tx_table.DCM_EllipseImuEulerAngles_signals.Roll_value;
}

float App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Get()
{
    return tx_table.DCM_EllipseImuEulerAngles_signals.Pitch_value;
}

float App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Get()
{
    return tx_table.DCM_EllipseImuEulerAngles_signals.Yaw_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_TX_OVERFLOW_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_TX_OVERFLOW_value;
}

bool App_CanTx_DCM_Warnings_DCM_WARNING_RX_OVERFLOW_Get()
{
    return tx_table.DCM_Warnings_signals.DCM_WARNING_RX_OVERFLOW_value;
}

bool App_CanTx_DCM_Faults_DCM_FAULT_STATE_FAULT_Get()
{
    return tx_table.DCM_Faults_signals.DCM_FAULT_STATE_FAULT_value;
}

bool App_CanTx_DCM_Faults_DCM_FAULT_MISSING_HEARTBEAT_Get()
{
    return tx_table.DCM_Faults_signals.DCM_FAULT_MISSING_HEARTBEAT_value;
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

const DCM_EllipseImuStatus_Signals* App_CanTx_DCM_EllipseImuStatus_GetData()
{
    return &tx_table.DCM_EllipseImuStatus_signals;
}

const DCM_EllipseImuTime_Signals* App_CanTx_DCM_EllipseImuTime_GetData()
{
    return &tx_table.DCM_EllipseImuTime_signals;
}

const DCM_EllipseImuAcceleration_Signals* App_CanTx_DCM_EllipseImuAcceleration_GetData()
{
    return &tx_table.DCM_EllipseImuAcceleration_signals;
}

const DCM_EllipseImuAngularVelocity_Signals* App_CanTx_DCM_EllipseImuAngularVelocity_GetData()
{
    return &tx_table.DCM_EllipseImuAngularVelocity_signals;
}

const DCM_EllipseImuEulerAngles_Signals* App_CanTx_DCM_EllipseImuEulerAngles_GetData()
{
    return &tx_table.DCM_EllipseImuEulerAngles_signals;
}

const DCM_Warnings_Signals* App_CanTx_DCM_Warnings_GetData()
{
    return &tx_table.DCM_Warnings_signals;
}

const DCM_Faults_Signals* App_CanTx_DCM_Faults_GetData()
{
    return &tx_table.DCM_Faults_signals;
}

