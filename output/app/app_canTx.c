/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canTx.h"


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
    BMS_Charger_Signals BMS_Charger_signals;
    BMS_BrusaControls_Signals BMS_BrusaControls_signals;
    BMS_OkStatuses_Signals BMS_OkStatuses_signals;
    BMS_Contactors_Signals BMS_Contactors_signals;
    BMS_CellTemperatures_Signals BMS_CellTemperatures_signals;
    BMS_VoltageAndChargeStats_Signals BMS_VoltageAndChargeStats_signals;
    BMS_CellStats_Signals BMS_CellStats_signals;
    BMS_TractiveSystem_Signals BMS_TractiveSystem_signals;
    BMS_CommitInfo_Signals BMS_CommitInfo_signals;
    BMS_OWC_Segment0to2_Status_Signals BMS_OWC_Segment0to2_Status_signals;
    BMS_OWC_Segment3to4_Status_Signals BMS_OWC_Segment3to4_Status_signals;
    BMS_BMSShdnNodeStatus_Signals BMS_BMSShdnNodeStatus_signals;
} BMS_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static _TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canTx_init()
{
    memset(&tx_table, 0, sizeof(BMS_TxMsgs));
    app_canTx_BMS_Heartbeat_set(CANSIG_BMS_HEARTBEAT_START_VAL);
    app_canTx_BMS_State_set(CANSIG_BMS_STATE_START_VAL);
    app_canTx_BMS_TxOverflowCount_set(CANSIG_BMS_TX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_RxOverflowCount_set(CANSIG_BMS_RX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_WatchdogTimeoutTaskName_set(CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    app_canTx_BMS_ModuleCommunication_NumCommTries_set(CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_START_VAL);
    app_canTx_BMS_ModuleCommunication_MonitorState_set(CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_START_VAL);
    app_canTx_BMS_ImdOkHs_set(CANSIG_BMS_IMD_OK_HS_START_VAL);
    app_canTx_BMS_ImdTimeSincePowerOn_set(CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_START_VAL);
    app_canTx_BMS_ImdValidDutyCycle_set(CANSIG_BMS_IMD_VALID_DUTY_CYCLE_START_VAL);
    app_canTx_BMS_ImdCondition_set(CANSIG_BMS_IMD_CONDITION_START_VAL);
    app_canTx_BMS_ImdDutyCycle_set(CANSIG_BMS_IMD_DUTY_CYCLE_START_VAL);
    app_canTx_BMS_ImdFrequency_set(CANSIG_BMS_IMD_FREQUENCY_START_VAL);
    app_canTx_BMS_ImdActiveFrequency_set(CANSIG_BMS_IMD_ACTIVE_FREQUENCY_START_VAL);
    app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_START_VAL);
    app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_START_VAL);
    app_canTx_BMS_ImdSpeedStartStatus30Hz_set(CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_START_VAL);
    app_canTx_BMS_ChargerConnected_set(CANSIG_BMS_CHARGER_CONNECTED_START_VAL);
    app_canTx_BMS_ChargerEnable_set(CANSIG_BMS_CHARGER_ENABLE_START_VAL);
    app_canTx_BMS_ClearLatch_set(CANSIG_BMS_CLEAR_LATCH_START_VAL);
    app_canTx_BMS_MaxMainsCurrent_set(CANSIG_BMS_MAX_MAINS_CURRENT_START_VAL);
    app_canTx_BMS_ChargingVoltage_set(CANSIG_BMS_CHARGING_VOLTAGE_START_VAL);
    app_canTx_BMS_ChargingCurrent_set(CANSIG_BMS_CHARGING_CURRENT_START_VAL);
    app_canTx_BMS_BmsOk_set(CANSIG_BMS_BMS_OK_START_VAL);
    app_canTx_BMS_ImdOk_set(CANSIG_BMS_IMD_OK_START_VAL);
    app_canTx_BMS_BspdOk_set(CANSIG_BMS_BSPD_OK_START_VAL);
    app_canTx_BMS_BmsLatchedFault_set(CANSIG_BMS_BMS_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_ImdLatchedFault_set(CANSIG_BMS_IMD_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_BspdLatchedFault_set(CANSIG_BMS_BSPD_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_AirPositive_set(CANSIG_BMS_AIR_POSITIVE_START_VAL);
    app_canTx_BMS_AirNegative_set(CANSIG_BMS_AIR_NEGATIVE_START_VAL);
    app_canTx_BMS_PrechargeRelay_set(CANSIG_BMS_PRECHARGE_RELAY_START_VAL);
    app_canTx_BMS_MinCellTemperature_set(CANSIG_BMS_MIN_CELL_TEMPERATURE_START_VAL);
    app_canTx_BMS_MaxCellTemperature_set(CANSIG_BMS_MAX_CELL_TEMPERATURE_START_VAL);
    app_canTx_BMS_PackVoltage_set(CANSIG_BMS_PACK_VOLTAGE_START_VAL);
    app_canTx_BMS_Soc_set(CANSIG_BMS_SOC_START_VAL);
    app_canTx_BMS_SocCorrupt_set(CANSIG_BMS_SOC_CORRUPT_START_VAL);
    app_canTx_BMS_MinCellVoltage_set(CANSIG_BMS_MIN_CELL_VOLTAGE_START_VAL);
    app_canTx_BMS_MaxCellVoltage_set(CANSIG_BMS_MAX_CELL_VOLTAGE_START_VAL);
    app_canTx_BMS_MinCellVoltageSegment_set(CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_START_VAL);
    app_canTx_BMS_MinCellVoltageIdx_set(CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_START_VAL);
    app_canTx_BMS_MaxCellVoltageSegment_set(CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_START_VAL);
    app_canTx_BMS_MaxCellVoltageIdx_set(CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_START_VAL);
    app_canTx_BMS_MinTempSegment_set(CANSIG_BMS_MIN_TEMP_SEGMENT_START_VAL);
    app_canTx_BMS_MinTempIdx_set(CANSIG_BMS_MIN_TEMP_IDX_START_VAL);
    app_canTx_BMS_MaxTempSegment_set(CANSIG_BMS_MAX_TEMP_SEGMENT_START_VAL);
    app_canTx_BMS_MaxTempIdx_set(CANSIG_BMS_MAX_TEMP_IDX_START_VAL);
    app_canTx_BMS_TractiveSystemVoltage_set(CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    app_canTx_BMS_TractiveSystemCurrent_set(CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    app_canTx_BMS_TractiveSystemPower_set(CANSIG_BMS_TRACTIVE_SYSTEM_POWER_START_VAL);
    app_canTx_BMS_AvailablePower_set(CANSIG_BMS_AVAILABLE_POWER_START_VAL);
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL);
    app_canTx_BMS_Hash_set(CANSIG_BMS_HASH_START_VAL);
    app_canTx_BMS_Clean_set(CANSIG_BMS_CLEAN_START_VAL);
    app_canTx_BMS_Segment0_OWC_Cells_Status_set(CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment1_OWC_Cells_Status_set(CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment2_OWC_Cells_Status_set(CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment3_OWC_Cells_Status_set(CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment4_OWC_Cells_Status_set(CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_HVDShdnOKStatus_set(CANSIG_BMS_HVD_SHDN_OK_STATUS_START_VAL);
    app_canTx_BMS_TSIlckOKStatus_set(CANSIG_BMS_TS_ILCK_OK_STATUS_START_VAL);
}
void app_canTx_BMS_Heartbeat_set(bool value)
{
    tx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HEARTBEAT_MAX_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MAX_VAL;
    }
    if (value < CANSIG_BMS_HEARTBEAT_MIN_VAL)
    {
        value = CANSIG_BMS_HEARTBEAT_MIN_VAL;
    }
}
void app_canTx_BMS_State_set(BmsState value)
{
    tx_table.BMS_Vitals_signals.BMS_State_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_STATE_MAX_VAL)
    {
        value = CANSIG_BMS_STATE_MAX_VAL;
    }
    if (value < CANSIG_BMS_STATE_MIN_VAL)
    {
        value = CANSIG_BMS_STATE_MIN_VAL;
    }
}
void app_canTx_BMS_TxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_TX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_TX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_TX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_BMS_RxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_RX_OVERFLOW_COUNT_MAX_VAL)
    {
        value = CANSIG_BMS_RX_OVERFLOW_COUNT_MAX_VAL;
    }
    if (value < CANSIG_BMS_RX_OVERFLOW_COUNT_MIN_VAL)
    {
        value = CANSIG_BMS_RX_OVERFLOW_COUNT_MIN_VAL;
    }
}
void app_canTx_BMS_WatchdogTimeoutTaskName_set(RtosTaskName value)
{
    tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX_VAL)
    {
        value = CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX_VAL;
    }
    if (value < CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MIN_VAL)
    {
        value = CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MIN_VAL;
    }
}
void app_canTx_BMS_ModuleCommunication_NumCommTries_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_NumCommTries_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MAX_VAL)
    {
        value = CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MAX_VAL;
    }
    if (value < CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MIN_VAL)
    {
        value = CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MIN_VAL;
    }
}
void app_canTx_BMS_ModuleCommunication_MonitorState_set(CAN_AccumulatorMonitorState value)
{
    tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_MonitorState_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MAX_VAL)
    {
        value = CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MIN_VAL)
    {
        value = CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MIN_VAL;
    }
}
void app_canTx_BMS_ImdOkHs_set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_OK_HS_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_OK_HS_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_OK_HS_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_OK_HS_MIN_VAL;
    }
}
void app_canTx_BMS_ImdTimeSincePowerOn_set(uint32_t value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdTimeSincePowerOn_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MIN_VAL;
    }
}
void app_canTx_BMS_ImdValidDutyCycle_set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MIN_VAL;
    }
}
void app_canTx_BMS_ImdCondition_set(ImdConditionName value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_CONDITION_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_CONDITION_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_CONDITION_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_CONDITION_MIN_VAL;
    }
}
void app_canTx_BMS_ImdDutyCycle_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_ImdStatus_signals.BMS_ImdDutyCycle_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_DUTY_CYCLE_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_DUTY_CYCLE_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_DUTY_CYCLE_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_DUTY_CYCLE_MIN_VAL;
    }
}
void app_canTx_BMS_ImdFrequency_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_ImdStatus_signals.BMS_ImdFrequency_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_FREQUENCY_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_FREQUENCY_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_FREQUENCY_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_FREQUENCY_MIN_VAL;
    }
}
void app_canTx_BMS_ImdActiveFrequency_set(ImdActiveFrequency value)
{
    tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MIN_VAL;
    }
}
void app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN_VAL;
    }
}
void app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN_VAL;
    }
}
void app_canTx_BMS_ImdSpeedStartStatus30Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MIN_VAL;
    }
}
void app_canTx_BMS_ChargerConnected_set(bool value)
{
    tx_table.BMS_Charger_signals.BMS_ChargerConnected_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CHARGER_CONNECTED_MAX_VAL)
    {
        value = CANSIG_BMS_CHARGER_CONNECTED_MAX_VAL;
    }
    if (value < CANSIG_BMS_CHARGER_CONNECTED_MIN_VAL)
    {
        value = CANSIG_BMS_CHARGER_CONNECTED_MIN_VAL;
    }
}
void app_canTx_BMS_ChargerEnable_set(bool value)
{
    tx_table.BMS_BrusaControls_signals.BMS_ChargerEnable_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CHARGER_ENABLE_MAX_VAL)
    {
        value = CANSIG_BMS_CHARGER_ENABLE_MAX_VAL;
    }
    if (value < CANSIG_BMS_CHARGER_ENABLE_MIN_VAL)
    {
        value = CANSIG_BMS_CHARGER_ENABLE_MIN_VAL;
    }
}
void app_canTx_BMS_ClearLatch_set(bool value)
{
    tx_table.BMS_BrusaControls_signals.BMS_ClearLatch_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CLEAR_LATCH_MAX_VAL)
    {
        value = CANSIG_BMS_CLEAR_LATCH_MAX_VAL;
    }
    if (value < CANSIG_BMS_CLEAR_LATCH_MIN_VAL)
    {
        value = CANSIG_BMS_CLEAR_LATCH_MIN_VAL;
    }
}
void app_canTx_BMS_MaxMainsCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_BrusaControls_signals.BMS_MaxMainsCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_MAINS_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_MAINS_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_MAINS_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_MAINS_CURRENT_MIN_VAL;
    }
}
void app_canTx_BMS_ChargingVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_BrusaControls_signals.BMS_ChargingVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CHARGING_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_CHARGING_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_CHARGING_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_CHARGING_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_BMS_ChargingCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_BrusaControls_signals.BMS_ChargingCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CHARGING_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_CHARGING_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_CHARGING_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_CHARGING_CURRENT_MIN_VAL;
    }
}
void app_canTx_BMS_BmsOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_OK_MIN_VAL;
    }
}
void app_canTx_BMS_ImdOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_OK_MIN_VAL;
    }
}
void app_canTx_BMS_BspdOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_OK_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_OK_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_OK_MIN_VAL;
    }
}
void app_canTx_BMS_BmsLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canTx_BMS_ImdLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canTx_BMS_BspdLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL;
    }
}
void app_canTx_BMS_AirPositive_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirPositive_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AIR_POSITIVE_MAX_VAL)
    {
        value = CANSIG_BMS_AIR_POSITIVE_MAX_VAL;
    }
    if (value < CANSIG_BMS_AIR_POSITIVE_MIN_VAL)
    {
        value = CANSIG_BMS_AIR_POSITIVE_MIN_VAL;
    }
}
void app_canTx_BMS_AirNegative_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirNegative_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AIR_NEGATIVE_MAX_VAL)
    {
        value = CANSIG_BMS_AIR_NEGATIVE_MAX_VAL;
    }
    if (value < CANSIG_BMS_AIR_NEGATIVE_MIN_VAL)
    {
        value = CANSIG_BMS_AIR_NEGATIVE_MIN_VAL;
    }
}
void app_canTx_BMS_PrechargeRelay_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_PRECHARGE_RELAY_MAX_VAL)
    {
        value = CANSIG_BMS_PRECHARGE_RELAY_MAX_VAL;
    }
    if (value < CANSIG_BMS_PRECHARGE_RELAY_MIN_VAL)
    {
        value = CANSIG_BMS_PRECHARGE_RELAY_MIN_VAL;
    }
}
void app_canTx_BMS_MinCellTemperature_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL;
    }
}
void app_canTx_BMS_MaxCellTemperature_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL;
    }
}
void app_canTx_BMS_PackVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_PACK_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_PACK_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_PACK_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_BMS_Soc_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_MIN_VAL;
    }
}
void app_canTx_BMS_SocCorrupt_set(bool value)
{
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SOC_CORRUPT_MAX_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MAX_VAL;
    }
    if (value < CANSIG_BMS_SOC_CORRUPT_MIN_VAL)
    {
        value = CANSIG_BMS_SOC_CORRUPT_MIN_VAL;
    }
}
void app_canTx_BMS_MinCellVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_BMS_MaxCellVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_BMS_MinCellVoltageSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MIN_VAL;
    }
}
void app_canTx_BMS_MinCellVoltageIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MIN_VAL;
    }
}
void app_canTx_BMS_MaxCellVoltageSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MIN_VAL;
    }
}
void app_canTx_BMS_MaxCellVoltageIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MIN_VAL;
    }
}
void app_canTx_BMS_MinTempSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_TEMP_SEGMENT_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_TEMP_SEGMENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_TEMP_SEGMENT_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_TEMP_SEGMENT_MIN_VAL;
    }
}
void app_canTx_BMS_MinTempIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MIN_TEMP_IDX_MAX_VAL)
    {
        value = CANSIG_BMS_MIN_TEMP_IDX_MAX_VAL;
    }
    if (value < CANSIG_BMS_MIN_TEMP_IDX_MIN_VAL)
    {
        value = CANSIG_BMS_MIN_TEMP_IDX_MIN_VAL;
    }
}
void app_canTx_BMS_MaxTempSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_TEMP_SEGMENT_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_TEMP_SEGMENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_TEMP_SEGMENT_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_TEMP_SEGMENT_MIN_VAL;
    }
}
void app_canTx_BMS_MaxTempIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_MAX_TEMP_IDX_MAX_VAL)
    {
        value = CANSIG_BMS_MAX_TEMP_IDX_MAX_VAL;
    }
    if (value < CANSIG_BMS_MAX_TEMP_IDX_MIN_VAL)
    {
        value = CANSIG_BMS_MAX_TEMP_IDX_MIN_VAL;
    }
}
void app_canTx_BMS_TractiveSystemVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL;
    }
}
void app_canTx_BMS_TractiveSystemCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL;
    }
}
void app_canTx_BMS_TractiveSystemPower_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL;
    }
}
void app_canTx_BMS_AvailablePower_set(uint32_t value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_AVAILABLE_POWER_MAX_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MAX_VAL;
    }
    if (value < CANSIG_BMS_AVAILABLE_POWER_MIN_VAL)
    {
        value = CANSIG_BMS_AVAILABLE_POWER_MIN_VAL;
    }
}
void app_canTx_BMS_BSPDCurrentThresholdExceeded_set(bool value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL;
    }
    if (value < CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL)
    {
        value = CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL;
    }
}
void app_canTx_BMS_Hash_set(uint32_t value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Hash_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HASH_MAX_VAL)
    {
        value = CANSIG_BMS_HASH_MAX_VAL;
    }
    if (value < CANSIG_BMS_HASH_MIN_VAL)
    {
        value = CANSIG_BMS_HASH_MIN_VAL;
    }
}
void app_canTx_BMS_Clean_set(bool value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Clean_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_CLEAN_MAX_VAL)
    {
        value = CANSIG_BMS_CLEAN_MAX_VAL;
    }
    if (value < CANSIG_BMS_CLEAN_MIN_VAL)
    {
        value = CANSIG_BMS_CLEAN_MIN_VAL;
    }
}
void app_canTx_BMS_Segment0_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment0_OWC_Cells_Status_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_Segment1_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment1_OWC_Cells_Status_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_Segment2_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment2_OWC_Cells_Status_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_Segment3_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment3_OWC_Cells_Status_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_Segment4_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment4_OWC_Cells_Status_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_HVDShdnOKStatus_set(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL;
    }
}
void app_canTx_BMS_TSIlckOKStatus_set(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value = value; // Set value
    // Clamp value to min/max
    if (value > CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL;
    }
    if (value < CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL)
    {
        value = CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL;
    }
}
bool app_canTx_BMS_Vitals_BMS_Heartbeat_get(void)
{
    return tx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}
