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
 * Struct for holding all messages transmitted by PDM (i.e. the TX table).
 */
typedef struct
{
    PDM_Vitals_Signals PDM_Vitals_signals;
    PDM_AlertsContext_Signals PDM_AlertsContext_signals;
    PDM_Voltages_Signals PDM_Voltages_signals;
    PDM_CommitInfo_Signals PDM_CommitInfo_signals;
    PDM_Warnings_Signals PDM_Warnings_signals;
    PDM_Faults_Signals PDM_Faults_signals;
    PDM_WarningsCounts_Signals PDM_WarningsCounts_signals;
    PDM_FaultsCounts_Signals PDM_FaultsCounts_signals;
} PDM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static PDM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(PDM_TxMsgs));
    App_CanTx_PDM_Heartbeat_Set(CANSIG_PDM_VITALS_PDM_HEARTBEAT_START_VAL);
    App_CanTx_PDM_State_Set(CANSIG_PDM_VITALS_PDM_STATE_START_VAL);
    App_CanTx_PDM_StackWatermarkAboveThresholdTask1Hz_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_START_VAL);
    App_CanTx_PDM_StackWatermarkAboveThresholdTask100Hz_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ_START_VAL);
    App_CanTx_PDM_StackWatermarkAboveThresholdTask1kHz_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_START_VAL);
    App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanTx_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_START_VAL);
    App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanRx_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_START_VAL);
    App_CanTx_PDM_WatchdogTimeout_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_PDM_TxOverflowCount_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_PDM_RxOverflowCount_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_PDM_WatchdogTimeoutTaskName_Set(CANSIG_PDM_ALERTS_CONTEXT_PDM_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_PDM_BatVoltage_Set(CANSIG_PDM_VOLTAGES_PDM_BAT_VOLTAGE_START_VAL);
    App_CanTx_PDM_BoostVoltage_Set(CANSIG_PDM_VOLTAGES_PDM_BOOST_VOLTAGE_START_VAL);
    App_CanTx_PDM_AccVoltage_Set(CANSIG_PDM_VOLTAGES_PDM_ACC_VOLTAGE_START_VAL);
    App_CanTx_PDM_Hash_Set(CANSIG_PDM_COMMIT_INFO_PDM_HASH_START_VAL);
    App_CanTx_PDM_Clean_Set(CANSIG_PDM_COMMIT_INFO_PDM_CLEAN_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask1HzWarning_Set(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask100HzWarning_Set(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask1kHzWarning_Set(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarning_Set(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarning_Set(CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanTx_PDM_WatchdogTimeoutWarning_Set(CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanTx_PDM_TxOverflowWarning_Set(CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_PDM_RxOverflowWarning_Set(CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_PDM_DummyFault_Set(CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask1HzWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask100HzWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTask1kHzWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_WatchdogTimeoutWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_WATCHDOG_TIMEOUT_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_TxOverflowWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_TX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_RxOverflowWarningCount_Set(CANSIG_PDM_WARNINGS_COUNTS_PDM_RX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_PDM_DummyFaultCount_Set(CANSIG_PDM_FAULTS_COUNTS_PDM_DUMMY_FAULT_COUNT_START_VAL);
}

void App_CanTx_PDM_Heartbeat_Set(bool value)
{
    tx_table.PDM_Vitals_signals.PDM_Heartbeat_value = value;
}

void App_CanTx_PDM_State_Set(PdmState value)
{
    tx_table.PDM_Vitals_signals.PDM_State_value = (value > CANSIG_PDM_VITALS_PDM_STATE_MAX) ? CANSIG_PDM_VITALS_PDM_STATE_MAX : value;
}

void App_CanTx_PDM_StackWatermarkAboveThresholdTask1Hz_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask1Hz_value = value;
}

void App_CanTx_PDM_StackWatermarkAboveThresholdTask100Hz_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask100Hz_value = value;
}

void App_CanTx_PDM_StackWatermarkAboveThresholdTask1kHz_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask1kHz_value = value;
}

void App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanTx_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTaskCanTx_value = value;
}

void App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanRx_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTaskCanRx_value = value;
}

void App_CanTx_PDM_WatchdogTimeout_Set(bool value)
{
    tx_table.PDM_AlertsContext_signals.PDM_WatchdogTimeout_value = value;
}

void App_CanTx_PDM_TxOverflowCount_Set(uint32_t value)
{
    tx_table.PDM_AlertsContext_signals.PDM_TxOverflowCount_value = (value > CANSIG_PDM_ALERTS_CONTEXT_PDM_TX_OVERFLOW_COUNT_MAX) ? CANSIG_PDM_ALERTS_CONTEXT_PDM_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_PDM_RxOverflowCount_Set(uint32_t value)
{
    tx_table.PDM_AlertsContext_signals.PDM_RxOverflowCount_value = (value > CANSIG_PDM_ALERTS_CONTEXT_PDM_RX_OVERFLOW_COUNT_MAX) ? CANSIG_PDM_ALERTS_CONTEXT_PDM_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_PDM_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.PDM_AlertsContext_signals.PDM_WatchdogTimeoutTaskName_value = (value > CANSIG_PDM_ALERTS_CONTEXT_PDM_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_PDM_ALERTS_CONTEXT_PDM_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_PDM_BatVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_PDM_VOLTAGES_PDM_BAT_VOLTAGE_MIN ? CANSIG_PDM_VOLTAGES_PDM_BAT_VOLTAGE_MIN : value;
    tx_table.PDM_Voltages_signals.PDM_BatVoltage_value = tmp > CANSIG_PDM_VOLTAGES_PDM_BAT_VOLTAGE_MAX ? CANSIG_PDM_VOLTAGES_PDM_BAT_VOLTAGE_MAX : tmp;
}

void App_CanTx_PDM_BoostVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_PDM_VOLTAGES_PDM_BOOST_VOLTAGE_MIN ? CANSIG_PDM_VOLTAGES_PDM_BOOST_VOLTAGE_MIN : value;
    tx_table.PDM_Voltages_signals.PDM_BoostVoltage_value = tmp > CANSIG_PDM_VOLTAGES_PDM_BOOST_VOLTAGE_MAX ? CANSIG_PDM_VOLTAGES_PDM_BOOST_VOLTAGE_MAX : tmp;
}

void App_CanTx_PDM_AccVoltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_PDM_VOLTAGES_PDM_ACC_VOLTAGE_MIN ? CANSIG_PDM_VOLTAGES_PDM_ACC_VOLTAGE_MIN : value;
    tx_table.PDM_Voltages_signals.PDM_AccVoltage_value = tmp > CANSIG_PDM_VOLTAGES_PDM_ACC_VOLTAGE_MAX ? CANSIG_PDM_VOLTAGES_PDM_ACC_VOLTAGE_MAX : tmp;
}

void App_CanTx_PDM_Hash_Set(uint32_t value)
{
    tx_table.PDM_CommitInfo_signals.PDM_Hash_value = value;
}

void App_CanTx_PDM_Clean_Set(bool value)
{
    tx_table.PDM_CommitInfo_signals.PDM_Clean_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTask1HzWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTask100HzWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTask1kHzWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanTx_PDM_WatchdogTimeoutWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value = value;
}

void App_CanTx_PDM_TxOverflowWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value = value;
}

void App_CanTx_PDM_RxOverflowWarning_Set(bool value)
{
    tx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value = value;
}

void App_CanTx_PDM_DummyFault_Set(bool value)
{
    tx_table.PDM_Faults_signals.PDM_DummyFault_value = value;
}

void App_CanTx_PDM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask1HzWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask100HzWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask1kHzWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTaskCanRxWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTaskCanTxWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_WatchdogTimeoutWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_WatchdogTimeoutWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_TxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_TxOverflowWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_TX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_TX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_RxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.PDM_WarningsCounts_signals.PDM_RxOverflowWarningCount_value = (value > CANSIG_PDM_WARNINGS_COUNTS_PDM_RX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_PDM_WARNINGS_COUNTS_PDM_RX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_PDM_DummyFaultCount_Set(uint32_t value)
{
    tx_table.PDM_FaultsCounts_signals.PDM_DummyFaultCount_value = (value > CANSIG_PDM_FAULTS_COUNTS_PDM_DUMMY_FAULT_COUNT_MAX) ? CANSIG_PDM_FAULTS_COUNTS_PDM_DUMMY_FAULT_COUNT_MAX : value;
}

bool App_CanTx_PDM_Heartbeat_Get()
{
    return tx_table.PDM_Vitals_signals.PDM_Heartbeat_value;
}

PdmState App_CanTx_PDM_State_Get()
{
    return tx_table.PDM_Vitals_signals.PDM_State_value;
}

bool App_CanTx_PDM_StackWatermarkAboveThresholdTask1Hz_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask1Hz_value;
}

bool App_CanTx_PDM_StackWatermarkAboveThresholdTask100Hz_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask100Hz_value;
}

bool App_CanTx_PDM_StackWatermarkAboveThresholdTask1kHz_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTask1kHz_value;
}

bool App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanTx_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTaskCanTx_value;
}

bool App_CanTx_PDM_StackWatermarkAboveThresholdTaskCanRx_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_StackWatermarkAboveThresholdTaskCanRx_value;
}

bool App_CanTx_PDM_WatchdogTimeout_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_WatchdogTimeout_value;
}

