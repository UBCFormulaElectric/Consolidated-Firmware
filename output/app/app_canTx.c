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
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    BMS_WarningsCounts_Signals BMS_WarningsCounts_signals;
    BMS_FaultsCounts_Signals BMS_FaultsCounts_signals;
} BMS_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static BMS_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canTx_init()
{
    memset(&tx_table, 0, sizeof(BMS_TxMsgs));
    app_canTx_BMS_Heartbeat_set(CANSIG_BMS_VITALS_BMS_HEARTBEAT_START_VAL);
    app_canTx_BMS_State_set(CANSIG_BMS_VITALS_BMS_STATE_START_VAL);
    app_canTx_BMS_TxOverflowCount_set(CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_RxOverflowCount_set(CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_WatchdogTimeoutTaskName_set(CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    app_canTx_BMS_ModuleCommunication_NumCommTries_set(CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_START_VAL);
    app_canTx_BMS_ModuleCommunication_MonitorState_set(CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_MONITOR_STATE_START_VAL);
    app_canTx_BMS_ImdOkHs_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_OK_HS_START_VAL);
    app_canTx_BMS_ImdTimeSincePowerOn_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_TIME_SINCE_POWER_ON_START_VAL);
    app_canTx_BMS_ImdValidDutyCycle_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_VALID_DUTY_CYCLE_START_VAL);
    app_canTx_BMS_ImdCondition_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_START_VAL);
    app_canTx_BMS_ImdDutyCycle_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_DUTY_CYCLE_START_VAL);
    app_canTx_BMS_ImdFrequency_set(CANSIG_BMS_IMD_STATUS_BMS_IMD_FREQUENCY_START_VAL);
    app_canTx_BMS_ImdActiveFrequency_set(CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_START_VAL);
    app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_START_VAL);
    app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_START_VAL);
    app_canTx_BMS_ImdSpeedStartStatus30Hz_set(CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_START_VAL);
    app_canTx_BMS_ChargerConnected_set(CANSIG_BMS_CHARGER_BMS_CHARGER_CONNECTED_START_VAL);
    app_canTx_BMS_ChargerEnable_set(CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGER_ENABLE_START_VAL);
    app_canTx_BMS_ClearLatch_set(CANSIG_BMS_BRUSA_CONTROLS_BMS_CLEAR_LATCH_START_VAL);
    app_canTx_BMS_MaxMainsCurrent_set(CANSIG_BMS_BRUSA_CONTROLS_BMS_MAX_MAINS_CURRENT_START_VAL);
    app_canTx_BMS_ChargingVoltage_set(CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_VOLTAGE_START_VAL);
    app_canTx_BMS_ChargingCurrent_set(CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_CURRENT_START_VAL);
    app_canTx_BMS_BmsOk_set(CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_START_VAL);
    app_canTx_BMS_ImdOk_set(CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_START_VAL);
    app_canTx_BMS_BspdOk_set(CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_START_VAL);
    app_canTx_BMS_BmsLatchedFault_set(CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_ImdLatchedFault_set(CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_BspdLatchedFault_set(CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_START_VAL);
    app_canTx_BMS_AirPositive_set(CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_START_VAL);
    app_canTx_BMS_AirNegative_set(CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_START_VAL);
    app_canTx_BMS_PrechargeRelay_set(CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_START_VAL);
    app_canTx_BMS_MinCellTemperature_set(CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_START_VAL);
    app_canTx_BMS_MaxCellTemperature_set(CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_START_VAL);
    app_canTx_BMS_PackVoltage_set(CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_PACK_VOLTAGE_START_VAL);
    app_canTx_BMS_Soc_set(CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_START_VAL);
    app_canTx_BMS_SocCorrupt_set(CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_CORRUPT_START_VAL);
    app_canTx_BMS_MinCellVoltage_set(CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MIN_CELL_VOLTAGE_START_VAL);
    app_canTx_BMS_MaxCellVoltage_set(CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MAX_CELL_VOLTAGE_START_VAL);
    app_canTx_BMS_MinCellVoltageSegment_set(CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_START_VAL);
    app_canTx_BMS_MinCellVoltageIdx_set(CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_START_VAL);
    app_canTx_BMS_MaxCellVoltageSegment_set(CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_START_VAL);
    app_canTx_BMS_MaxCellVoltageIdx_set(CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_START_VAL);
    app_canTx_BMS_MinTempSegment_set(CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_START_VAL);
    app_canTx_BMS_MinTempIdx_set(CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_START_VAL);
    app_canTx_BMS_MaxTempSegment_set(CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_START_VAL);
    app_canTx_BMS_MaxTempIdx_set(CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_START_VAL);
    app_canTx_BMS_TractiveSystemVoltage_set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL);
    app_canTx_BMS_TractiveSystemCurrent_set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL);
    app_canTx_BMS_TractiveSystemPower_set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_POWER_START_VAL);
    app_canTx_BMS_AvailablePower_set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_AVAILABLE_POWER_START_VAL);
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(CANSIG_BMS_TRACTIVE_SYSTEM_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL);
    app_canTx_BMS_Hash_set(CANSIG_BMS_COMMIT_INFO_BMS_HASH_START_VAL);
    app_canTx_BMS_Clean_set(CANSIG_BMS_COMMIT_INFO_BMS_CLEAN_START_VAL);
    app_canTx_BMS_Segment0_OWC_Cells_Status_set(CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT0_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment1_OWC_Cells_Status_set(CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT1_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment2_OWC_Cells_Status_set(CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT2_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment3_OWC_Cells_Status_set(CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT3_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_Segment4_OWC_Cells_Status_set(CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT4_OWC_CELLS_STATUS_START_VAL);
    app_canTx_BMS_HVDShdnOKStatus_set(CANSIG_BMS_BMS_SHDN_NODE_STATUS_BMS_HVD_SHDN_OK_STATUS_START_VAL);
    app_canTx_BMS_TSIlckOKStatus_set(CANSIG_BMS_BMS_SHDN_NODE_STATUS_BMS_TS_ILCK_OK_STATUS_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_set(CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canTx_BMS_Warning_WatchdogTimeout_set(CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canTx_BMS_Warning_TxOverflow_set(CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_START_VAL);
    app_canTx_BMS_Warning_RxOverflow_set(CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheckFault_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_set(CANSIG_BMS_WARNINGS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_START_VAL);
    app_canTx_BMS_Warning_MissingVCHeartbeat_set(CANSIG_BMS_WARNINGS_BMS_WARNING_MISSING_VC_HEARTBEAT_START_VAL);
    app_canTx_BMS_Fault_StateMachine_set(CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_START_VAL);
    app_canTx_BMS_Fault_CellUndervoltage_set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_START_VAL);
    app_canTx_BMS_Fault_CellOvervoltage_set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_START_VAL);
    app_canTx_BMS_Fault_ModuleCommunicationError_set(CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_START_VAL);
    app_canTx_BMS_Fault_CellUndertemp_set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_START_VAL);
    app_canTx_BMS_Fault_CellOvertemp_set(CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_START_VAL);
    app_canTx_BMS_Fault_ChargerReportedError_set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_REPORTED_ERROR_START_VAL);
    app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_START_VAL);
    app_canTx_BMS_Fault_ChargerShutdownLoopOpen_set(CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_START_VAL);
    app_canTx_BMS_Fault_TractiveSystemOvercurrent_set(CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_START_VAL);
    app_canTx_BMS_Fault_PrechargeFailure_set(CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    app_canTx_BMS_Warning_WatchdogTimeoutCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    app_canTx_BMS_Warning_TxOverflowCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_Warning_RxOverflowCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheckFaultCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_START_VAL);
    app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_START_VAL);
    app_canTx_BMS_Warning_MissingVCHeartbeatCount_set(CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_START_VAL);
    app_canTx_BMS_Fault_StateMachineCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_START_VAL);
    app_canTx_BMS_Fault_CellUndervoltageCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_START_VAL);
    app_canTx_BMS_Fault_CellOvervoltageCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_START_VAL);
    app_canTx_BMS_Fault_ModuleCommunicationErrorCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_START_VAL);
    app_canTx_BMS_Fault_CellUndertempCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_START_VAL);
    app_canTx_BMS_Fault_CellOvertempCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_START_VAL);
    app_canTx_BMS_Fault_ChargerReportedErrorCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_START_VAL);
    app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_START_VAL);
    app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_START_VAL);
    app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_START_VAL);
    app_canTx_BMS_Fault_PrechargeFailureCount_set(CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_START_VAL);
}

