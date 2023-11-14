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
 * Struct for holding all messages transmitted by BMS (i.e. the TX table).
 */
typedef struct
{
    BMS_Vitals_Signals BMS_Vitals_signals;
    BMS_AlertsContext_Signals BMS_AlertsContext_signals;
    BMS_ImdStatus_Signals BMS_ImdStatus_signals;
    BMS_ImdData_Signals BMS_ImdData_signals;
    BMS_ImdPwmOutput_Signals BMS_ImdPwmOutput_signals;
    BMS_Charger_Signals BMS_Charger_signals;
    BMS_OkStatuses_Signals BMS_OkStatuses_signals;
    BMS_Contactors_Signals BMS_Contactors_signals;
    BMS_CellTemperatures_Signals BMS_CellTemperatures_signals;
    BMS_CellVoltages_Signals BMS_CellVoltages_signals;
    BMS_CellStats_Signals BMS_CellStats_signals;
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_PackVoltage_Signals BMS_PackVoltage_signals;
    BMS_AvailablePower_Signals BMS_AvailablePower_signals;
    BMS_AuxThermistors_Signals BMS_AuxThermistors_signals;
    BMS_CommitInfo_Signals BMS_CommitInfo_signals;
    BMS_StackRemaining_Signals BMS_StackRemaining_signals;
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    BMS_WarningsCounts_Signals BMS_WarningsCounts_signals;
    BMS_FaultsCounts_Signals BMS_FaultsCounts_signals;
} BMS_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static BMS_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(BMS_TxMsgs));
    App_CanTx_BMS_Heartbeat_Set(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    App_CanTx_BMS_State_Set(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    App_CanTx_BMS_TxOverflowCount_Set(CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_BMS_RxOverflowCount_Set(CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_BMS_WatchdogTimeoutTaskName_Set(CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_BMS_ImdOkHs_Set(CANSIG_BMS_IMD_STATUS_BMS_IMD_OK_HS_START_VAL);
    App_CanTx_BMS_ImdSecondsSincePowerOn_Set(CANSIG_BMS_IMD_STATUS_BMS_IMD_SECONDS_SINCE_POWER_ON_START_VAL);
    App_CanTx_BMS_ImdValidDutyCycle_Set(CANSIG_BMS_IMD_STATUS_BMS_IMD_VALID_DUTY_CYCLE_START_VAL);
    App_CanTx_BMS_ImdCondition_Set(CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_START_VAL);
    App_CanTx_BMS_ImdActiveFrequency_Set(CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_START_VAL);
    App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Set(CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_START_VAL);
    App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Set(CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_START_VAL);
    App_CanTx_BMS_ImdSpeedStartStatus30Hz_Set(CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_START_VAL);
    App_CanTx_BMS_ImdDutyCycle_Set(CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_START_VAL);
    App_CanTx_BMS_ImdFrequency_Set(CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_START_VAL);
    App_CanTx_BMS_ChargerConnected_Set(CANSIG_BMS_CHARGER_BMS_CHARGER_CONNECTED_START_VAL);
    App_CanTx_BMS_ChargingComplete_Set(CANSIG_BMS_CHARGER_BMS_CHARGING_COMPLETE_START_VAL);
    App_CanTx_BMS_BmsOk_Set(CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_START_VAL);
    App_CanTx_BMS_ImdOk_Set(CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_START_VAL);
    App_CanTx_BMS_BspdOk_Set(CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_START_VAL);
    App_CanTx_BMS_BmsLatchedFault_Set(CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_START_VAL);
    App_CanTx_BMS_ImdLatchedFault_Set(CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_START_VAL);
    App_CanTx_BMS_BspdLatchedFault_Set(CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_START_VAL);
    App_CanTx_BMS_AirPositive_Set(CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_START_VAL);
    App_CanTx_BMS_AirNegative_Set(CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_START_VAL);
    App_CanTx_BMS_PrechargeRelay_Set(CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_START_VAL);
    App_CanTx_BMS_MinCellTemperature_Set(CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_START_VAL);
    App_CanTx_BMS_MaxCellTemperature_Set(CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_START_VAL);
    App_CanTx_BMS_MinCellVoltage_Set(CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_START_VAL);
    App_CanTx_BMS_MaxCellVoltage_Set(CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_START_VAL);
    App_CanTx_BMS_MinCellVoltageSegment_Set(CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_START_VAL);
    App_CanTx_BMS_MinCellVoltageIdx_Set(CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_START_VAL);
    App_CanTx_BMS_MaxCellVoltageSegment_Set(CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_START_VAL);
    App_CanTx_BMS_MaxCellVoltageIdx_Set(CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_START_VAL);
    App_CanTx_BMS_MinTempSegment_Set(CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_START_VAL);
    App_CanTx_BMS_MinTempIdx_Set(CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_START_VAL);
    App_CanTx_BMS_MaxTempSegment_Set(CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_START_VAL);
    App_CanTx_BMS_MaxTempIdx_Set(CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_START_VAL);
    App_CanTx_BMS_TractiveSystemVoltage_Set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    App_CanTx_BMS_TractiveSystemCurrent_Set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    App_CanTx_BMS_PackVoltage_Set(CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_START_VAL);
    App_CanTx_BMS_AvailablePower_Set(CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_START_VAL);
    App_CanTx_BMS_ThermTemp0_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_START_VAL);
    App_CanTx_BMS_ThermTemp1_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_START_VAL);
    App_CanTx_BMS_ThermTemp2_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_START_VAL);
    App_CanTx_BMS_ThermTemp3_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_START_VAL);
    App_CanTx_BMS_ThermTemp4_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_START_VAL);
    App_CanTx_BMS_ThermTemp5_Set(CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_START_VAL);
    App_CanTx_BMS_Hash_Set(CANSIG_BMS_COMMIT_INFO_BMS_HASH_START_VAL);
    App_CanTx_BMS_Clean_Set(CANSIG_BMS_COMMIT_INFO_BMS_CLEAN_START_VAL);
    App_CanTx_BMS_StackRemainingTask1Hz_Set(CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1_HZ_START_VAL);
    App_CanTx_BMS_StackRemainingTask100Hz_Set(CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK100_HZ_START_VAL);
    App_CanTx_BMS_StackRemainingTask1kHz_Set(CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1K_HZ_START_VAL);
    App_CanTx_BMS_StackRemainingTaskCanRx_Set(CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_RX_START_VAL);
    App_CanTx_BMS_StackRemainingTaskCanTx_Set(CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_TX_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask1HzWarning_Set(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask100HzWarning_Set(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask1kHzWarning_Set(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarning_Set(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarning_Set(CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanTx_BMS_WatchdogTimeoutWarning_Set(CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanTx_BMS_TxOverflowWarning_Set(CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_BMS_RxOverflowWarning_Set(CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_BMS_MissingHeartbeatFault_Set(CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanTx_BMS_StateMachineFault_Set(CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_START_VAL);
    App_CanTx_BMS_CellUndervoltageFault_Set(CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_START_VAL);
    App_CanTx_BMS_CellOvervoltageFault_Set(CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_START_VAL);
    App_CanTx_BMS_ModuleCommunicationFault_Set(CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_START_VAL);
    App_CanTx_BMS_CellUndertempFault_Set(CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_START_VAL);
    App_CanTx_BMS_CellOvertempFault_Set(CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_START_VAL);
    App_CanTx_BMS_ChargerFault_Set(CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_START_VAL);
    App_CanTx_BMS_ChargerDisconnectedDuringChargeFault_Set(CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_START_VAL);
    App_CanTx_BMS_ChargerExternalShutdownFault_Set(CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_START_VAL);
    App_CanTx_BMS_TractiveSystemOvercurrentFault_Set(CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_START_VAL);
    App_CanTx_BMS_PrechargeFailureFault_Set(CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask1HzWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask100HzWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTask1kHzWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_WatchdogTimeoutWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WATCHDOG_TIMEOUT_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_TxOverflowWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_TX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_RxOverflowWarningCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_RX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_BMS_MissingHeartbeatFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_MISSING_HEARTBEAT_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_StateMachineFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_STATE_MACHINE_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_CellUndervoltageFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERVOLTAGE_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_CellOvervoltageFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERVOLTAGE_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_ModuleCommunicationFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_MODULE_COMMUNICATION_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_CellUndertempFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERTEMP_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_CellOvertempFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERTEMP_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_ChargerFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_ChargerDisconnectedDuringChargeFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_ChargerExternalShutdownFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_TractiveSystemOvercurrentFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_COUNT_START_VAL);
    App_CanTx_BMS_PrechargeFailureFaultCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_PRECHARGE_FAILURE_FAULT_COUNT_START_VAL);
}

void App_CanTx_BMS_Heartbeat_Set(bool value)
{
    tx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value;
}

void App_CanTx_BMS_State_Set(BmsState value)
{
    tx_table.BMS_Vitals_signals.BMS_State_value = (value > CANSIG_BMS_VITALS_BMS_STATE_MAX) ? CANSIG_BMS_VITALS_BMS_STATE_MAX : value;
}

void App_CanTx_BMS_TxOverflowCount_Set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_BMS_RxOverflowCount_Set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_BMS_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_BMS_ImdOkHs_Set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value = value;
}

void App_CanTx_BMS_ImdSecondsSincePowerOn_Set(uint32_t value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdSecondsSincePowerOn_value = (value > CANSIG_BMS_IMD_STATUS_BMS_IMD_SECONDS_SINCE_POWER_ON_MAX) ? CANSIG_BMS_IMD_STATUS_BMS_IMD_SECONDS_SINCE_POWER_ON_MAX : value;
}

void App_CanTx_BMS_ImdValidDutyCycle_Set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value = value;
}

void App_CanTx_BMS_ImdCondition_Set(uint32_t value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value = (value > CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_MAX) ? CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_MAX : value;
}

void App_CanTx_BMS_ImdActiveFrequency_Set(ImdActiveFrequency value)
{
    tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value = (value > CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_MAX) ? CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_MAX : value;
}

void App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX : tmp;
}

void App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX : tmp;
}

void App_CanTx_BMS_ImdSpeedStartStatus30Hz_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MAX : tmp;
}

void App_CanTx_BMS_ImdDutyCycle_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_MIN ? CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_MIN : value;
    tx_table.BMS_ImdPwmOutput_signals.BMS_ImdDutyCycle_value = tmp > CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_MAX ? CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_MAX : tmp;
}

void App_CanTx_BMS_ImdFrequency_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_MIN ? CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_MIN : value;
    tx_table.BMS_ImdPwmOutput_signals.BMS_ImdFrequency_value = tmp > CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_MAX ? CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_MAX : tmp;
}

void App_CanTx_BMS_ChargerConnected_Set(bool value)
{
    tx_table.BMS_Charger_signals.BMS_ChargerConnected_value = value;
}

void App_CanTx_BMS_ChargingComplete_Set(bool value)
{
    tx_table.BMS_Charger_signals.BMS_ChargingComplete_value = value;
}

void App_CanTx_BMS_BmsOk_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value;
}

