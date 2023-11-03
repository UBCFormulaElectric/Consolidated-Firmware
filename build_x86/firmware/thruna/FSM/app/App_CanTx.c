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
 * Struct for holding all messages transmitted by FSM (i.e. the TX table).
 */
typedef struct
{
    FSM_Vitals_Signals FSM_Vitals_signals;
    FSM_AlertsContext_Signals FSM_AlertsContext_signals;
    FSM_Apps_Signals FSM_Apps_signals;
    FSM_Brake_Signals FSM_Brake_signals;
    FSM_Coolant_Signals FSM_Coolant_signals;
    FSM_Steering_Signals FSM_Steering_signals;
    FSM_Wheels_Signals FSM_Wheels_signals;
    FSM_CommitInfo_Signals FSM_CommitInfo_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    FSM_WarningsCounts_Signals FSM_WarningsCounts_signals;
    FSM_FaultsCounts_Signals FSM_FaultsCounts_signals;
} FSM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static FSM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(FSM_TxMsgs));
    App_CanTx_FSM_Heartbeat_Set(CANSIG_FSM_VITALS_FSM_HEARTBEAT_START_VAL);
    App_CanTx_FSM_State_Set(CANSIG_FSM_VITALS_FSM_STATE_START_VAL);
    App_CanTx_FSM_WatchdogTimeoutTaskName_Set(CANSIG_FSM_ALERTS_CONTEXT_FSM_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_FSM_TxOverflowCount_Set(CANSIG_FSM_ALERTS_CONTEXT_FSM_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_FSM_RxOverflowCount_Set(CANSIG_FSM_ALERTS_CONTEXT_FSM_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_FSM_PappsMappedPedalPercentage_Set(CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanTx_FSM_SappsMappedPedalPercentage_Set(CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanTx_FSM_PappsRawPedalPercentage_Set(CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_START_VAL);
    App_CanTx_FSM_SappsRawPedalPercentage_Set(CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_START_VAL);
    App_CanTx_FSM_TorqueLimit_Set(CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_START_VAL);
    App_CanTx_FSM_TorquePlausabilityFailed_Set(CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanTx_FSM_FrontBrakePressure_Set(CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_START_VAL);
    App_CanTx_FSM_RearBrakePressure_Set(CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_START_VAL);
    App_CanTx_FSM_BrakePedalPercentage_Set(CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_START_VAL);
    App_CanTx_FSM_BrakeActuated_Set(CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_START_VAL);
    App_CanTx_FSM_BrakePressureSensorOCSC_Set(CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_START_VAL);
    App_CanTx_FSM_BrakePedalSensorOCSC_Set(CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_START_VAL);
    App_CanTx_FSM_CoolantFlowRate_Set(CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_START_VAL);
    App_CanTx_FSM_CoolantTemperatureA_Set(CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_START_VAL);
    App_CanTx_FSM_CoolantTemperatureB_Set(CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_START_VAL);
    App_CanTx_FSM_CoolantPressureA_Set(CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_START_VAL);
    App_CanTx_FSM_CoolantPressureB_Set(CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_START_VAL);
    App_CanTx_FSM_SteeringAngle_Set(CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_START_VAL);
    App_CanTx_FSM_LeftWheelSpeed_Set(CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_START_VAL);
    App_CanTx_FSM_RightWheelSpeed_Set(CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_START_VAL);
    App_CanTx_FSM_Hash_Set(CANSIG_FSM_COMMIT_INFO_FSM_HASH_START_VAL);
    App_CanTx_FSM_Clean_Set(CANSIG_FSM_COMMIT_INFO_FSM_CLEAN_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask1Hz_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask100Hz_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHz_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRx_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTx_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanTx_FSM_Warning_WatchdogTimeout_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_FSM_Warning_TxOverflow_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_FSM_Warning_RxOverflow_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanTx_FSM_Warning_SteeringAngleOCSC_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_START_VAL);
    App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_Warning_FlowRateOutOfRange_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanTx_FSM_Warning_BrakeAppsDisagreement_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_START_VAL);
    App_CanTx_FSM_Warning_AppsDisagreement_Set(CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_START_VAL);
    App_CanTx_FSM_Fault_MissingHeartbeat_Set(CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanTx_FSM_Fault_StateMachine_Set(CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_START_VAL);
    App_CanTx_FSM_Fault_PappsOCSC_Set(CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_START_VAL);
    App_CanTx_FSM_Fault_SappsOCSCFault_Set(CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_START_VAL);
    App_CanTx_FSM_Fault_FlowMeterUnderflow_Set(CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_START_VAL);
    App_CanTx_FSM_Fault_TorquePlausabilityFailed_Set(CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask1HzCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask100HzCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHzCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    App_CanTx_FSM_Warning_WatchdogTimeoutCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    App_CanTx_FSM_Warning_TxOverflowCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_FSM_Warning_RxOverflowCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_START_VAL);
    App_CanTx_FSM_Warning_RightWheelSpeedOutOfRangeCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_START_VAL);
    App_CanTx_FSM_Warning_SteeringAngleOCSCCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_START_VAL);
    App_CanTx_FSM_Warning_SteeringAngleOutOfRangeCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_START_VAL);
    App_CanTx_FSM_Warning_FrontBrakePressureOutOfRangeCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_START_VAL);
    App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_Warning_FlowRateOutOfRangeCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_COUNT_START_VAL);
    App_CanTx_FSM_Warning_BrakeAppsDisagreementCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_START_VAL);
    App_CanTx_FSM_Warning_AppsDisagreementCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_APPS_DISAGREEMENT_COUNT_START_VAL);
    App_CanTx_FSM_Fault_MissingHeartbeatCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_MISSING_HEARTBEAT_COUNT_START_VAL);
    App_CanTx_FSM_Fault_StateMachineCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_STATE_MACHINE_COUNT_START_VAL);
    App_CanTx_FSM_Fault_PappsOCSCCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_PAPPS_OCSC_COUNT_START_VAL);
    App_CanTx_FSM_Fault_SappsOCSCFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_SAPPS_OCSC_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_Fault_FlowMeterUnderflowCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_FLOW_METER_UNDERFLOW_COUNT_START_VAL);
    App_CanTx_FSM_Fault_TorquePlausabilityFailedCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_COUNT_START_VAL);
}