void app_canTx_BMS_Heartbeat_set(bool value)
{
    tx_table.BMS_Vitals_signals.BMS_Heartbeat_value = value;
}

void app_canTx_BMS_State_set(BmsState value)
{
    tx_table.BMS_Vitals_signals.BMS_State_value = (value > CANSIG_BMS_VITALS_BMS_STATE_MAX) ? CANSIG_BMS_VITALS_BMS_STATE_MAX : value;
}

void app_canTx_BMS_TxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_MAX : value;
}

void app_canTx_BMS_RxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_MAX : value;
}

void app_canTx_BMS_WatchdogTimeoutTaskName_set(RtosTaskName value)
{
    tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void app_canTx_BMS_ModuleCommunication_NumCommTries_set(uint32_t value)
{
    tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_NumCommTries_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MAX : value;
}

void app_canTx_BMS_ModuleCommunication_MonitorState_set(CAN_AccumulatorMonitorState value)
{
    tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_MonitorState_value = (value > CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MAX) ? CANSIG_BMS_ALERTS_CONTEXT_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MAX : value;
}

void app_canTx_BMS_ImdOkHs_set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value = value;
}

void app_canTx_BMS_ImdTimeSincePowerOn_set(uint32_t value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdTimeSincePowerOn_value = (value > CANSIG_BMS_IMD_STATUS_BMS_IMD_TIME_SINCE_POWER_ON_MAX) ? CANSIG_BMS_IMD_STATUS_BMS_IMD_TIME_SINCE_POWER_ON_MAX : value;
}