void App_CanTx_BMS_ImdOk_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value;
}

void App_CanTx_BMS_BspdOk_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value;
}

void App_CanTx_BMS_BmsLatchedFault_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value;
}

void App_CanTx_BMS_ImdLatchedFault_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value;
}

void App_CanTx_BMS_BspdLatchedFault_Set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value;
}

void App_CanTx_BMS_AirPositive_Set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirPositive_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX : value;
}

void App_CanTx_BMS_AirNegative_Set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirNegative_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX : value;
}

void App_CanTx_BMS_PrechargeRelay_Set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value = (value > CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX) ? CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX : value;
}

void App_CanTx_BMS_MinCellTemperature_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MIN ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MIN : value;
    tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value = tmp > CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MAX ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MAX : tmp;
}

void App_CanTx_BMS_MaxCellTemperature_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MIN ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MIN : value;
    tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value = tmp > CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MAX ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MAX : tmp;
}

void App_CanTx_BMS_MinCellVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MIN : value;
    tx_table.BMS_CellVoltages_signals.BMS_MinCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_MAX : tmp;
}

void App_CanTx_BMS_MaxCellVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MIN ? CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MIN : value;
    tx_table.BMS_CellVoltages_signals.BMS_MaxCellVoltage_value = tmp > CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MAX ? CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_MAX : tmp;
}

