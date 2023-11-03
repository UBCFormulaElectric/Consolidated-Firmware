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
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    App_CanTx_DIM_Warning_WatchdogTimeout_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_DIM_Warning_TxOverflow_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_DIM_Warning_RxOverflow_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_DIM_Fault_MissingHeartbeat_Set(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_START_VAL);
    App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_START_VAL);
    App_CanTx_DIM_Warning_WatchdogTimeoutCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_WATCHDOG_TIMEOUT_COUNT_START_VAL);
    App_CanTx_DIM_Warning_TxOverflowCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_Warning_RxOverflowCount_Set(CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_Fault_MissingHeartbeatCount_Set(CANSIG_DIM_FAULTS_COUNTS_DIM_FAULT_MISSING_HEARTBEAT_COUNT_START_VAL);
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

void App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void App_CanTx_DIM_Warning_WatchdogTimeout_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value = value;
}

void App_CanTx_DIM_Warning_TxOverflow_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value = value;
}

void App_CanTx_DIM_Warning_RxOverflow_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value = value;
}

void App_CanTx_DIM_Fault_MissingHeartbeat_Set(bool value)
{
    tx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value = value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask1HzCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask100HzCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask1kHzCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTaskCanRxCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTaskCanTxCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_WatchdogTimeoutCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_WatchdogTimeoutCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_WATCHDOG_TIMEOUT_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_TxOverflowCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_Warning_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_WarningsCounts_signals.DIM_Warning_RxOverflowCount_value = (value > CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_Fault_MissingHeartbeatCount_Set(uint32_t value)
{
    tx_table.DIM_FaultsCounts_signals.DIM_Fault_MissingHeartbeatCount_value = (value > CANSIG_DIM_FAULTS_COUNTS_DIM_FAULT_MISSING_HEARTBEAT_COUNT_MAX) ? CANSIG_DIM_FAULTS_COUNTS_DIM_FAULT_MISSING_HEARTBEAT_COUNT_MAX : value;
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

bool App_CanTx_DIM_Warning_StackWaterMarkHighTask1Hz_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1Hz_value;
}

bool App_CanTx_DIM_Warning_StackWaterMarkHighTask100Hz_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask100Hz_value;
}

bool App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHz_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTask1kHz_value;
}

bool App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRx_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTx_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool App_CanTx_DIM_Warning_WatchdogTimeout_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_WatchdogTimeout_value;
}

bool App_CanTx_DIM_Warning_TxOverflow_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_TxOverflow_value;
}

bool App_CanTx_DIM_Warning_RxOverflow_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_Warning_RxOverflow_value;
}

bool App_CanTx_DIM_Fault_MissingHeartbeat_Get()
{
    return tx_table.DIM_Faults_signals.DIM_Fault_MissingHeartbeat_value;
}

uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask1HzCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask1HzCount_value;
}

uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask100HzCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask100HzCount_value;
}

uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTask1kHzCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTask1kHzCount_value;
}

uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanRxCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTaskCanRxCount_value;
}

uint32_t App_CanTx_DIM_Warning_StackWaterMarkHighTaskCanTxCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_StackWaterMarkHighTaskCanTxCount_value;
}

uint32_t App_CanTx_DIM_Warning_WatchdogTimeoutCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_WatchdogTimeoutCount_value;
}

uint32_t App_CanTx_DIM_Warning_TxOverflowCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_TxOverflowCount_value;
}

uint32_t App_CanTx_DIM_Warning_RxOverflowCount_Get()
{
    return tx_table.DIM_WarningsCounts_signals.DIM_Warning_RxOverflowCount_value;
}

uint32_t App_CanTx_DIM_Fault_MissingHeartbeatCount_Get()
{
    return tx_table.DIM_FaultsCounts_signals.DIM_Fault_MissingHeartbeatCount_value;
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