void app_canTx_BMS_ImdValidDutyCycle_set(bool value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value = value;
}

void app_canTx_BMS_ImdCondition_set(ImdConditionName value)
{
    tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value = (value > CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_MAX) ? CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_MAX : value;
}

void app_canTx_BMS_ImdDutyCycle_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_STATUS_BMS_IMD_DUTY_CYCLE_MIN ? CANSIG_BMS_IMD_STATUS_BMS_IMD_DUTY_CYCLE_MIN : value;
    tx_table.BMS_ImdStatus_signals.BMS_ImdDutyCycle_value = tmp > CANSIG_BMS_IMD_STATUS_BMS_IMD_DUTY_CYCLE_MAX ? CANSIG_BMS_IMD_STATUS_BMS_IMD_DUTY_CYCLE_MAX : tmp;
}

void app_canTx_BMS_ImdFrequency_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_STATUS_BMS_IMD_FREQUENCY_MIN ? CANSIG_BMS_IMD_STATUS_BMS_IMD_FREQUENCY_MIN : value;
    tx_table.BMS_ImdStatus_signals.BMS_ImdFrequency_value = tmp > CANSIG_BMS_IMD_STATUS_BMS_IMD_FREQUENCY_MAX ? CANSIG_BMS_IMD_STATUS_BMS_IMD_FREQUENCY_MAX : tmp;
}

void app_canTx_BMS_ImdActiveFrequency_set(ImdActiveFrequency value)
{
    tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value = (value > CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_MAX) ? CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_MAX : value;
}

void app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX : tmp;
}

void app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX : tmp;
}

void app_canTx_BMS_ImdSpeedStartStatus30Hz_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MIN ? CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MIN : value;
    tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value = tmp > CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MAX ? CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_MAX : tmp;
}

void app_canTx_BMS_ChargerConnected_set(bool value)
{
    tx_table.BMS_Charger_signals.BMS_ChargerConnected_value = value;
}

void app_canTx_BMS_ChargerEnable_set(bool value)
{
    tx_table.BMS_BrusaControls_signals.BMS_ChargerEnable_value = value;
}

void app_canTx_BMS_ClearLatch_set(bool value)
{
    tx_table.BMS_BrusaControls_signals.BMS_ClearLatch_value = value;
}

void app_canTx_BMS_MaxMainsCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_BRUSA_CONTROLS_BMS_MAX_MAINS_CURRENT_MIN ? CANSIG_BMS_BRUSA_CONTROLS_BMS_MAX_MAINS_CURRENT_MIN : value;
    tx_table.BMS_BrusaControls_signals.BMS_MaxMainsCurrent_value = tmp > CANSIG_BMS_BRUSA_CONTROLS_BMS_MAX_MAINS_CURRENT_MAX ? CANSIG_BMS_BRUSA_CONTROLS_BMS_MAX_MAINS_CURRENT_MAX : tmp;
}

void app_canTx_BMS_ChargingVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_VOLTAGE_MIN ? CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_VOLTAGE_MIN : value;
    tx_table.BMS_BrusaControls_signals.BMS_ChargingVoltage_value = tmp > CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_VOLTAGE_MAX ? CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_VOLTAGE_MAX : tmp;
}

void app_canTx_BMS_ChargingCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_CURRENT_MIN ? CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_CURRENT_MIN : value;
    tx_table.BMS_BrusaControls_signals.BMS_ChargingCurrent_value = tmp > CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_CURRENT_MAX ? CANSIG_BMS_BRUSA_CONTROLS_BMS_CHARGING_CURRENT_MAX : tmp;
}

void app_canTx_BMS_BmsOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value = value;
}

void app_canTx_BMS_ImdOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value = value;
}

void app_canTx_BMS_BspdOk_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value = value;
}

void app_canTx_BMS_BmsLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value = value;
}

void app_canTx_BMS_ImdLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value = value;
}

void app_canTx_BMS_BspdLatchedFault_set(bool value)
{
    tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value = value;
}

void app_canTx_BMS_AirPositive_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirPositive_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_MAX : value;
}

void app_canTx_BMS_AirNegative_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_AirNegative_value = (value > CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX) ? CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_MAX : value;
}

void app_canTx_BMS_PrechargeRelay_set(ContactorState value)
{
    tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value = (value > CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX) ? CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_MAX : value;
}