void App_CanTx_BMS_MinCellVoltageSegment_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX : value;
}

void App_CanTx_BMS_MinCellVoltageIdx_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_MAX : value;
}

void App_CanTx_BMS_MaxCellVoltageSegment_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX : value;
}

void App_CanTx_BMS_MaxCellVoltageIdx_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_MAX : value;
}

void App_CanTx_BMS_MinTempSegment_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_MAX : value;
}

void App_CanTx_BMS_MinTempIdx_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_MAX : value;
}

void App_CanTx_BMS_MaxTempSegment_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_MAX : value;
}

void App_CanTx_BMS_MaxTempIdx_Set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_MAX : value;
}

void App_CanTx_BMS_TractiveSystemVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN : value;
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX : tmp;
}

void App_CanTx_BMS_TractiveSystemCurrent_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN : value;
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX : tmp;
}

void App_CanTx_BMS_PackVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_MIN ? CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_MIN : value;
    tx_table.BMS_PackVoltage_signals.BMS_PackVoltage_value = tmp > CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_MAX ? CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_MAX : tmp;
}

void App_CanTx_BMS_AvailablePower_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MIN ? CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MIN : value;
    tx_table.BMS_AvailablePower_signals.BMS_AvailablePower_value = tmp > CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MAX ? CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp0_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp0_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp1_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp1_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp2_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp2_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp3_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp3_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp4_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp4_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_MAX : tmp;
}