void App_CanTx_FSM_Heartbeat_Set(bool value)
{
    tx_table.FSM_Vitals_signals.FSM_Heartbeat_value = value;
}

void App_CanTx_FSM_State_Set(State_Machine value)
{
    tx_table.FSM_Vitals_signals.FSM_State_value = (value > CANSIG_FSM_VITALS_FSM_STATE_MAX) ? CANSIG_FSM_VITALS_FSM_STATE_MAX : value;
}

void App_CanTx_FSM_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.FSM_AlertsContext_signals.FSM_WatchdogTimeoutTaskName_value = (value > CANSIG_FSM_ALERTS_CONTEXT_FSM_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_FSM_ALERTS_CONTEXT_FSM_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_FSM_TxOverflowCount_Set(uint32_t value)
{
    tx_table.FSM_AlertsContext_signals.FSM_TxOverflowCount_value = (value > CANSIG_FSM_ALERTS_CONTEXT_FSM_TX_OVERFLOW_COUNT_MAX) ? CANSIG_FSM_ALERTS_CONTEXT_FSM_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_FSM_RxOverflowCount_Set(uint32_t value)
{
    tx_table.FSM_AlertsContext_signals.FSM_RxOverflowCount_value = (value > CANSIG_FSM_ALERTS_CONTEXT_FSM_RX_OVERFLOW_COUNT_MAX) ? CANSIG_FSM_ALERTS_CONTEXT_FSM_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_FSM_PappsMappedPedalPercentage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    tx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanTx_FSM_SappsMappedPedalPercentage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    tx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanTx_FSM_PappsRawPedalPercentage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MIN : value;
    tx_table.FSM_Apps_signals.FSM_PappsRawPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanTx_FSM_SappsRawPedalPercentage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MIN : value;
    tx_table.FSM_Apps_signals.FSM_SappsRawPedalPercentage_value = tmp > CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanTx_FSM_TorqueLimit_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MIN ? CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MIN : value;
    tx_table.FSM_Apps_signals.FSM_TorqueLimit_value = tmp > CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MAX ? CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_MAX : tmp;
}

void App_CanTx_FSM_TorquePlausabilityFailed_Set(bool value)
{
    tx_table.FSM_Apps_signals.FSM_TorquePlausabilityFailed_value = value;
}

void App_CanTx_FSM_FrontBrakePressure_Set(uint32_t value)
{
    tx_table.FSM_Brake_signals.FSM_FrontBrakePressure_value = (value > CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_MAX : value;
}

void App_CanTx_FSM_RearBrakePressure_Set(uint32_t value)
{
    tx_table.FSM_Brake_signals.FSM_RearBrakePressure_value = (value > CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_MAX) ? CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_MAX : value;
}

void App_CanTx_FSM_BrakePedalPercentage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MIN : value;
    tx_table.FSM_Brake_signals.FSM_BrakePedalPercentage_value = tmp > CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanTx_FSM_BrakeActuated_Set(bool value)
{
    tx_table.FSM_Brake_signals.FSM_BrakeActuated_value = value;
}

void App_CanTx_FSM_BrakePressureSensorOCSC_Set(bool value)
{
    tx_table.FSM_Brake_signals.FSM_BrakePressureSensorOCSC_value = value;
}

void App_CanTx_FSM_BrakePedalSensorOCSC_Set(bool value)
{
    tx_table.FSM_Brake_signals.FSM_BrakePedalSensorOCSC_value = value;
}

void App_CanTx_FSM_CoolantFlowRate_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_MIN ? CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_MIN : value;
    tx_table.FSM_Coolant_signals.FSM_CoolantFlowRate_value = tmp > CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_MAX ? CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_MAX : tmp;
}

void App_CanTx_FSM_CoolantTemperatureA_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_MIN ? CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_MIN : value;
    tx_table.FSM_Coolant_signals.FSM_CoolantTemperatureA_value = tmp > CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_MAX ? CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_MAX : tmp;
}

void App_CanTx_FSM_CoolantTemperatureB_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_MIN ? CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_MIN : value;
    tx_table.FSM_Coolant_signals.FSM_CoolantTemperatureB_value = tmp > CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_MAX ? CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_MAX : tmp;
}