void app_canTx_BMS_MinCellTemperature_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MIN ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MIN : value;
    tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value = tmp > CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MAX ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_MAX : tmp;
}

void app_canTx_BMS_MaxCellTemperature_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MIN ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MIN : value;
    tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value = tmp > CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MAX ? CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_MAX : tmp;
}

void app_canTx_BMS_PackVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_PACK_VOLTAGE_MIN ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_PACK_VOLTAGE_MIN : value;
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value = tmp > CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_PACK_VOLTAGE_MAX ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_PACK_VOLTAGE_MAX : tmp;
}

void app_canTx_BMS_Soc_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_MIN ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_MIN : value;
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value = tmp > CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_MAX ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_SOC_MAX : tmp;
}

void app_canTx_BMS_SocCorrupt_set(bool value)
{
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value = value;
}

void app_canTx_BMS_MinCellVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MIN_CELL_VOLTAGE_MIN ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MIN_CELL_VOLTAGE_MIN : value;
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value = tmp > CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MIN_CELL_VOLTAGE_MAX ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MIN_CELL_VOLTAGE_MAX : tmp;
}

void app_canTx_BMS_MaxCellVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MAX_CELL_VOLTAGE_MIN ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MAX_CELL_VOLTAGE_MIN : value;
    tx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value = tmp > CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MAX_CELL_VOLTAGE_MAX ? CANSIG_BMS_VOLTAGE_AND_CHARGE_STATS_BMS_MAX_CELL_VOLTAGE_MAX : tmp;
}

void app_canTx_BMS_MinCellVoltageSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX : value;
}

void app_canTx_BMS_MinCellVoltageIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_MAX : value;
}

void app_canTx_BMS_MaxCellVoltageSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX : value;
}

void app_canTx_BMS_MaxCellVoltageIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_MAX : value;
}

void app_canTx_BMS_MinTempSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_MAX : value;
}

void app_canTx_BMS_MinTempIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_MAX : value;
}

void app_canTx_BMS_MaxTempSegment_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_MAX : value;
}

void app_canTx_BMS_MaxTempIdx_set(uint32_t value)
{
    tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value = (value > CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_MAX) ? CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_MAX : value;
}

void app_canTx_BMS_TractiveSystemVoltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN : value;
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX : tmp;
}

void app_canTx_BMS_TractiveSystemCurrent_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MIN : value;
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_MAX : tmp;
}

void app_canTx_BMS_TractiveSystemPower_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_POWER_MIN ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_POWER_MIN : value;
    tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value = tmp > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_POWER_MAX ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_POWER_MAX : tmp;
}

void app_canTx_BMS_AvailablePower_set(uint32_t value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value = (value > CANSIG_BMS_TRACTIVE_SYSTEM_BMS_AVAILABLE_POWER_MAX) ? CANSIG_BMS_TRACTIVE_SYSTEM_BMS_AVAILABLE_POWER_MAX : value;
}

void app_canTx_BMS_BSPDCurrentThresholdExceeded_set(bool value)
{
    tx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value = value;
}

void app_canTx_BMS_Hash_set(uint32_t value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Hash_value = value;
}

void app_canTx_BMS_Clean_set(bool value)
{
    tx_table.BMS_CommitInfo_signals.BMS_Clean_value = value;
}

void app_canTx_BMS_Segment0_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment0_OWC_Cells_Status_value = (value > CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT0_OWC_CELLS_STATUS_MAX) ? CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT0_OWC_CELLS_STATUS_MAX : value;
}

void app_canTx_BMS_Segment1_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment1_OWC_Cells_Status_value = (value > CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT1_OWC_CELLS_STATUS_MAX) ? CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT1_OWC_CELLS_STATUS_MAX : value;
}

void app_canTx_BMS_Segment2_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment2_OWC_Cells_Status_value = (value > CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT2_OWC_CELLS_STATUS_MAX) ? CANSIG_BMS_OWC_SEGMENT0TO2_STATUS_BMS_SEGMENT2_OWC_CELLS_STATUS_MAX : value;
}

void app_canTx_BMS_Segment3_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment3_OWC_Cells_Status_value = (value > CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT3_OWC_CELLS_STATUS_MAX) ? CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT3_OWC_CELLS_STATUS_MAX : value;
}

void app_canTx_BMS_Segment4_OWC_Cells_Status_set(uint32_t value)
{
    tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment4_OWC_Cells_Status_value = (value > CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT4_OWC_CELLS_STATUS_MAX) ? CANSIG_BMS_OWC_SEGMENT3TO4_STATUS_BMS_SEGMENT4_OWC_CELLS_STATUS_MAX : value;
}

void app_canTx_BMS_HVDShdnOKStatus_set(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value = value;
}