void App_CanTx_BMS_ThermTemp5_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_MIN ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_MIN : value;
    tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp5_value = tmp > CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_MAX ? CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_MAX : tmp;
}

void App_CanTx_BMS_Hash_Set(uint32_t value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Hash_value = value;
}

void App_CanTx_BMS_Clean_Set(bool value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Clean_value = value;
}

void App_CanTx_BMS_StackRemainingTask1Hz_Set(uint32_t value)
{
    tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask1Hz_value = (value > CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1_HZ_MAX) ? CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1_HZ_MAX : value;
}

void App_CanTx_BMS_StackRemainingTask100Hz_Set(uint32_t value)
{
    tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask100Hz_value = (value > CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK100_HZ_MAX) ? CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK100_HZ_MAX : value;
}

void App_CanTx_BMS_StackRemainingTask1kHz_Set(uint32_t value)
{
    tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask1kHz_value = (value > CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1K_HZ_MAX) ? CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1K_HZ_MAX : value;
}

void App_CanTx_BMS_StackRemainingTaskCanRx_Set(uint32_t value)
{
    tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTaskCanRx_value = (value > CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_RX_MAX) ? CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_RX_MAX : value;
}

void App_CanTx_BMS_StackRemainingTaskCanTx_Set(uint32_t value)
{
    tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTaskCanTx_value = (value > CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_TX_MAX) ? CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_TX_MAX : value;
}

void App_CanTx_BMS_StackWaterMarkHighTask1HzWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanTx_BMS_StackWaterMarkHighTask100HzWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanTx_BMS_StackWaterMarkHighTask1kHzWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanTx_BMS_WatchdogTimeoutWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value = value;
}

void App_CanTx_BMS_TxOverflowWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value = value;
}

void App_CanTx_BMS_RxOverflowWarning_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value = value;
}

void App_CanTx_BMS_MissingHeartbeatFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value = value;
}