uint32_t App_CanTx_PDM_TxOverflowCount_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_TxOverflowCount_value;
}

uint32_t App_CanTx_PDM_RxOverflowCount_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_RxOverflowCount_value;
}

RtosTaskName App_CanTx_PDM_WatchdogTimeoutTaskName_Get()
{
    return tx_table.PDM_AlertsContext_signals.PDM_WatchdogTimeoutTaskName_value;
}

float App_CanTx_PDM_BatVoltage_Get()
{
    return tx_table.PDM_Voltages_signals.PDM_BatVoltage_value;
}

float App_CanTx_PDM_BoostVoltage_Get()
{
    return tx_table.PDM_Voltages_signals.PDM_BoostVoltage_value;
}

float App_CanTx_PDM_AccVoltage_Get()
{
    return tx_table.PDM_Voltages_signals.PDM_AccVoltage_value;
}

uint32_t App_CanTx_PDM_Hash_Get()
{
    return tx_table.PDM_CommitInfo_signals.PDM_Hash_value;
}

bool App_CanTx_PDM_Clean_Get()
{
    return tx_table.PDM_CommitInfo_signals.PDM_Clean_value;
}

bool App_CanTx_PDM_StackWaterMarkHighTask1HzWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanTx_PDM_StackWaterMarkHighTask100HzWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanTx_PDM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanTx_PDM_WatchdogTimeoutWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_WatchdogTimeoutWarning_value;
}