BmsState app_canTx_BMS_Vitals_BMS_State_get(void)
{
    return tx_table.BMS_Vitals_signals.BMS_State_value;
}
uint32_t app_canTx_BMS_AlertsContext_BMS_TxOverflowCount_get(void)
{
    return tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value;
}
uint32_t app_canTx_BMS_AlertsContext_BMS_RxOverflowCount_get(void)
{
    return tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value;
}
RtosTaskName app_canTx_BMS_AlertsContext_BMS_WatchdogTimeoutTaskName_get(void)
{
    return tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value;
}
uint32_t app_canTx_BMS_AlertsContext_BMS_ModuleCommunication_NumCommTries_get(void)
{
    return tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_NumCommTries_value;
}
CAN_AccumulatorMonitorState app_canTx_BMS_AlertsContext_BMS_ModuleCommunication_MonitorState_get(void)
{
    return tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_MonitorState_value;
}
bool app_canTx_BMS_ImdStatus_BMS_ImdOkHs_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value;
}
uint32_t app_canTx_BMS_ImdStatus_BMS_ImdTimeSincePowerOn_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdTimeSincePowerOn_value;
}
bool app_canTx_BMS_ImdStatus_BMS_ImdValidDutyCycle_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value;
}
ImdConditionName app_canTx_BMS_ImdStatus_BMS_ImdCondition_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value;
}
float app_canTx_BMS_ImdStatus_BMS_ImdDutyCycle_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdDutyCycle_value;
}
float app_canTx_BMS_ImdStatus_BMS_ImdFrequency_get(void)
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdFrequency_value;
}
ImdActiveFrequency app_canTx_BMS_ImdData_BMS_ImdActiveFrequency_get(void)
{
    return tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value;
}
float app_canTx_BMS_ImdData_BMS_ImdInsulationMeasurementDcp10Hz_get(void)
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value;
}
float app_canTx_BMS_ImdData_BMS_ImdInsulationMeasurementDcp20Hz_get(void)
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value;
}
float app_canTx_BMS_ImdData_BMS_ImdSpeedStartStatus30Hz_get(void)
{
    return tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value;
}
bool app_canTx_BMS_Charger_BMS_ChargerConnected_get(void)
{
    return tx_table.BMS_Charger_signals.BMS_ChargerConnected_value;
}
bool app_canTx_BMS_BrusaControls_BMS_ChargerEnable_get(void)
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargerEnable_value;
}
bool app_canTx_BMS_BrusaControls_BMS_ClearLatch_get(void)
{
    return tx_table.BMS_BrusaControls_signals.BMS_ClearLatch_value;
}
float app_canTx_BMS_BrusaControls_BMS_MaxMainsCurrent_get(void)
{
    return tx_table.BMS_BrusaControls_signals.BMS_MaxMainsCurrent_value;
}
float app_canTx_BMS_BrusaControls_BMS_ChargingVoltage_get(void)
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargingVoltage_value;
}
float app_canTx_BMS_BrusaControls_BMS_ChargingCurrent_get(void)
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargingCurrent_value;
}
bool app_canTx_BMS_OkStatuses_BMS_BmsOk_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}
bool app_canTx_BMS_OkStatuses_BMS_ImdOk_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}
bool app_canTx_BMS_OkStatuses_BMS_BspdOk_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}
bool app_canTx_BMS_OkStatuses_BMS_BmsLatchedFault_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}
bool app_canTx_BMS_OkStatuses_BMS_ImdLatchedFault_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}
bool app_canTx_BMS_OkStatuses_BMS_BspdLatchedFault_get(void)
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
}
ContactorState app_canTx_BMS_Contactors_BMS_AirPositive_get(void)
{
    return tx_table.BMS_Contactors_signals.BMS_AirPositive_value;
}
ContactorState app_canTx_BMS_Contactors_BMS_AirNegative_get(void)
{
    return tx_table.BMS_Contactors_signals.BMS_AirNegative_value;
}
ContactorState app_canTx_BMS_Contactors_BMS_PrechargeRelay_get(void)
{
    return tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value;
}
float app_canTx_BMS_CellTemperatures_BMS_MinCellTemperature_get(void)
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value;
}
float app_canTx_BMS_CellTemperatures_BMS_MaxCellTemperature_get(void)
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value;
}
float app_canTx_BMS_VoltageAndChargeStats_BMS_PackVoltage_get(void)
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value;
}
float app_canTx_BMS_VoltageAndChargeStats_BMS_Soc_get(void)
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value;
}
bool app_canTx_BMS_VoltageAndChargeStats_BMS_SocCorrupt_get(void)
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value;
}
float app_canTx_BMS_VoltageAndChargeStats_BMS_MinCellVoltage_get(void)
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value;
}
float app_canTx_BMS_VoltageAndChargeStats_BMS_MaxCellVoltage_get(void)
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MinCellVoltageSegment_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MinCellVoltageIdx_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MaxCellVoltageSegment_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MaxCellVoltageIdx_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MinTempSegment_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MinTempIdx_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MaxTempSegment_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value;
}
uint32_t app_canTx_BMS_CellStats_BMS_MaxTempIdx_get(void)
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value;
}
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemVoltage_get(void)
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemCurrent_get(void)
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}
float app_canTx_BMS_TractiveSystem_BMS_TractiveSystemPower_get(void)
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value;
}
uint32_t app_canTx_BMS_TractiveSystem_BMS_AvailablePower_get(void)
{
    return tx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value;
}
bool app_canTx_BMS_TractiveSystem_BMS_BSPDCurrentThresholdExceeded_get(void)
{
    return tx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value;
}
uint32_t app_canTx_BMS_CommitInfo_BMS_Hash_get(void)
{
    return tx_table.BMS_CommitInfo_signals.BMS_Hash_value;
}
bool app_canTx_BMS_CommitInfo_BMS_Clean_get(void)
{
    return tx_table.BMS_CommitInfo_signals.BMS_Clean_value;
}
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment0_OWC_Cells_Status_get(void)
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment0_OWC_Cells_Status_value;
}
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment1_OWC_Cells_Status_get(void)
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment1_OWC_Cells_Status_value;
}
uint32_t app_canTx_BMS_OWC_Segment0to2_Status_BMS_Segment2_OWC_Cells_Status_get(void)
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment2_OWC_Cells_Status_value;
}
uint32_t app_canTx_BMS_OWC_Segment3to4_Status_BMS_Segment3_OWC_Cells_Status_get(void)
{
    return tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment3_OWC_Cells_Status_value;
}
uint32_t app_canTx_BMS_OWC_Segment3to4_Status_BMS_Segment4_OWC_Cells_Status_get(void)
{
    return tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment4_OWC_Cells_Status_value;
}
bool app_canTx_BMS_BMSShdnNodeStatus_BMS_HVDShdnOKStatus_get(void)
{
    return tx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value;
}
bool app_canTx_BMS_BMSShdnNodeStatus_BMS_TSIlckOKStatus_get(void)
{
    return tx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value;
}
BMS_Vitals_Signals* app_canTx_BMS_Vitals_getData()
{
    return &tx_table.BMS_Vitals_signals;
}
BMS_AlertsContext_Signals* app_canTx_BMS_AlertsContext_getData()
{
    return &tx_table.BMS_AlertsContext_signals;
}
BMS_ImdStatus_Signals* app_canTx_BMS_ImdStatus_getData()
{
    return &tx_table.BMS_ImdStatus_signals;
}
BMS_ImdData_Signals* app_canTx_BMS_ImdData_getData()
{
    return &tx_table.BMS_ImdData_signals;
}
BMS_Charger_Signals* app_canTx_BMS_Charger_getData()
{
    return &tx_table.BMS_Charger_signals;
}
BMS_BrusaControls_Signals* app_canTx_BMS_BrusaControls_getData()
{
    return &tx_table.BMS_BrusaControls_signals;
}
BMS_OkStatuses_Signals* app_canTx_BMS_OkStatuses_getData()
{
    return &tx_table.BMS_OkStatuses_signals;
}
BMS_Contactors_Signals* app_canTx_BMS_Contactors_getData()
{
    return &tx_table.BMS_Contactors_signals;
}
BMS_CellTemperatures_Signals* app_canTx_BMS_CellTemperatures_getData()
{
    return &tx_table.BMS_CellTemperatures_signals;
}
BMS_VoltageAndChargeStats_Signals* app_canTx_BMS_VoltageAndChargeStats_getData()
{
    return &tx_table.BMS_VoltageAndChargeStats_signals;
}
BMS_CellStats_Signals* app_canTx_BMS_CellStats_getData()
{
    return &tx_table.BMS_CellStats_signals;
}
BMS_TractiveSystem_Signals* app_canTx_BMS_TractiveSystem_getData()
{
    return &tx_table.BMS_TractiveSystem_signals;
}
BMS_CommitInfo_Signals* app_canTx_BMS_CommitInfo_getData()
{
    return &tx_table.BMS_CommitInfo_signals;
}
BMS_OWC_Segment0to2_Status_Signals* app_canTx_BMS_OWC_Segment0to2_Status_getData()
{
    return &tx_table.BMS_OWC_Segment0to2_Status_signals;
}
BMS_OWC_Segment3to4_Status_Signals* app_canTx_BMS_OWC_Segment3to4_Status_getData()
{
    return &tx_table.BMS_OWC_Segment3to4_Status_signals;
}
BMS_BMSShdnNodeStatus_Signals* app_canTx_BMS_BMSShdnNodeStatus_getData()
{
    return &tx_table.BMS_BMSShdnNodeStatus_signals;
}