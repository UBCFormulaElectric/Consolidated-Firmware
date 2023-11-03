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
    App_CanTx_BMS_Warning_StackWaterMarkHighTask1Hz_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTask100Hz_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHz_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRx_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTx_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanTx_BMS_Warning_WatchdogTimeout_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_BMS_Warning_TxOverflow_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_BMS_Warning_RxOverflow_Set(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_BMS_Fault_MissingHeartbeat_Set(CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanTx_BMS_Fault_StateMachine_Set(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_START_VAL);
    App_CanTx_BMS_Fault_CellUndervoltage_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    App_CanTx_BMS_Fault_CellOvervoltage_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    App_CanTx_BMS_Fault_ModuleCommunicationError_Set(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_START_VAL);
    App_CanTx_BMS_Fault_CellUndertemp_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    App_CanTx_BMS_Fault_CellOvertemp_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    App_CanTx_BMS_Fault_Charger_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_START_VAL);
    App_CanTx_BMS_Fault_ChargerDisconnectedDuringCharge_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    App_CanTx_BMS_Fault_ChargerExternalShutdown_Set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_START_VAL);
    App_CanTx_BMS_Fault_TractiveSystemOvercurrent_Set(CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_START_VAL);
    App_CanTx_BMS_Fault_PrechargeFailure_Set(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTask1HzCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTask100HzCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    App_CanTx_BMS_Warning_WatchdogTimeoutCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    App_CanTx_BMS_Warning_TxOverflowCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_BMS_Warning_RxOverflowCount_Set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_BMS_Fault_MissingHeartbeatCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MISSING_HEARTBEAT_COUNT_START_VAL);
    App_CanTx_BMS_Fault_StateMachineCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_START_VAL);
    App_CanTx_BMS_Fault_CellUndervoltageCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_START_VAL);
    App_CanTx_BMS_Fault_CellOvervoltageCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_START_VAL);
    App_CanTx_BMS_Fault_ModuleCommunicationErrorCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_START_VAL);
    App_CanTx_BMS_Fault_CellUndertempCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_START_VAL);
    App_CanTx_BMS_Fault_CellOvertempCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_START_VAL);
    App_CanTx_BMS_Fault_ChargerCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_COUNT_START_VAL);
    App_CanTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_START_VAL);
    App_CanTx_BMS_Fault_ChargerExternalShutdownCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_COUNT_START_VAL);
    App_CanTx_BMS_Fault_TractiveSystemOvercurrentCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_START_VAL);
    App_CanTx_BMS_Fault_PrechargeFailureCount_Set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_START_VAL);
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

void App_CanTx_BMS_Warning_StackWaterMarkHighTask1Hz_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTask100Hz_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHz_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRx_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTx_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanTx_BMS_Warning_WatchdogTimeout_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value = value;
}

void App_CanTx_BMS_Warning_TxOverflow_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value = value;
}

void App_CanTx_BMS_Warning_RxOverflow_Set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value = value;
}

void App_CanTx_BMS_Fault_MissingHeartbeat_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value = value;
}

void App_CanTx_BMS_Fault_StateMachine_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value = value;
}

void App_CanTx_BMS_Fault_CellUndervoltage_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value = value;
}

void App_CanTx_BMS_Fault_CellOvervoltage_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value = value;
}

void App_CanTx_BMS_Fault_ModuleCommunicationError_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value = value;
}

void App_CanTx_BMS_Fault_CellUndertemp_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value = value;
}

void App_CanTx_BMS_Fault_CellOvertemp_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value = value;
}

void App_CanTx_BMS_Fault_Charger_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_Charger_value = value;
}

void App_CanTx_BMS_Fault_ChargerDisconnectedDuringCharge_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value = value;
}

void App_CanTx_BMS_Fault_ChargerExternalShutdown_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value = value;
}

void App_CanTx_BMS_Fault_TractiveSystemOvercurrent_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value = value;
}

void App_CanTx_BMS_Fault_PrechargeFailure_Set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value = value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_WatchdogTimeoutCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_TxOverflowCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_BMS_Warning_RxOverflowCount_Set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_MissingHeartbeatCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_MissingHeartbeatCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MISSING_HEARTBEAT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MISSING_HEARTBEAT_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_StateMachineCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_CellUndervoltageCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_CellOvervoltageCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_ModuleCommunicationErrorCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_CellUndertempCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_CellOvertempCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_ChargerCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_ChargerExternalShutdownCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerExternalShutdownCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_TractiveSystemOvercurrentCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX : value;
}

void App_CanTx_BMS_Fault_PrechargeFailureCount_Set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX : value;
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

bool App_CanTx_BMS_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanTx_BMS_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanTx_BMS_Warning_WatchdogTimeout_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value;
}

bool App_CanTx_BMS_Warning_TxOverflow_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value;
}

bool App_CanTx_BMS_Warning_RxOverflow_Get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value;
}

bool App_CanTx_BMS_Fault_MissingHeartbeat_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_MissingHeartbeat_value;
}

bool App_CanTx_BMS_Fault_StateMachine_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value;
}

bool App_CanTx_BMS_Fault_CellUndervoltage_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value;
}

bool App_CanTx_BMS_Fault_CellOvervoltage_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value;
}

bool App_CanTx_BMS_Fault_ModuleCommunicationError_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value;
}

bool App_CanTx_BMS_Fault_CellUndertemp_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value;
}

bool App_CanTx_BMS_Fault_CellOvertemp_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value;
}

bool App_CanTx_BMS_Fault_Charger_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_Charger_value;
}

bool App_CanTx_BMS_Fault_ChargerDisconnectedDuringCharge_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value;
}

bool App_CanTx_BMS_Fault_ChargerExternalShutdown_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ChargerExternalShutdown_value;
}

bool App_CanTx_BMS_Fault_TractiveSystemOvercurrent_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value;
}

bool App_CanTx_BMS_Fault_PrechargeFailure_Get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value;
}

uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask1HzCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value;
}

uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask100HzCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value;
}

uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value;
}

uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value;
}

uint32_t App_CanTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value;
}

uint32_t App_CanTx_BMS_Warning_WatchdogTimeoutCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value;
}

uint32_t App_CanTx_BMS_Warning_TxOverflowCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value;
}

uint32_t App_CanTx_BMS_Warning_RxOverflowCount_Get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value;
}

uint32_t App_CanTx_BMS_Fault_MissingHeartbeatCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_MissingHeartbeatCount_value;
}

uint32_t App_CanTx_BMS_Fault_StateMachineCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value;
}

uint32_t App_CanTx_BMS_Fault_CellUndervoltageCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value;
}

uint32_t App_CanTx_BMS_Fault_CellOvervoltageCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value;
}

uint32_t App_CanTx_BMS_Fault_ModuleCommunicationErrorCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value;
}

uint32_t App_CanTx_BMS_Fault_CellUndertempCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value;
}

uint32_t App_CanTx_BMS_Fault_CellOvertempCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value;
}

uint32_t App_CanTx_BMS_Fault_ChargerCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerCount_value;
}

uint32_t App_CanTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value;
}

uint32_t App_CanTx_BMS_Fault_ChargerExternalShutdownCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerExternalShutdownCount_value;
}

uint32_t App_CanTx_BMS_Fault_TractiveSystemOvercurrentCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value;
}

uint32_t App_CanTx_BMS_Fault_PrechargeFailureCount_Get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value;
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

