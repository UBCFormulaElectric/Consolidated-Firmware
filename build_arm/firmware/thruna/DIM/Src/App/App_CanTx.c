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
 * Struct for holding all messages transmitted by DIM (i.e. the TX table).
 */
typedef struct
{
    DIM_Vitals_Signals DIM_Vitals_signals;
    DIM_Switches_Signals DIM_Switches_signals;
    DIM_AlertsContext_Signals DIM_AlertsContext_signals;
    DIM_CommitInfo_Signals DIM_CommitInfo_signals;
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
    DIM_WarningsCounts_Signals DIM_WarningsCounts_signals;
    DIM_FaultsCounts_Signals DIM_FaultsCounts_signals;
} DIM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static DIM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(DIM_TxMsgs));
    App_CanTx_DIM_Heartbeat_Set(CANSIG_DIM_VITALS_DIM_HEARTBEAT_START_VAL);
    App_CanTx_DIM_State_Set(CANSIG_DIM_VITALS_DIM_STATE_START_VAL);
    App_CanTx_DIM_StartSwitch_Set(CANSIG_DIM_SWITCHES_DIM_START_SWITCH_START_VAL);
    App_CanTx_DIM_AuxSwitch_Set(CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_START_VAL);
    App_CanTx_DIM_TxOverflowCount_Set(CANSIG_DIM_ALERTS_CONTEXT_DIM_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_RxOverflowCount_Set(CANSIG_DIM_ALERTS_CONTEXT_DIM_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_WatchdogTimeoutTaskName_Set(CANSIG_DIM_ALERTS_CONTEXT_DIM_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_DIM_Hash_Set(CANSIG_DIM_COMMIT_INFO_DIM_HASH_START_VAL);
    App_CanTx_DIM_Clean_Set(CANSIG_DIM_COMMIT_INFO_DIM_CLEAN_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask1HzWarning_Set(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask100HzWarning_Set(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask1kHzWarning_Set(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarning_Set(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarning_Set(CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_START_VAL);
    App_CanTx_DIM_WatchdogTimeoutWarning_Set(CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_START_VAL);
    App_CanTx_DIM_TxOverflowWarning_Set(CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_DIM_RxOverflowWarning_Set(CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_START_VAL);
    App_CanTx_DIM_MissingHeartbeatFault_Set(CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask1HzWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask100HzWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTask1kHzWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_WatchdogTimeoutWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WATCHDOG_TIMEOUT_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_TxOverflowWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_TX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_RxOverflowWarningCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_RX_OVERFLOW_WARNING_COUNT_START_VAL);
    App_CanTx_DIM_MissingHeartbeatFaultCount_Set(CANSIG_DIM_FAULTS_COUNTS_DIM_MISSING_HEARTBEAT_FAULT_COUNT_START_VAL);
}

void App_CanTx_DIM_Heartbeat_Set(bool value)
{
    tx_table.DIM_Vitals_signals.DIM_Heartbeat_value = value;
}

void App_CanTx_DIM_State_Set(DimState value)
{
    tx_table.DIM_Vitals_signals.DIM_State_value = (value > CANSIG_DIM_VITALS_DIM_STATE_MAX) ? CANSIG_DIM_VITALS_DIM_STATE_MAX : value;
}

void App_CanTx_DIM_StartSwitch_Set(SwitchState value)
{
    tx_table.DIM_Switches_signals.DIM_StartSwitch_value = (value > CANSIG_DIM_SWITCHES_DIM_START_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_DIM_START_SWITCH_MAX : value;
}

void App_CanTx_DIM_AuxSwitch_Set(SwitchState value)
{
    tx_table.DIM_Switches_signals.DIM_AuxSwitch_value = (value > CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_MAX : value;
}

void App_CanTx_DIM_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_AlertsContext_signals.DIM_TxOverflowCount_value = (value > CANSIG_DIM_ALERTS_CONTEXT_DIM_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_DIM_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_AlertsContext_signals.DIM_RxOverflowCount_value = (value > CANSIG_DIM_ALERTS_CONTEXT_DIM_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_DIM_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.DIM_AlertsContext_signals.DIM_WatchdogTimeoutTaskName_value = (value > CANSIG_DIM_ALERTS_CONTEXT_DIM_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_DIM_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_DIM_Hash_Set(uint32_t value)
{
    tx_table.DIM_CommitInfo_signals.DIM_Hash_value = value;
}

void App_CanTx_DIM_Clean_Set(bool value)
{
    tx_table.DIM_CommitInfo_signals.DIM_Clean_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTask1HzWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTask100HzWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTask1kHzWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value = value;
}

void App_CanTx_DIM_WatchdogTimeoutWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value = value;
}

void App_CanTx_DIM_TxOverflowWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value = value;
}

void App_CanTx_DIM_RxOverflowWarning_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value = value;
}

void App_CanTx_DIM_MissingHeartbeatFault_Set(bool value)
{
    tx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value = value;
}

void App_CanTx_DIM_StackWaterMarkHighTask1HzWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask1HzWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_StackWaterMarkHighTask100HzWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask100HzWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_StackWaterMarkHighTask1kHzWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask1kHzWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTaskCanRxWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTaskCanTxWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_WatchdogTimeoutWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_WatchdogTimeoutWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WATCHDOG_TIMEOUT_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_TxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_TxOverflowWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_TX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_TX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_RxOverflowWarningCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_RxOverflowWarningCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_RX_OVERFLOW_WARNING_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_RX_OVERFLOW_WARNING_COUNT_MAX : value;
}

void App_CanTx_DIM_MissingHeartbeatFaultCount_Set(uint32_t value)
{
    tx_table.DIM_FaultsCounts_signals.DIM_MissingHeartbeatFaultCount_value = (value > CANSIG_DIM_FAULTS_COUNTS_DIM_MISSING_HEARTBEAT_FAULT_COUNT_MAX) ? CANSIG_DIM_FAULTS_COUNTS_DIM_MISSING_HEARTBEAT_FAULT_COUNT_MAX : value;
}

bool App_CanTx_DIM_Heartbeat_Get()
{
    return tx_table.DIM_Vitals_signals.DIM_Heartbeat_value;
}

DimState App_CanTx_DIM_State_Get()
{
    return tx_table.DIM_Vitals_signals.DIM_State_value;
}

SwitchState App_CanTx_DIM_StartSwitch_Get()
{
    return tx_table.DIM_Switches_signals.DIM_StartSwitch_value;
}

SwitchState App_CanTx_DIM_AuxSwitch_Get()
{
    return tx_table.DIM_Switches_signals.DIM_AuxSwitch_value;
}

uint32_t App_CanTx_DIM_TxOverflowCount_Get()
{
    return tx_table.DIM_AlertsContext_signals.DIM_TxOverflowCount_value;
}

uint32_t App_CanTx_DIM_RxOverflowCount_Get()
{
    return tx_table.DIM_AlertsContext_signals.DIM_RxOverflowCount_value;
}

RtosTaskName App_CanTx_DIM_WatchdogTimeoutTaskName_Get()
{
    return tx_table.DIM_AlertsContext_signals.DIM_WatchdogTimeoutTaskName_value;
}

uint32_t App_CanTx_DIM_Hash_Get()
{
    return tx_table.DIM_CommitInfo_signals.DIM_Hash_value;
}

bool App_CanTx_DIM_Clean_Get()
{
    return tx_table.DIM_CommitInfo_signals.DIM_Clean_value;
}

bool App_CanTx_DIM_StackWaterMarkHighTask1HzWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1HzWarning_value;
}

bool App_CanTx_DIM_StackWaterMarkHighTask100HzWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask100HzWarning_value;
}

bool App_CanTx_DIM_StackWaterMarkHighTask1kHzWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTask1kHzWarning_value;
}

bool App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanRxWarning_value;
}

bool App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_StackWaterMarkHighTaskCanTxWarning_value;
}

bool App_CanTx_DIM_WatchdogTimeoutWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WatchdogTimeoutWarning_value;
}

bool App_CanTx_DIM_TxOverflowWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_TxOverflowWarning_value;
}

bool App_CanTx_DIM_RxOverflowWarning_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_RxOverflowWarning_value;
}

bool App_CanTx_DIM_MissingHeartbeatFault_Get()
{
    return tx_table.DIM_Faults_signals.DIM_MissingHeartbeatFault_value;
}

uint32_t App_CanTx_DIM_StackWaterMarkHighTask1HzWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask1HzWarningCount_value;
}