void App_CanTx_BMS_StateMachineFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_StateMachineFault_value = value;
}

void App_CanTx_BMS_CellUndervoltageFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value = value;
}

void App_CanTx_BMS_CellOvervoltageFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value = value;
}

void App_CanTx_BMS_ModuleCommunicationFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value = value;
}

void App_CanTx_BMS_CellUndertempFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_CellUndertempFault_value = value;
}

void App_CanTx_BMS_CellOvertempFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_CellOvertempFault_value = value;
}

void App_CanTx_BMS_ChargerFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_ChargerFault_value = value;
}

void App_CanTx_BMS_ChargerDisconnectedDuringChargeFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value = value;
}

void App_CanTx_BMS_ChargerExternalShutdownFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value = value;
}

void App_CanTx_BMS_TractiveSystemOvercurrentFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value = value;
}

void App_CanTx_BMS_PrechargeFailureFault_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value = value;
}

void App_CanTx_BMS_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask1HzWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask100HzWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask1kHzWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTaskCanRxWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTaskCanTxWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_WatchdogTimeoutWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_WatchdogTimeoutWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_TxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_TxOverflowWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_TX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_TX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_RxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_RxOverflowWarningCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_RX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_RX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_BMS_MissingHeartbeatFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_MissingHeartbeatFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_MISSING_HEARTBEAT_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_MISSING_HEARTBEAT_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_StateMachineFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_StateMachineFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_STATE_MACHINE_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_STATE_MACHINE_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_CellUndervoltageFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_CellUndervoltageFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERVOLTAGE_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERVOLTAGE_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_CellOvervoltageFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_CellOvervoltageFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERVOLTAGE_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERVOLTAGE_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_ModuleCommunicationFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_ModuleCommunicationFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_MODULE_COMMUNICATION_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_MODULE_COMMUNICATION_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_CellUndertempFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_CellUndertempFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERTEMP_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERTEMP_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_CellOvertempFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_CellOvertempFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERTEMP_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERTEMP_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_ChargerFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_ChargerFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_ChargerDisconnectedDuringChargeFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_ChargerDisconnectedDuringChargeFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_ChargerExternalShutdownFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_ChargerExternalShutdownFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_TractiveSystemOvercurrentFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_TractiveSystemOvercurrentFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_COUNT_MAX : value;
}

void App_CanTx_BMS_PrechargeFailureFaultCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_PrechargeFailureFaultCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_PRECHARGE_FAILURE_FAULT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_PRECHARGE_FAILURE_FAULT_COUNT_MAX : value;
}

bool App_CanTx_BMS_Heartbeat_Get()
{
    return tx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}

BmsState App_CanTx_BMS_State_Get()
{
    return tx_table.BMS_Vitals_signals.BMS_State_value;
}

uint32_t App_CanTx_BMS_TxOverflowCount_Get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value;
}

uint32_t App_CanTx_BMS_RxOverflowCount_Get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value;
}

RtosTaskName App_CanTx_BMS_WatchdogTimeoutTaskName_Get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value;
}

bool App_CanTx_BMS_ImdOkHs_Get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value;
}

uint32_t App_CanTx_BMS_ImdSecondsSincePowerOn_Get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdSecondsSincePowerOn_value;
}

bool App_CanTx_BMS_ImdValidDutyCycle_Get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value;
}

uint32_t App_CanTx_BMS_ImdCondition_Get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value;
}

ImdActiveFrequency App_CanTx_BMS_ImdActiveFrequency_Get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value;
}

float App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value;
}

float App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value;
}

float App_CanTx_BMS_ImdSpeedStartStatus30Hz_Get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value;
}

float App_CanTx_BMS_ImdDutyCycle_Get()
{
    return tx_table.BMS_ImdPwmOutput_signals.BMS_ImdDutyCycle_value;
}

float App_CanTx_BMS_ImdFrequency_Get()
{
    return tx_table.BMS_ImdPwmOutput_signals.BMS_ImdFrequency_value;
}

