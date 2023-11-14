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
    FSM_StackRemaining_Signals FSM_StackRemaining_signals;
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
    App_CanTx_FSM_StackRemainingTask1Hz_Set(CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1_HZ_START_VAL);
    App_CanTx_FSM_StackRemainingTask100Hz_Set(CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK100_HZ_START_VAL);
    App_CanTx_FSM_StackRemainingTask1kHz_Set(CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1K_HZ_START_VAL);
    App_CanTx_FSM_StackRemainingTaskCanRx_Set(CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_RX_START_VAL);
    App_CanTx_FSM_StackRemainingTaskCanTx_Set(CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_TX_START_VAL);
    App_CanTx_FSM_Hash_Set(CANSIG_FSM_COMMIT_INFO_FSM_HASH_START_VAL);
    App_CanTx_FSM_Clean_Set(CANSIG_FSM_COMMIT_INFO_FSM_CLEAN_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Set(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Set(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Set(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Set(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Set(CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanTx_FSM_WatchdogTimeoutWarning_Set(CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanTx_FSM_TxOverflowWarning_Set(CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_FSM_RxOverflowWarning_Set(CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_SteeringAngleOCSCWarning_Set(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_START_VAL);
    App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_FlowRateOutOfRangeWarning_Set(CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_START_VAL);
    App_CanTx_FSM_BrakeAppsDisagreementWarning_Set(CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanTx_FSM_AppsDisagreementWarning_Set(CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_START_VAL);
    App_CanTx_FSM_MissingHeartbeatFault_Set(CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanTx_FSM_StateMachineFault_Set(CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_START_VAL);
    App_CanTx_FSM_PappsOCSCFault_Set(CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_START_VAL);
    App_CanTx_FSM_SappsOCSCFault_Set(CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_START_VAL);
    App_CanTx_FSM_FlowMeterUnderflowFault_Set(CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_START_VAL);
    App_CanTx_FSM_TorquePlausabilityFailedFault_Set(CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_WatchdogTimeoutWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_WATCHDOG_TIMEOUT_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_TxOverflowWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_TX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_RxOverflowWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_RX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_SteeringAngleOCSCWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OCSC_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_AppsDisagreementWarningCount_Set(CANSIG_FSM_WARNINGS_COUNTS_FSM_APPS_DISAGREEMENT_WARNING_COUNT_START_VAL);
    App_CanTx_FSM_MissingHeartbeatFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_MISSING_HEARTBEAT_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_StateMachineFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_STATE_MACHINE_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_PappsOCSCFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_PAPPS_OCSC_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_SappsOCSCFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_SAPPS_OCSC_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_FlowMeterUnderflowFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_FLOW_METER_UNDERFLOW_FAULT_COUNT_START_VAL);
    App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Set(CANSIG_FSM_FAULTS_COUNTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_COUNT_START_VAL);
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

void App_CanTx_FSM_StackRemainingTask1Hz_Set(uint32_t value)
{
    tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask1Hz_value = (value > CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1_HZ_MAX) ? CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1_HZ_MAX : value;
}

void App_CanTx_FSM_StackRemainingTask100Hz_Set(uint32_t value)
{
    tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask100Hz_value = (value > CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK100_HZ_MAX) ? CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK100_HZ_MAX : value;
}

void App_CanTx_FSM_StackRemainingTask1kHz_Set(uint32_t value)
{
    tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask1kHz_value = (value > CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1K_HZ_MAX) ? CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1K_HZ_MAX : value;
}

void App_CanTx_FSM_StackRemainingTaskCanRx_Set(uint32_t value)
{
    tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTaskCanRx_value = (value > CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_RX_MAX) ? CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_RX_MAX : value;
}

void App_CanTx_FSM_StackRemainingTaskCanTx_Set(uint32_t value)
{
    tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTaskCanTx_value = (value > CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_TX_MAX) ? CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_TX_MAX : value;
}

void App_CanTx_FSM_Hash_Set(uint32_t value)
{
    tx_table.FSM_CommitInfo_signals.FSM_Hash_value = value;
}

void App_CanTx_FSM_Clean_Set(bool value)
{
    tx_table.FSM_CommitInfo_signals.FSM_Clean_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanTx_FSM_WatchdogTimeoutWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value = value;
}

void App_CanTx_FSM_TxOverflowWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value = value;
}

void App_CanTx_FSM_RxOverflowWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value = value;
}

void App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_SteeringAngleOCSCWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value = value;
}

void App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_FlowRateOutOfRangeWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value = value;
}

void App_CanTx_FSM_BrakeAppsDisagreementWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value = value;
}

void App_CanTx_FSM_AppsDisagreementWarning_Set(bool value)
{
    tx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value = value;
}

void App_CanTx_FSM_MissingHeartbeatFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value = value;
}

void App_CanTx_FSM_StateMachineFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_StateMachineFault_value = value;
}

void App_CanTx_FSM_PappsOCSCFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value = value;
}

void App_CanTx_FSM_SappsOCSCFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value = value;
}

void App_CanTx_FSM_FlowMeterUnderflowFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value = value;
}

void App_CanTx_FSM_TorquePlausabilityFailedFault_Set(bool value)
{
    tx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value = value;
}

void App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask1HzWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask100HzWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask1kHzWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTaskCanRxWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTaskCanTxWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_WatchdogTimeoutWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_WatchdogTimeoutWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_TxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_TxOverflowWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_TX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_TX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_RxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_RxOverflowWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_RX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_RX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_LeftWheelSpeedOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_RightWheelSpeedOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_SteeringAngleOCSCWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_SteeringAngleOCSCWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OCSC_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OCSC_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_SteeringAngleOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_FrontBrakePressureOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_RearBrakePressureOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_FlowRateOutOfRangeWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_BrakeAppsDisagreementWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_AppsDisagreementWarningCount_Set(uint32_t value)
{
    tx_table.FSM_WarningsCounts_signals.FSM_AppsDisagreementWarningCount_value = (value > CANSIG_FSM_WARNINGS_COUNTS_FSM_APPS_DISAGREEMENT_WARNING_COUNT_MAX) ? CANSIG_FSM_WARNINGS_COUNTS_FSM_APPS_DISAGREEMENT_WARNING_COUNT_MAX : value;
}

void App_CanTx_FSM_MissingHeartbeatFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_MissingHeartbeatFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_MISSING_HEARTBEAT_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_MISSING_HEARTBEAT_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_StateMachineFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_StateMachineFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_STATE_MACHINE_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_STATE_MACHINE_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_PappsOCSCFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_PappsOCSCFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_PAPPS_OCSC_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_PAPPS_OCSC_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_SappsOCSCFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_SappsOCSCFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_SAPPS_OCSC_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_SAPPS_OCSC_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_FlowMeterUnderflowFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_FlowMeterUnderflowFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_FLOW_METER_UNDERFLOW_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_FLOW_METER_UNDERFLOW_FAULT_COUNT_MAX : value;
}