void App_CanTx_FSM_CoolantPressureA_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_MIN ? CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_MIN : value;
    tx_table.FSM_Coolant_signals.FSM_CoolantPressureA_value = tmp > CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_MAX ? CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_MAX : tmp;
}

void App_CanTx_FSM_CoolantPressureB_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_MIN ? CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_MIN : value;
    tx_table.FSM_Coolant_signals.FSM_CoolantPressureB_value = tmp > CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_MAX ? CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_MAX : tmp;
}

void App_CanTx_FSM_SteeringAngle_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_MIN ? CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_MIN : value;
    tx_table.FSM_Steering_signals.FSM_SteeringAngle_value = tmp > CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_MAX ? CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_MAX : tmp;
}

void App_CanTx_FSM_LeftWheelSpeed_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_MIN ? CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_MIN : value;
    tx_table.FSM_Wheels_signals.FSM_LeftWheelSpeed_value = tmp > CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_MAX ? CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_MAX : tmp;
}

void App_CanTx_FSM_RightWheelSpeed_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_MIN ? CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_MIN : value;
    tx_table.FSM_Wheels_signals.FSM_RightWheelSpeed_value = tmp > CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_MAX ? CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_MAX : tmp;
}

void App_CanTx_FSM_Hash_Set(uint32_t value)
{
    tx_table.FSM_CommitInfo_signals.FSM_Hash_value = value;
}

void App_CanTx_FSM_Clean_Set(bool value)
{
    tx_table.FSM_CommitInfo_signals.FSM_Clean_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask1Hz_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask100Hz_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHz_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanTx_FSM_Warning_WatchdogTimeout_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value = value;
}

void App_CanTx_FSM_Warning_TxOverflow_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value = value;
}

void App_CanTx_FSM_Warning_RxOverflow_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value = value;
}

void App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value = value;
}

void App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value = value;
}

void App_CanTx_FSM_Warning_SteeringAngleOCSC_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value = value;
}

void App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value = value;
}

void App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value = value;
}

void App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_Warning_FlowRateOutOfRange_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value = value;
}

void App_CanTx_FSM_Warning_BrakeAppsDisagreement_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value = value;
}

void App_CanTx_FSM_Warning_AppsDisagreement_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value = value;
}

void App_CanTx_FSM_Fault_MissingHeartbeat_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value = value;
}

void App_CanTx_FSM_Fault_StateMachine_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value = value;
}

void App_CanTx_FSM_Fault_PappsOCSC_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value = value;
}