bool App_CanTx_BMS_ChargerConnected_Get()
{
    return tx_table.BMS_Charger_signals.BMS_ChargerConnected_value;
}

bool App_CanTx_BMS_ChargingComplete_Get()
{
    return tx_table.BMS_Charger_signals.BMS_ChargingComplete_value;
}

bool App_CanTx_BMS_BmsOk_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}

bool App_CanTx_BMS_ImdOk_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}

bool App_CanTx_BMS_BspdOk_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}

bool App_CanTx_BMS_BmsLatchedFault_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}

bool App_CanTx_BMS_ImdLatchedFault_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}

bool App_CanTx_BMS_BspdLatchedFault_Get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
}

ContactorState App_CanTx_BMS_AirPositive_Get()
{
    return tx_table.BMS_Contactors_signals.BMS_AirPositive_value;
}

ContactorState App_CanTx_BMS_AirNegative_Get()
{
    return tx_table.BMS_Contactors_signals.BMS_AirNegative_value;
}

ContactorState App_CanTx_BMS_PrechargeRelay_Get()
{
    return tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value;
}

float App_CanTx_BMS_MinCellTemperature_Get()
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value;
}

float App_CanTx_BMS_MaxCellTemperature_Get()
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value;
}

float App_CanTx_BMS_MinCellVoltage_Get()
{
    return tx_table.BMS_CellVoltages_signals.BMS_MinCellVoltage_value;
}

float App_CanTx_BMS_MaxCellVoltage_Get()
{
    return tx_table.BMS_CellVoltages_signals.BMS_MaxCellVoltage_value;
}

uint32_t App_CanTx_BMS_MinCellVoltageSegment_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value;
}

uint32_t App_CanTx_BMS_MinCellVoltageIdx_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value;
}

uint32_t App_CanTx_BMS_MaxCellVoltageSegment_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value;
}

uint32_t App_CanTx_BMS_MaxCellVoltageIdx_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value;
}

uint32_t App_CanTx_BMS_MinTempSegment_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value;
}

uint32_t App_CanTx_BMS_MinTempIdx_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value;
}

uint32_t App_CanTx_BMS_MaxTempSegment_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value;
}

uint32_t App_CanTx_BMS_MaxTempIdx_Get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value;
}

float App_CanTx_BMS_TractiveSystemVoltage_Get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}

float App_CanTx_BMS_TractiveSystemCurrent_Get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}

float App_CanTx_BMS_PackVoltage_Get()
{
    return tx_table.BMS_PackVoltage_signals.BMS_PackVoltage_value;
}

float App_CanTx_BMS_AvailablePower_Get()
{
    return tx_table.BMS_AvailablePower_signals.BMS_AvailablePower_value;
}

float App_CanTx_BMS_ThermTemp0_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp0_value;
}

float App_CanTx_BMS_ThermTemp1_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp1_value;
}

float App_CanTx_BMS_ThermTemp2_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp2_value;
}

float App_CanTx_BMS_ThermTemp3_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp3_value;
}

float App_CanTx_BMS_ThermTemp4_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp4_value;
}

float App_CanTx_BMS_ThermTemp5_Get()
{
    return tx_table.BMS_AuxThermistors_signals.BMS_ThermTemp5_value;
}

uint32_t App_CanTx_BMS_Hash_Get()
{
    return tx_table.BMS_CommitInfo_signals.BMS_Hash_value;
}

bool App_CanTx_BMS_Clean_Get()
{
    return tx_table.BMS_CommitInfo_signals.BMS_Clean_value;
}

uint32_t App_CanTx_BMS_StackRemainingTask1Hz_Get()
{
    return tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask1Hz_value;
}

uint32_t App_CanTx_BMS_StackRemainingTask100Hz_Get()
{
    return tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask100Hz_value;
}

uint32_t App_CanTx_BMS_StackRemainingTask1kHz_Get()
{
    return tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTask1kHz_value;
}