void App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Set(uint32_t value)
{
    tx_table.FSM_FaultsCounts_signals.FSM_TorquePlausabilityFailedFaultCount_value = (value > CANSIG_FSM_FAULTS_COUNTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_COUNT_MAX) ? CANSIG_FSM_FAULTS_COUNTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_COUNT_MAX : value;
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

uint32_t App_CanTx_FSM_StackRemainingTask1Hz_Get()
{
    return tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask1Hz_value;
}

uint32_t App_CanTx_FSM_StackRemainingTask100Hz_Get()
{
    return tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask100Hz_value;
}

uint32_t App_CanTx_FSM_StackRemainingTask1kHz_Get()
{
    return tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTask1kHz_value;
}

uint32_t App_CanTx_FSM_StackRemainingTaskCanRx_Get()
{
    return tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTaskCanRx_value;
}

uint32_t App_CanTx_FSM_StackRemainingTaskCanTx_Get()
{
    return tx_table.FSM_StackRemaining_signals.FSM_StackRemainingTaskCanTx_value;
}

uint32_t App_CanTx_FSM_Hash_Get()
{
    return tx_table.FSM_CommitInfo_signals.FSM_Hash_value;
}

bool App_CanTx_FSM_Clean_Get()
{
    return tx_table.FSM_CommitInfo_signals.FSM_Clean_value;
}

bool App_CanTx_FSM_StackWaterMarkHighTask1HzWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanTx_FSM_StackWaterMarkHighTask100HzWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanTx_FSM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanTx_FSM_WatchdogTimeoutWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_WatchdogTimeoutWarning_value;
}

bool App_CanTx_FSM_TxOverflowWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_TxOverflowWarning_value;
}

bool App_CanTx_FSM_RxOverflowWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_RxOverflowWarning_value;
}

bool App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_LeftWheelSpeedOutOfRangeWarning_value;
}

bool App_CanTx_FSM_RightWheelSpeedOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_RightWheelSpeedOutOfRangeWarning_value;
}