void app_canTx_BMS_TSIlckOKStatus_set(bool value)
{
    tx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void app_canTx_BMS_Warning_WatchdogTimeout_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value = value;
}

void app_canTx_BMS_Warning_TxOverflow_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value = value;
}

void app_canTx_BMS_Warning_RxOverflow_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheckFault_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheckFault_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment0_GND_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment1_GND_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment2_GND_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment3_GND_value = value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment4_GND_value = value;
}

void app_canTx_BMS_Warning_MissingVCHeartbeat_set(bool value)
{
    tx_table.BMS_Warnings_signals.BMS_Warning_MissingVCHeartbeat_value = value;
}

void app_canTx_BMS_Fault_StateMachine_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value = value;
}

void app_canTx_BMS_Fault_CellUndervoltage_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value = value;
}

void app_canTx_BMS_Fault_CellOvervoltage_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value = value;
}

void app_canTx_BMS_Fault_ModuleCommunicationError_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value = value;
}

void app_canTx_BMS_Fault_CellUndertemp_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value = value;
}

void app_canTx_BMS_Fault_CellOvertemp_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value = value;
}

void app_canTx_BMS_Fault_ChargerReportedError_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ChargerReportedError_value = value;
}

void app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value = value;
}

void app_canTx_BMS_Fault_ChargerShutdownLoopOpen_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_ChargerShutdownLoopOpen_value = value;
}

void app_canTx_BMS_Fault_TractiveSystemOvercurrent_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value = value;
}

void app_canTx_BMS_Fault_PrechargeFailure_set(bool value)
{
    tx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value = value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_WatchdogTimeoutCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_TxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_RxOverflowCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheckFaultCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheckFaultCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment0_GNDCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment1_GNDCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment2_GNDCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment3_GNDCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment4_GNDCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_MAX : value;
}

void app_canTx_BMS_Warning_MissingVCHeartbeatCount_set(uint32_t value)
{
    tx_table.BMS_WarningsCounts_signals.BMS_Warning_MissingVCHeartbeatCount_value = (value > CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MAX) ? CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_StateMachineCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_CellUndervoltageCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_CellOvervoltageCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_ModuleCommunicationErrorCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_CellUndertempCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_CellOvertempCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_ChargerReportedErrorCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerReportedErrorCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerShutdownLoopOpenCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_MAX : value;
}

void app_canTx_BMS_Fault_PrechargeFailureCount_set(uint32_t value)
{
    tx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value = (value > CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX) ? CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_MAX : value;
}

bool app_canTx_BMS_Heartbeat_get()
{
    return tx_table.BMS_Vitals_signals.BMS_Heartbeat_value;
}

BmsState app_canTx_BMS_State_get()
{
    return tx_table.BMS_Vitals_signals.BMS_State_value;
}

uint32_t app_canTx_BMS_TxOverflowCount_get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_TxOverflowCount_value;
}

uint32_t app_canTx_BMS_RxOverflowCount_get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_RxOverflowCount_value;
}

RtosTaskName app_canTx_BMS_WatchdogTimeoutTaskName_get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_WatchdogTimeoutTaskName_value;
}

uint32_t app_canTx_BMS_ModuleCommunication_NumCommTries_get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_NumCommTries_value;
}

CAN_AccumulatorMonitorState app_canTx_BMS_ModuleCommunication_MonitorState_get()
{
    return tx_table.BMS_AlertsContext_signals.BMS_ModuleCommunication_MonitorState_value;
}

bool app_canTx_BMS_ImdOkHs_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdOkHs_value;
}

uint32_t app_canTx_BMS_ImdTimeSincePowerOn_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdTimeSincePowerOn_value;
}

bool app_canTx_BMS_ImdValidDutyCycle_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdValidDutyCycle_value;
}

ImdConditionName app_canTx_BMS_ImdCondition_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdCondition_value;
}

float app_canTx_BMS_ImdDutyCycle_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdDutyCycle_value;
}

float app_canTx_BMS_ImdFrequency_get()
{
    return tx_table.BMS_ImdStatus_signals.BMS_ImdFrequency_value;
}

ImdActiveFrequency app_canTx_BMS_ImdActiveFrequency_get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdActiveFrequency_value;
}

float app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp10Hz_value;
}

float app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdInsulationMeasurementDcp20Hz_value;
}

float app_canTx_BMS_ImdSpeedStartStatus30Hz_get()
{
    return tx_table.BMS_ImdData_signals.BMS_ImdSpeedStartStatus30Hz_value;
}

bool app_canTx_BMS_ChargerConnected_get()
{
    return tx_table.BMS_Charger_signals.BMS_ChargerConnected_value;
}

bool app_canTx_BMS_ChargerEnable_get()
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargerEnable_value;
}

bool app_canTx_BMS_ClearLatch_get()
{
    return tx_table.BMS_BrusaControls_signals.BMS_ClearLatch_value;
}