uint32_t App_CanTx_BMS_StackRemainingTaskCanRx_Get()
{
    return tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTaskCanRx_value;
}

uint32_t App_CanTx_BMS_StackRemainingTaskCanTx_Get()
{
    return tx_table.BMS_StackRemaining_signals.BMS_StackRemainingTaskCanTx_value;
}

bool App_CanTx_BMS_StackWaterMarkHighTask1HzWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanTx_BMS_StackWaterMarkHighTask100HzWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanTx_BMS_StackWaterMarkHighTask1kHzWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanTx_BMS_WatchdogTimeoutWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_WatchdogTimeoutWarning_value;
}

bool App_CanTx_BMS_TxOverflowWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_TxOverflowWarning_value;
}

bool App_CanTx_BMS_RxOverflowWarning_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_RxOverflowWarning_value;
}

bool App_CanTx_BMS_MissingHeartbeatFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_MissingHeartbeatFault_value;
}

bool App_CanTx_BMS_StateMachineFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_StateMachineFault_value;
}

bool App_CanTx_BMS_CellUndervoltageFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_CellUndervoltageFault_value;
}

bool App_CanTx_BMS_CellOvervoltageFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_CellOvervoltageFault_value;
}

bool App_CanTx_BMS_ModuleCommunicationFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_ModuleCommunicationFault_value;
}

bool App_CanTx_BMS_CellUndertempFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_CellUndertempFault_value;
}

bool App_CanTx_BMS_CellOvertempFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_CellOvertempFault_value;
}

bool App_CanTx_BMS_ChargerFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_ChargerFault_value;
}

bool App_CanTx_BMS_ChargerDisconnectedDuringChargeFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_ChargerDisconnectedDuringChargeFault_value;
}

bool App_CanTx_BMS_ChargerExternalShutdownFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_ChargerExternalShutdownFault_value;
}

bool App_CanTx_BMS_TractiveSystemOvercurrentFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_TractiveSystemOvercurrentFault_value;
}

bool App_CanTx_BMS_PrechargeFailureFault_Get()
{
    return tx_table.BMS_Faults_signals.BMS_PrechargeFailureFault_value;
}

uint32_t App_CanTx_BMS_StackWaterMarkHighTask1HzWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask1HzWarningCount_value;
}

uint32_t App_CanTx_BMS_StackWaterMarkHighTask100HzWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask100HzWarningCount_value;
}

uint32_t App_CanTx_BMS_StackWaterMarkHighTask1kHzWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTask1kHzWarningCount_value;
}

uint32_t App_CanTx_BMS_StackWaterMarkHighTaskCanRxWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTaskCanRxWarningCount_value;
}

uint32_t App_CanTx_BMS_StackWaterMarkHighTaskCanTxWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_StackWaterMarkHighTaskCanTxWarningCount_value;
}

uint32_t App_CanTx_BMS_WatchdogTimeoutWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_WatchdogTimeoutWarningCount_value;
}

uint32_t App_CanTx_BMS_TxOverflowWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_TxOverflowWarningCount_value;
}

uint32_t App_CanTx_BMS_RxOverflowWarningCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_RxOverflowWarningCount_value;
}

uint32_t App_CanTx_BMS_MissingHeartbeatFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_MissingHeartbeatFaultCount_value;
}

uint32_t App_CanTx_BMS_StateMachineFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_StateMachineFaultCount_value;
}

uint32_t App_CanTx_BMS_CellUndervoltageFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_CellUndervoltageFaultCount_value;
}

uint32_t App_CanTx_BMS_CellOvervoltageFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_CellOvervoltageFaultCount_value;
}

uint32_t App_CanTx_BMS_ModuleCommunicationFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_ModuleCommunicationFaultCount_value;
}

uint32_t App_CanTx_BMS_CellUndertempFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_CellUndertempFaultCount_value;
}

uint32_t App_CanTx_BMS_CellOvertempFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_CellOvertempFaultCount_value;
}

uint32_t App_CanTx_BMS_ChargerFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_ChargerFaultCount_value;
}