bool App_CanTx_FSM_SteeringAngleOCSCWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_SteeringAngleOCSCWarning_value;
}

bool App_CanTx_FSM_SteeringAngleOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_SteeringAngleOutOfRangeWarning_value;
}

bool App_CanTx_FSM_FrontBrakePressureOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_FrontBrakePressureOutOfRangeWarning_value;
}

bool App_CanTx_FSM_RearBrakePressureOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_RearBrakePressureOutOfRangeWarning_value;
}

bool App_CanTx_FSM_FlowRateOutOfRangeWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_FlowRateOutOfRangeWarning_value;
}

bool App_CanTx_FSM_BrakeAppsDisagreementWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_BrakeAppsDisagreementWarning_value;
}

bool App_CanTx_FSM_AppsDisagreementWarning_Get()
{
    return tx_table.FSM_Warnings_signals.FSM_AppsDisagreementWarning_value;
}

bool App_CanTx_FSM_MissingHeartbeatFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_MissingHeartbeatFault_value;
}

bool App_CanTx_FSM_StateMachineFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_StateMachineFault_value;
}

bool App_CanTx_FSM_PappsOCSCFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_PappsOCSCFault_value;
}

bool App_CanTx_FSM_SappsOCSCFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_SappsOCSCFault_value;
}

bool App_CanTx_FSM_FlowMeterUnderflowFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_FlowMeterUnderflowFault_value;
}

bool App_CanTx_FSM_TorquePlausabilityFailedFault_Get()
{
    return tx_table.FSM_Faults_signals.FSM_TorquePlausabilityFailedFault_value;
}

uint32_t App_CanTx_FSM_StackWaterMarkHighTask1HzWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask1HzWarningCount_value;
}

uint32_t App_CanTx_FSM_StackWaterMarkHighTask100HzWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask100HzWarningCount_value;
}

uint32_t App_CanTx_FSM_StackWaterMarkHighTask1kHzWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTask1kHzWarningCount_value;
}

uint32_t App_CanTx_FSM_StackWaterMarkHighTaskCanRxWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTaskCanRxWarningCount_value;
}

uint32_t App_CanTx_FSM_StackWaterMarkHighTaskCanTxWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_StackWaterMarkHighTaskCanTxWarningCount_value;
}

uint32_t App_CanTx_FSM_WatchdogTimeoutWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_WatchdogTimeoutWarningCount_value;
}

uint32_t App_CanTx_FSM_TxOverflowWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_TxOverflowWarningCount_value;
}

uint32_t App_CanTx_FSM_RxOverflowWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_RxOverflowWarningCount_value;
}

uint32_t App_CanTx_FSM_LeftWheelSpeedOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_LeftWheelSpeedOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_RightWheelSpeedOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_RightWheelSpeedOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_SteeringAngleOCSCWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_SteeringAngleOCSCWarningCount_value;
}

uint32_t App_CanTx_FSM_SteeringAngleOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_SteeringAngleOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_FrontBrakePressureOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_FrontBrakePressureOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_RearBrakePressureOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_RearBrakePressureOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_FlowRateOutOfRangeWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_FlowRateOutOfRangeWarningCount_value;
}

uint32_t App_CanTx_FSM_BrakeAppsDisagreementWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_BrakeAppsDisagreementWarningCount_value;
}

uint32_t App_CanTx_FSM_AppsDisagreementWarningCount_Get()
{
    return tx_table.FSM_WarningsCounts_signals.FSM_AppsDisagreementWarningCount_value;
}

uint32_t App_CanTx_FSM_MissingHeartbeatFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_MissingHeartbeatFaultCount_value;
}

uint32_t App_CanTx_FSM_StateMachineFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_StateMachineFaultCount_value;
}

uint32_t App_CanTx_FSM_PappsOCSCFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_PappsOCSCFaultCount_value;
}

uint32_t App_CanTx_FSM_SappsOCSCFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_SappsOCSCFaultCount_value;
}

uint32_t App_CanTx_FSM_FlowMeterUnderflowFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_FlowMeterUnderflowFaultCount_value;
}

uint32_t App_CanTx_FSM_TorquePlausabilityFailedFaultCount_Get()
{
    return tx_table.FSM_FaultsCounts_signals.FSM_TorquePlausabilityFailedFaultCount_value;
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

const FSM_StackRemaining_Signals* App_CanTx_FSM_StackRemaining_GetData()
{
    return &tx_table.FSM_StackRemaining_signals;
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