float app_canTx_BMS_MaxMainsCurrent_get()
{
    return tx_table.BMS_BrusaControls_signals.BMS_MaxMainsCurrent_value;
}

float app_canTx_BMS_ChargingVoltage_get()
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargingVoltage_value;
}

float app_canTx_BMS_ChargingCurrent_get()
{
    return tx_table.BMS_BrusaControls_signals.BMS_ChargingCurrent_value;
}

bool app_canTx_BMS_BmsOk_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsOk_value;
}

bool app_canTx_BMS_ImdOk_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdOk_value;
}

bool app_canTx_BMS_BspdOk_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdOk_value;
}

bool app_canTx_BMS_BmsLatchedFault_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BmsLatchedFault_value;
}

bool app_canTx_BMS_ImdLatchedFault_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_ImdLatchedFault_value;
}

bool app_canTx_BMS_BspdLatchedFault_get()
{
    return tx_table.BMS_OkStatuses_signals.BMS_BspdLatchedFault_value;
}

ContactorState app_canTx_BMS_AirPositive_get()
{
    return tx_table.BMS_Contactors_signals.BMS_AirPositive_value;
}

ContactorState app_canTx_BMS_AirNegative_get()
{
    return tx_table.BMS_Contactors_signals.BMS_AirNegative_value;
}

ContactorState app_canTx_BMS_PrechargeRelay_get()
{
    return tx_table.BMS_Contactors_signals.BMS_PrechargeRelay_value;
}

float app_canTx_BMS_MinCellTemperature_get()
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MinCellTemperature_value;
}

float app_canTx_BMS_MaxCellTemperature_get()
{
    return tx_table.BMS_CellTemperatures_signals.BMS_MaxCellTemperature_value;
}

float app_canTx_BMS_PackVoltage_get()
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_PackVoltage_value;
}

float app_canTx_BMS_Soc_get()
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_Soc_value;
}

bool app_canTx_BMS_SocCorrupt_get()
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_SocCorrupt_value;
}

float app_canTx_BMS_MinCellVoltage_get()
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_MinCellVoltage_value;
}

float app_canTx_BMS_MaxCellVoltage_get()
{
    return tx_table.BMS_VoltageAndChargeStats_signals.BMS_MaxCellVoltage_value;
}

uint32_t app_canTx_BMS_MinCellVoltageSegment_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageSegment_value;
}

uint32_t app_canTx_BMS_MinCellVoltageIdx_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinCellVoltageIdx_value;
}

uint32_t app_canTx_BMS_MaxCellVoltageSegment_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageSegment_value;
}

uint32_t app_canTx_BMS_MaxCellVoltageIdx_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxCellVoltageIdx_value;
}

uint32_t app_canTx_BMS_MinTempSegment_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempSegment_value;
}

uint32_t app_canTx_BMS_MinTempIdx_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MinTempIdx_value;
}

uint32_t app_canTx_BMS_MaxTempSegment_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempSegment_value;
}

uint32_t app_canTx_BMS_MaxTempIdx_get()
{
    return tx_table.BMS_CellStats_signals.BMS_MaxTempIdx_value;
}

float app_canTx_BMS_TractiveSystemVoltage_get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemVoltage_value;
}

float app_canTx_BMS_TractiveSystemCurrent_get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemCurrent_value;
}

float app_canTx_BMS_TractiveSystemPower_get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_TractiveSystemPower_value;
}

uint32_t app_canTx_BMS_AvailablePower_get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_AvailablePower_value;
}

bool app_canTx_BMS_BSPDCurrentThresholdExceeded_get()
{
    return tx_table.BMS_TractiveSystem_signals.BMS_BSPDCurrentThresholdExceeded_value;
}

uint32_t app_canTx_BMS_Hash_get()
{
    return tx_table.BMS_CommitInfo_signals.BMS_Hash_value;
}

bool app_canTx_BMS_Clean_get()
{
    return tx_table.BMS_CommitInfo_signals.BMS_Clean_value;
}

uint32_t app_canTx_BMS_Segment0_OWC_Cells_Status_get()
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment0_OWC_Cells_Status_value;
}

uint32_t app_canTx_BMS_Segment1_OWC_Cells_Status_get()
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment1_OWC_Cells_Status_value;
}

uint32_t app_canTx_BMS_Segment2_OWC_Cells_Status_get()
{
    return tx_table.BMS_OWC_Segment0to2_Status_signals.BMS_Segment2_OWC_Cells_Status_value;
}

uint32_t app_canTx_BMS_Segment3_OWC_Cells_Status_get()
{
    return tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment3_OWC_Cells_Status_value;
}

uint32_t app_canTx_BMS_Segment4_OWC_Cells_Status_get()
{
    return tx_table.BMS_OWC_Segment3to4_Status_signals.BMS_Segment4_OWC_Cells_Status_value;
}