uint32_t App_CanTx_DIM_StackWaterMarkHighTask100HzWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask100HzWarningCount_value;
}

uint32_t App_CanTx_DIM_StackWaterMarkHighTask1kHzWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTask1kHzWarningCount_value;
}

uint32_t App_CanTx_DIM_StackWaterMarkHighTaskCanRxWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTaskCanRxWarningCount_value;
}

uint32_t App_CanTx_DIM_StackWaterMarkHighTaskCanTxWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_StackWaterMarkHighTaskCanTxWarningCount_value;
}

uint32_t App_CanTx_DIM_WatchdogTimeoutWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_WatchdogTimeoutWarningCount_value;
}

uint32_t App_CanTx_DIM_TxOverflowWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_TxOverflowWarningCount_value;
}

uint32_t App_CanTx_DIM_RxOverflowWarningCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_RxOverflowWarningCount_value;
}

uint32_t App_CanTx_DIM_MissingHeartbeatFaultCount_Get()
{
    return tx_table.DIM_FaultsCounts_signals.DIM_MissingHeartbeatFaultCount_value;
}

const DIM_Vitals_Signals* App_CanTx_DIM_Vitals_GetData()
{
    return &tx_table.DIM_Vitals_signals;
}

const DIM_Switches_Signals* App_CanTx_DIM_Switches_GetData()
{
    return &tx_table.DIM_Switches_signals;
}

const DIM_AlertsContext_Signals* App_CanTx_DIM_AlertsContext_GetData()
{
    return &tx_table.DIM_AlertsContext_signals;
}

const DIM_CommitInfo_Signals* App_CanTx_DIM_CommitInfo_GetData()
{
    return &tx_table.DIM_CommitInfo_signals;
}

const DIM_Warnings_Signals* App_CanTx_DIM_Warnings_GetData()
{
    return &tx_table.DIM_Warnings_signals;
}

const DIM_Faults_Signals* App_CanTx_DIM_Faults_GetData()
{
    return &tx_table.DIM_Faults_signals;
}

const DIM_WarningsCounts_Signals* App_CanTx_DIM_WarningsCounts_GetData()
{
    return &tx_table.DIM_WarningsCounts_signals;
}

const DIM_FaultsCounts_Signals* App_CanTx_DIM_FaultsCounts_GetData()
{
    return &tx_table.DIM_FaultsCounts_signals;
}