void App_CanTx_FSM_Fault_SappsOCSCFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value = value;
}

void App_CanTx_FSM_Fault_FlowMeterUnderflow_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value = value;
}

void App_CanTx_FSM_Fault_TorquePlausabilityFailed_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value = value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1HzCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask100HzCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1kHzCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanRxCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanTxCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_WatchdogTimeoutCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_WatchdogTimeoutCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_TxOverflowCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_TxOverflowCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_TX_OVERFLOW_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_RxOverflowCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_RxOverflowCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RX_OVERFLOW_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_LeftWheelSpeedOutOfRangeCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_RightWheelSpeedOutOfRangeCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_RightWheelSpeedOutOfRangeCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_SteeringAngleOCSCCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOCSCCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_SteeringAngleOutOfRangeCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOutOfRangeCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_FrontBrakePressureOutOfRangeCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOutOfRangeCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_FlowRateOutOfRangeCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_FlowRateOutOfRangeCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_BrakeAppsDisagreementCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_BrakeAppsDisagreementCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_MAX : value;
}

void App_CanTx_FSM_Warning_AppsDisagreementCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_Warning_AppsDisagreementCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_APPS_DISAGREEMENT_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_MissingHeartbeatCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_MissingHeartbeatCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_MISSING_HEARTBEAT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_MISSING_HEARTBEAT_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_StateMachineCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_StateMachineCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_STATE_MACHINE_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_STATE_MACHINE_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_PappsOCSCCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_PappsOCSCCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_PAPPS_OCSC_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_PAPPS_OCSC_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_SappsOCSCFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_SappsOCSCFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_SAPPS_OCSC_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_SAPPS_OCSC_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_FlowMeterUnderflowCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_FlowMeterUnderflowCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_FLOW_METER_UNDERFLOW_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_FLOW_METER_UNDERFLOW_COUNT_MAX : value;
}

void App_CanTx_FSM_Fault_TorquePlausabilityFailedCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_Fault_TorquePlausabilityFailedCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_COUNT_MAX : value;
}

bool App_CanTx_FSM_Heartbeat_Get()
{
    return tx_table.FSM_Vitals_signals.FSM_Heartbeat_value;
}

State_Machine App_CanTx_FSM_State_Get()
{
    return tx_table.FSM_Vitals_signals.FSM_State_value;
}

RtosTaskName App_CanTx_FSM_WatchdogTimeoutTaskName_Get()
{
    return tx_table.FSM_AlertsContext_signals.FSM_WatchdogTimeoutTaskName_value;
}

uint32_t App_CanTx_FSM_TxOverflowCount_Get()
{
    return tx_table.FSM_AlertsContext_signals.FSM_TxOverflowCount_value;
}

uint32_t App_CanTx_FSM_RxOverflowCount_Get()
{
    return tx_table.FSM_AlertsContext_signals.FSM_RxOverflowCount_value;
}

float App_CanTx_FSM_PappsMappedPedalPercentage_Get()
{
    return tx_table.FSM_Apps_signals.FSM_PappsMappedPedalPercentage_value;
}

float App_CanTx_FSM_SappsMappedPedalPercentage_Get()
{
    return tx_table.FSM_Apps_signals.FSM_SappsMappedPedalPercentage_value;
}

float App_CanTx_FSM_PappsRawPedalPercentage_Get()
{
    return tx_table.FSM_Apps_signals.FSM_PappsRawPedalPercentage_value;
}

float App_CanTx_FSM_SappsRawPedalPercentage_Get()
{
    return tx_table.FSM_Apps_signals.FSM_SappsRawPedalPercentage_value;
}

float App_CanTx_FSM_TorqueLimit_Get()
{
    return tx_table.FSM_Apps_signals.FSM_TorqueLimit_value;
}

bool App_CanTx_FSM_TorquePlausabilityFailed_Get()
{
    return tx_table.FSM_Apps_signals.FSM_TorquePlausabilityFailed_value;
}

uint32_t App_CanTx_FSM_FrontBrakePressure_Get()
{
    return tx_table.FSM_Brake_signals.FSM_FrontBrakePressure_value;
}

uint32_t App_CanTx_FSM_RearBrakePressure_Get()
{
    return tx_table.FSM_Brake_signals.FSM_RearBrakePressure_value;
}

float App_CanTx_FSM_BrakePedalPercentage_Get()
{
    return tx_table.FSM_Brake_signals.FSM_BrakePedalPercentage_value;
}