bool app_canTx_BMS_HVDShdnOKStatus_get()
{
    return tx_table.BMS_BMSShdnNodeStatus_signals.BMS_HVDShdnOKStatus_value;
}

bool app_canTx_BMS_TSIlckOKStatus_get()
{
    return tx_table.BMS_BMSShdnNodeStatus_signals.BMS_TSIlckOKStatus_value;
}

bool app_canTx_BMS_Warning_StackWaterMarkHighTask1Hz_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1Hz_value;
}

bool app_canTx_BMS_Warning_StackWaterMarkHighTask100Hz_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask100Hz_value;
}

bool app_canTx_BMS_Warning_StackWaterMarkHighTask1kHz_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTask1kHz_value;
}

bool app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool app_canTx_BMS_Warning_WatchdogTimeout_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_WatchdogTimeout_value;
}

bool app_canTx_BMS_Warning_TxOverflow_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_TxOverflow_value;
}

bool app_canTx_BMS_Warning_RxOverflow_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_RxOverflow_value;
}

bool app_canTx_BMS_Warning_OpenWireCheckFault_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheckFault_value;
}

bool app_canTx_BMS_Warning_OpenWireCheck_Segment0_GND_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment0_GND_value;
}

bool app_canTx_BMS_Warning_OpenWireCheck_Segment1_GND_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment1_GND_value;
}

bool app_canTx_BMS_Warning_OpenWireCheck_Segment2_GND_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment2_GND_value;
}

bool app_canTx_BMS_Warning_OpenWireCheck_Segment3_GND_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment3_GND_value;
}

bool app_canTx_BMS_Warning_OpenWireCheck_Segment4_GND_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_OpenWireCheck_Segment4_GND_value;
}

bool app_canTx_BMS_Warning_MissingVCHeartbeat_get()
{
    return tx_table.BMS_Warnings_signals.BMS_Warning_MissingVCHeartbeat_value;
}

bool app_canTx_BMS_Fault_StateMachine_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_StateMachine_value;
}

bool app_canTx_BMS_Fault_CellUndervoltage_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellUndervoltage_value;
}

bool app_canTx_BMS_Fault_CellOvervoltage_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellOvervoltage_value;
}

bool app_canTx_BMS_Fault_ModuleCommunicationError_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ModuleCommunicationError_value;
}

bool app_canTx_BMS_Fault_CellUndertemp_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellUndertemp_value;
}

bool app_canTx_BMS_Fault_CellOvertemp_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_CellOvertemp_value;
}

bool app_canTx_BMS_Fault_ChargerReportedError_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ChargerReportedError_value;
}

bool app_canTx_BMS_Fault_ChargerDisconnectedDuringCharge_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ChargerDisconnectedDuringCharge_value;
}

bool app_canTx_BMS_Fault_ChargerShutdownLoopOpen_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_ChargerShutdownLoopOpen_value;
}

bool app_canTx_BMS_Fault_TractiveSystemOvercurrent_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_TractiveSystemOvercurrent_value;
}

bool app_canTx_BMS_Fault_PrechargeFailure_get()
{
    return tx_table.BMS_Faults_signals.BMS_Fault_PrechargeFailure_value;
}

uint32_t app_canTx_BMS_Warning_StackWaterMarkHighTask1HzCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1HzCount_value;
}

uint32_t app_canTx_BMS_Warning_StackWaterMarkHighTask100HzCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask100HzCount_value;
}

uint32_t app_canTx_BMS_Warning_StackWaterMarkHighTask1kHzCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTask1kHzCount_value;
}

uint32_t app_canTx_BMS_Warning_StackWaterMarkHighTaskCanRxCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanRxCount_value;
}

uint32_t app_canTx_BMS_Warning_StackWaterMarkHighTaskCanTxCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_StackWaterMarkHighTaskCanTxCount_value;
}

uint32_t app_canTx_BMS_Warning_WatchdogTimeoutCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_WatchdogTimeoutCount_value;
}

uint32_t app_canTx_BMS_Warning_TxOverflowCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_TxOverflowCount_value;
}

uint32_t app_canTx_BMS_Warning_RxOverflowCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_RxOverflowCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheckFaultCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheckFaultCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheck_Segment0_GNDCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment0_GNDCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheck_Segment1_GNDCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment1_GNDCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheck_Segment2_GNDCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment2_GNDCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheck_Segment3_GNDCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment3_GNDCount_value;
}

uint32_t app_canTx_BMS_Warning_OpenWireCheck_Segment4_GNDCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_OpenWireCheck_Segment4_GNDCount_value;
}

uint32_t app_canTx_BMS_Warning_MissingVCHeartbeatCount_get()
{
    return tx_table.BMS_WarningsCounts_signals.BMS_Warning_MissingVCHeartbeatCount_value;
}