uint32_t App_CanTx_BMS_ChargerDisconnectedDuringChargeFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_ChargerDisconnectedDuringChargeFaultCount_value;
}

uint32_t App_CanTx_BMS_ChargerExternalShutdownFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_ChargerExternalShutdownFaultCount_value;
}

uint32_t App_CanTx_BMS_TractiveSystemOvercurrentFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_TractiveSystemOvercurrentFaultCount_value;
}

uint32_t App_CanTx_BMS_PrechargeFailureFaultCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_PrechargeFailureFaultCount_value;
}

const BMS_Vitals_Signals* App_CanTx_BMS_Vitals_GetData()
{
    return &tx_table.BMS_Vitals_signals;
}

const BMS_AlertsContext_Signals* App_CanTx_BMS_AlertsContext_GetData()
{
    return &tx_table.BMS_AlertsContext_signals;
}

const BMS_ImdStatus_Signals* App_CanTx_BMS_ImdStatus_GetData()
{
    return &tx_table.BMS_ImdStatus_signals;
}

const BMS_ImdData_Signals* App_CanTx_BMS_ImdData_GetData()
{
    return &tx_table.BMS_ImdData_signals;
}

const BMS_ImdPwmOutput_Signals* App_CanTx_BMS_ImdPwmOutput_GetData()
{
    return &tx_table.BMS_ImdPwmOutput_signals;
}

const BMS_Charger_Signals* App_CanTx_BMS_Charger_GetData()
{
    return &tx_table.BMS_Charger_signals;
}

const BMS_OkStatuses_Signals* App_CanTx_BMS_OkStatuses_GetData()
{
    return &tx_table.BMS_OkStatuses_signals;
}

const BMS_Contactors_Signals* App_CanTx_BMS_Contactors_GetData()
{
    return &tx_table.BMS_Contactors_signals;
}

const BMS_CellTemperatures_Signals* App_CanTx_BMS_CellTemperatures_GetData()
{
    return &tx_table.BMS_CellTemperatures_signals;
}

const BMS_CellVoltages_Signals* App_CanTx_BMS_CellVoltages_GetData()
{
    return &tx_table.BMS_CellVoltages_signals;
}

const BMS_CellStats_Signals* App_CanTx_BMS_CellStats_GetData()
{
    return &tx_table.BMS_CellStats_signals;
}

const BMS_TractiveSystem_Signals* App_CanTx_BMS_TractiveSystem_GetData()
{
    return &tx_table.BMS_TractiveSystem_signals;
}

const BMS_PackVoltage_Signals* App_CanTx_BMS_PackVoltage_GetData()
{
    return &tx_table.BMS_PackVoltage_signals;
}

const BMS_AvailablePower_Signals* App_CanTx_BMS_AvailablePower_GetData()
{
    return &tx_table.BMS_AvailablePower_signals;
}

const BMS_AuxThermistors_Signals* App_CanTx_BMS_AuxThermistors_GetData()
{
    return &tx_table.BMS_AuxThermistors_signals;
}

const BMS_CommitInfo_Signals* App_CanTx_BMS_CommitInfo_GetData()
{
    return &tx_table.BMS_CommitInfo_signals;
}

const BMS_StackRemaining_Signals* App_CanTx_BMS_StackRemaining_GetData()
{
    return &tx_table.BMS_StackRemaining_signals;
}

const BMS_Warnings_Signals* App_CanTx_BMS_Warnings_GetData()
{
    return &tx_table.BMS_Warnings_signals;
}

const BMS_Faults_Signals* App_CanTx_BMS_Faults_GetData()
{
    return &tx_table.BMS_Faults_signals;
}

const BMS_WarningsCounts_Signals* App_CanTx_BMS_WarningsCounts_GetData()
{
    return &tx_table.BMS_WarningsCounts_signals;
}

const BMS_FaultsCounts_Signals* App_CanTx_BMS_FaultsCounts_GetData()
{
    return &tx_table.BMS_FaultsCounts_signals;
}