bool App_CanTx_PDM_TxOverflowWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_TxOverflowWarning_value;
}

bool App_CanTx_PDM_RxOverflowWarning_Get()
{
    return tx_table.PDM_Warnings_signals.PDM_RxOverflowWarning_value;
}

bool App_CanTx_PDM_DummyFault_Get()
{
    return tx_table.PDM_Faults_signals.PDM_DummyFault_value;
}

uint32_t App_CanTx_PDM_StackWaterMarkHighTask1HzWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask1HzWarningCount_value;
}

uint32_t App_CanTx_PDM_StackWaterMarkHighTask100HzWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask100HzWarningCount_value;
}

uint32_t App_CanTx_PDM_StackWaterMarkHighTask1kHzWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTask1kHzWarningCount_value;
}

uint32_t App_CanTx_PDM_StackWaterMarkHighTaskCanRxWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTaskCanRxWarningCount_value;
}

uint32_t App_CanTx_PDM_StackWaterMarkHighTaskCanTxWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_StackWaterMarkHighTaskCanTxWarningCount_value;
}

uint32_t App_CanTx_PDM_WatchdogTimeoutWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_WatchdogTimeoutWarningCount_value;
}

uint32_t App_CanTx_PDM_TxOverflowWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_TxOverflowWarningCount_value;
}

uint32_t App_CanTx_PDM_RxOverflowWarningCount_Get()
{
    return tx_table.PDM_WarningsCounts_signals.PDM_RxOverflowWarningCount_value;
}

uint32_t App_CanTx_PDM_DummyFaultCount_Get()
{
    return tx_table.PDM_FaultsCounts_signals.PDM_DummyFaultCount_value;
}

const PDM_Vitals_Signals* App_CanTx_PDM_Vitals_GetData()
{
    return &tx_table.PDM_Vitals_signals;
}

const PDM_AlertsContext_Signals* App_CanTx_PDM_AlertsContext_GetData()
{
    return &tx_table.PDM_AlertsContext_signals;
}

const PDM_Voltages_Signals* App_CanTx_PDM_Voltages_GetData()
{
    return &tx_table.PDM_Voltages_signals;
}

const PDM_CommitInfo_Signals* App_CanTx_PDM_CommitInfo_GetData()
{
    return &tx_table.PDM_CommitInfo_signals;
}

const PDM_Warnings_Signals* App_CanTx_PDM_Warnings_GetData()
{
    return &tx_table.PDM_Warnings_signals;
}

const PDM_Faults_Signals* App_CanTx_PDM_Faults_GetData()
{
    return &tx_table.PDM_Faults_signals;
}

const PDM_WarningsCounts_Signals* App_CanTx_PDM_WarningsCounts_GetData()
{
    return &tx_table.PDM_WarningsCounts_signals;
}

const PDM_FaultsCounts_Signals* App_CanTx_PDM_FaultsCounts_GetData()
{
    return &tx_table.PDM_FaultsCounts_signals;
}