uint32_t app_canTx_BMS_Fault_StateMachineCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_StateMachineCount_value;
}

uint32_t app_canTx_BMS_Fault_CellUndervoltageCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndervoltageCount_value;
}

uint32_t app_canTx_BMS_Fault_CellOvervoltageCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvervoltageCount_value;
}

uint32_t app_canTx_BMS_Fault_ModuleCommunicationErrorCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ModuleCommunicationErrorCount_value;
}

uint32_t app_canTx_BMS_Fault_CellUndertempCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellUndertempCount_value;
}

uint32_t app_canTx_BMS_Fault_CellOvertempCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_CellOvertempCount_value;
}

uint32_t app_canTx_BMS_Fault_ChargerReportedErrorCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerReportedErrorCount_value;
}

uint32_t app_canTx_BMS_Fault_ChargerDisconnectedDuringChargeCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerDisconnectedDuringChargeCount_value;
}

uint32_t app_canTx_BMS_Fault_ChargerShutdownLoopOpenCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_ChargerShutdownLoopOpenCount_value;
}

uint32_t app_canTx_BMS_Fault_TractiveSystemOvercurrentCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_TractiveSystemOvercurrentCount_value;
}

uint32_t app_canTx_BMS_Fault_PrechargeFailureCount_get()
{
    return tx_table.BMS_FaultsCounts_signals.BMS_Fault_PrechargeFailureCount_value;
}

const BMS_Vitals_Signals* app_canTx_BMS_Vitals_getData()
{
    return &tx_table.BMS_Vitals_signals;
}

const BMS_AlertsContext_Signals* app_canTx_BMS_AlertsContext_getData()
{
    return &tx_table.BMS_AlertsContext_signals;
}

const BMS_ImdStatus_Signals* app_canTx_BMS_ImdStatus_getData()
{
    return &tx_table.BMS_ImdStatus_signals;
}

const BMS_ImdData_Signals* app_canTx_BMS_ImdData_getData()
{
    return &tx_table.BMS_ImdData_signals;
}

const BMS_Charger_Signals* app_canTx_BMS_Charger_getData()
{
    return &tx_table.BMS_Charger_signals;
}

const BMS_BrusaControls_Signals* app_canTx_BMS_BrusaControls_getData()
{
    return &tx_table.BMS_BrusaControls_signals;
}

const BMS_OkStatuses_Signals* app_canTx_BMS_OkStatuses_getData()
{
    return &tx_table.BMS_OkStatuses_signals;
}

const BMS_Contactors_Signals* app_canTx_BMS_Contactors_getData()
{
    return &tx_table.BMS_Contactors_signals;
}

const BMS_CellTemperatures_Signals* app_canTx_BMS_CellTemperatures_getData()
{
    return &tx_table.BMS_CellTemperatures_signals;
}

const BMS_VoltageAndChargeStats_Signals* app_canTx_BMS_VoltageAndChargeStats_getData()
{
    return &tx_table.BMS_VoltageAndChargeStats_signals;
}

const BMS_CellStats_Signals* app_canTx_BMS_CellStats_getData()
{
    return &tx_table.BMS_CellStats_signals;
}

const BMS_TractiveSystem_Signals* app_canTx_BMS_TractiveSystem_getData()
{
    return &tx_table.BMS_TractiveSystem_signals;
}

const BMS_CommitInfo_Signals* app_canTx_BMS_CommitInfo_getData()
{
    return &tx_table.BMS_CommitInfo_signals;
}

const BMS_OWC_Segment0to2_Status_Signals* app_canTx_BMS_OWC_Segment0to2_Status_getData()
{
    return &tx_table.BMS_OWC_Segment0to2_Status_signals;
}

const BMS_OWC_Segment3to4_Status_Signals* app_canTx_BMS_OWC_Segment3to4_Status_getData()
{
    return &tx_table.BMS_OWC_Segment3to4_Status_signals;
}

const BMS_BMSShdnNodeStatus_Signals* app_canTx_BMS_BMSShdnNodeStatus_getData()
{
    return &tx_table.BMS_BMSShdnNodeStatus_signals;
}

const BMS_Warnings_Signals* app_canTx_BMS_Warnings_getData()
{
    return &tx_table.BMS_Warnings_signals;
}

const BMS_Faults_Signals* app_canTx_BMS_Faults_getData()
{
    return &tx_table.BMS_Faults_signals;
}

const BMS_WarningsCounts_Signals* app_canTx_BMS_WarningsCounts_getData()
{
    return &tx_table.BMS_WarningsCounts_signals;
}

const BMS_FaultsCounts_Signals* app_canTx_BMS_FaultsCounts_getData()
{
    return &tx_table.BMS_FaultsCounts_signals;
}
