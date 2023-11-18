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
 * Struct for holding all messages transmitted by GSM (i.e. the TX table).
 */
typedef struct
{
    GSM_GSM_Vitals_Signals GSM_GSM_Vitals_signals;
    GSM_GSM_Warnings_Signals GSM_GSM_Warnings_signals;
    GSM_CommitInfo_Signals GSM_CommitInfo_signals;
} GSM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static GSM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(GSM_TxMsgs));
    App_CanTx_GSM_Heartbeat_Set(CANSIG_GSM_GSM_VITALS_GSM_HEARTBEAT_START_VAL);
    App_CanTx_GSM_StackWatermarkAboveThresholdTask1Hz_Set(CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_START_VAL);
    App_CanTx_GSM_StackWatermarkAboveThresholdTask100Hz_Set(CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ_START_VAL);
    App_CanTx_GSM_StackWatermarkAboveThresholdTask1kHz_Set(CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_START_VAL);
    App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanTx_Set(CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_START_VAL);
    App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanRx_Set(CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_START_VAL);
    App_CanTx_GSM_WatchdogTimeout_Set(CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_GSM_TxOverflowCount_Set(CANSIG_GSM_GSM_WARNINGS_GSM_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_GSM_RxOverflowCount_Set(CANSIG_GSM_GSM_WARNINGS_GSM_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_GSM_WatchdogTimeoutTaskName_Set(CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_GSM_Hash_Set(CANSIG_GSM_COMMIT_INFO_GSM_HASH_START_VAL);
    App_CanTx_GSM_Clean_Set(CANSIG_GSM_COMMIT_INFO_GSM_CLEAN_START_VAL);
}

void App_CanTx_GSM_Heartbeat_Set(bool value)
{
    tx_table.GSM_GSM_Vitals_signals.GSM_Heartbeat_value = value;
}

void App_CanTx_GSM_StackWatermarkAboveThresholdTask1Hz_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask1Hz_value = value;
}

void App_CanTx_GSM_StackWatermarkAboveThresholdTask100Hz_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask100Hz_value = value;
}

void App_CanTx_GSM_StackWatermarkAboveThresholdTask1kHz_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask1kHz_value = value;
}

void App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanTx_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTaskCanTx_value = value;
}

void App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanRx_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTaskCanRx_value = value;
}

void App_CanTx_GSM_WatchdogTimeout_Set(bool value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_WatchdogTimeout_value = value;
}

void App_CanTx_GSM_TxOverflowCount_Set(uint32_t value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_TxOverflowCount_value = (value > CANSIG_GSM_GSM_WARNINGS_GSM_TX_OVERFLOW_COUNT_MAX) ? CANSIG_GSM_GSM_WARNINGS_GSM_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_GSM_RxOverflowCount_Set(uint32_t value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_RxOverflowCount_value = (value > CANSIG_GSM_GSM_WARNINGS_GSM_RX_OVERFLOW_COUNT_MAX) ? CANSIG_GSM_GSM_WARNINGS_GSM_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_GSM_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.GSM_GSM_Warnings_signals.GSM_WatchdogTimeoutTaskName_value = (value > CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_GSM_Hash_Set(uint32_t value)
{
    tx_table.GSM_CommitInfo_signals.GSM_Hash_value = value;
}

void App_CanTx_GSM_Clean_Set(bool value)
{
    tx_table.GSM_CommitInfo_signals.GSM_Clean_value = value;
}

bool App_CanTx_GSM_Heartbeat_Get()
{
    return tx_table.GSM_GSM_Vitals_signals.GSM_Heartbeat_value;
}

bool App_CanTx_GSM_StackWatermarkAboveThresholdTask1Hz_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask1Hz_value;
}

bool App_CanTx_GSM_StackWatermarkAboveThresholdTask100Hz_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask100Hz_value;
}

bool App_CanTx_GSM_StackWatermarkAboveThresholdTask1kHz_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTask1kHz_value;
}

bool App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanTx_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTaskCanTx_value;
}

bool App_CanTx_GSM_StackWatermarkAboveThresholdTaskCanRx_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_StackWatermarkAboveThresholdTaskCanRx_value;
}

bool App_CanTx_GSM_WatchdogTimeout_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_WatchdogTimeout_value;
}

uint32_t App_CanTx_GSM_TxOverflowCount_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_TxOverflowCount_value;
}

uint32_t App_CanTx_GSM_RxOverflowCount_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_RxOverflowCount_value;
}

RtosTaskName App_CanTx_GSM_WatchdogTimeoutTaskName_Get()
{
    return tx_table.GSM_GSM_Warnings_signals.GSM_WatchdogTimeoutTaskName_value;
}

uint32_t App_CanTx_GSM_Hash_Get()
{
    return tx_table.GSM_CommitInfo_signals.GSM_Hash_value;
}

bool App_CanTx_GSM_Clean_Get()
{
    return tx_table.GSM_CommitInfo_signals.GSM_Clean_value;
}

const GSM_GSM_Vitals_Signals* App_CanTx_GSM_GSM_Vitals_GetData()
{
    return &tx_table.GSM_GSM_Vitals_signals;
}

const GSM_GSM_Warnings_Signals* App_CanTx_GSM_GSM_Warnings_GetData()
{
    return &tx_table.GSM_GSM_Warnings_signals;
}

const GSM_CommitInfo_Signals* App_CanTx_GSM_CommitInfo_GetData()
{
    return &tx_table.GSM_CommitInfo_signals;
}