bool App_CanTx_FSM_BrakeActuated_Get()
{
    return tx_table.FSM_Brake_signals.FSM_BrakeActuated_value;
}

bool App_CanTx_FSM_BrakePressureSensorOCSC_Get()
{
    return tx_table.FSM_Brake_signals.FSM_BrakePressureSensorOCSC_value;
}

bool App_CanTx_FSM_BrakePedalSensorOCSC_Get()
{
    return tx_table.FSM_Brake_signals.FSM_BrakePedalSensorOCSC_value;
}

float App_CanTx_FSM_CoolantFlowRate_Get()
{
    return tx_table.FSM_Coolant_signals.FSM_CoolantFlowRate_value;
}

float App_CanTx_FSM_CoolantTemperatureA_Get()
{
    return tx_table.FSM_Coolant_signals.FSM_CoolantTemperatureA_value;
}

float App_CanTx_FSM_CoolantTemperatureB_Get()
{
    return tx_table.FSM_Coolant_signals.FSM_CoolantTemperatureB_value;
}

float App_CanTx_FSM_CoolantPressureA_Get()
{
    return tx_table.FSM_Coolant_signals.FSM_CoolantPressureA_value;
}

float App_CanTx_FSM_CoolantPressureB_Get()
{
    return tx_table.FSM_Coolant_signals.FSM_CoolantPressureB_value;
}

float App_CanTx_FSM_SteeringAngle_Get()
{
    return tx_table.FSM_Steering_signals.FSM_SteeringAngle_value;
}

float App_CanTx_FSM_LeftWheelSpeed_Get()
{
    return tx_table.FSM_Wheels_signals.FSM_LeftWheelSpeed_value;
}

float App_CanTx_FSM_RightWheelSpeed_Get()
{
    return tx_table.FSM_Wheels_signals.FSM_RightWheelSpeed_value;
}

uint32_t App_CanTx_FSM_Hash_Get()
{
    return tx_table.FSM_CommitInfo_signals.FSM_Hash_value;
}

bool App_CanTx_FSM_Clean_Get()
{
    return tx_table.FSM_CommitInfo_signals.FSM_Clean_value;
}

bool App_CanTx_FSM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanTx_FSM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanTx_FSM_Warning_WatchdogTimeout_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_WatchdogTimeout_value;
}

bool App_CanTx_FSM_Warning_TxOverflow_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_TxOverflow_value;
}

bool App_CanTx_FSM_Warning_RxOverflow_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_RxOverflow_value;
}

bool App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRange_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_LeftWheelSpeedOutOfRange_value;
}

bool App_CanTx_FSM_Warning_RightWheelSpeedOutOfRange_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_RightWheelSpeedOutOfRange_value;
}

bool App_CanTx_FSM_Warning_SteeringAngleOCSC_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOCSC_value;
}

bool App_CanTx_FSM_Warning_SteeringAngleOutOfRange_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_SteeringAngleOutOfRange_value;
}

bool App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_FrontBrakePressureOutOfRange_value;
}

bool App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanTx_FSM_Warning_FlowRateOutOfRange_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_FlowRateOutOfRange_value;
}

bool App_CanTx_FSM_Warning_BrakeAppsDisagreement_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_BrakeAppsDisagreement_value;
}

bool App_CanTx_FSM_Warning_AppsDisagreement_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_Warning_AppsDisagreement_value;
}

bool App_CanTx_FSM_Fault_MissingHeartbeat_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_MissingHeartbeat_value;
}

bool App_CanTx_FSM_Fault_StateMachine_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_StateMachine_value;
}

bool App_CanTx_FSM_Fault_PappsOCSC_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_PappsOCSC_value;
}

bool App_CanTx_FSM_Fault_SappsOCSCFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_SappsOCSCFault_value;
}

bool App_CanTx_FSM_Fault_FlowMeterUnderflow_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_FlowMeterUnderflow_value;
}

bool App_CanTx_FSM_Fault_TorquePlausabilityFailed_Get()
{
    return tx_table.FSM_Faults_signals.FSM_Fault_TorquePlausabilityFailed_value;
}

uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask1HzCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1HzCount_value;
}

uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask100HzCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask100HzCount_value;
}

uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTask1kHzCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTask1kHzCount_value;
}

uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanRxCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanRxCount_value;
}

uint32_t App_CanTx_FSM_Warning_StackWaterMarkHighTaskCanTxCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_StackWaterMarkHighTaskCanTxCount_value;
}

uint32_t App_CanTx_FSM_Warning_WatchdogTimeoutCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_WatchdogTimeoutCount_value;
}

uint32_t App_CanTx_FSM_Warning_TxOverflowCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_TxOverflowCount_value;
}

uint32_t App_CanTx_FSM_Warning_RxOverflowCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_RxOverflowCount_value;
}

uint32_t App_CanTx_FSM_Warning_LeftWheelSpeedOutOfRangeCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_LeftWheelSpeedOutOfRangeCount_value;
}

uint32_t App_CanTx_FSM_Warning_RightWheelSpeedOutOfRangeCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_RightWheelSpeedOutOfRangeCount_value;
}

uint32_t App_CanTx_FSM_Warning_SteeringAngleOCSCCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOCSCCount_value;
}

uint32_t App_CanTx_FSM_Warning_SteeringAngleOutOfRangeCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_SteeringAngleOutOfRangeCount_value;
}

uint32_t App_CanTx_FSM_Warning_FrontBrakePressureOutOfRangeCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_FrontBrakePressureOutOfRangeCount_value;
}

uint32_t App_CanTx_FSM_Warning_RearBrakePressureOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_RearBrakePressureOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_Warning_FlowRateOutOfRangeCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_FlowRateOutOfRangeCount_value;
}

uint32_t App_CanTx_FSM_Warning_BrakeAppsDisagreementCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_BrakeAppsDisagreementCount_value;
}

uint32_t App_CanTx_FSM_Warning_AppsDisagreementCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_Warning_AppsDisagreementCount_value;
}

uint32_t App_CanTx_FSM_Fault_MissingHeartbeatCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_MissingHeartbeatCount_value;
}

uint32_t App_CanTx_FSM_Fault_StateMachineCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_StateMachineCount_value;
}

uint32_t App_CanTx_FSM_Fault_PappsOCSCCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_PappsOCSCCount_value;
}

uint32_t App_CanTx_FSM_Fault_SappsOCSCFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_SappsOCSCFaultCount_value;
}

uint32_t App_CanTx_FSM_Fault_FlowMeterUnderflowCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_FlowMeterUnderflowCount_value;
}

uint32_t App_CanTx_FSM_Fault_TorquePlausabilityFailedCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_Fault_TorquePlausabilityFailedCount_value;
}

const FSM_Vitals_Signals* App_CanTx_FSM_Vitals_GetData()
{
    return &tx_table.FSM_Vitals_signals;
}

const FSM_AlertsContext_Signals* App_CanTx_FSM_AlertsContext_GetData()
{
    return &tx_table.FSM_AlertsContext_signals;
}

const FSM_Apps_Signals* App_CanTx_FSM_Apps_GetData()
{
    return &tx_table.FSM_Apps_signals;
}

const FSM_Brake_Signals* App_CanTx_FSM_Brake_GetData()
{
    return &tx_table.FSM_Brake_signals;
}

const FSM_Coolant_Signals* App_CanTx_FSM_Coolant_GetData()
{
    return &tx_table.FSM_Coolant_signals;
}

const FSM_Steering_Signals* App_CanTx_FSM_Steering_GetData()
{
    return &tx_table.FSM_Steering_signals;
}

const FSM_Wheels_Signals* App_CanTx_FSM_Wheels_GetData()
{
    return &tx_table.FSM_Wheels_signals;
}

const FSM_CommitInfo_Signals* App_CanTx_FSM_CommitInfo_GetData()
{
    return &tx_table.FSM_CommitInfo_signals;
}

const FSM_Warnings_Signals* App_CanTx_FSM_Warnings_GetData()
{
    return &tx_table.FSM_Warnings_signals;
}

const FSM_Faults_Signals* App_CanTx_FSM_Faults_GetData()
{
    return &tx_table.FSM_Faults_signals;
}

const FSM_WarningsCounts_Signals* App_CanTx_FSM_WarningsCounts_GetData()
{
    return &tx_table.FSM_WarningsCounts_signals;
}

const FSM_FaultsCounts_Signals* App_CanTx_FSM_FaultsCounts_GetData()
{
    return &tx_table.FSM_FaultsCounts_signals;
}

