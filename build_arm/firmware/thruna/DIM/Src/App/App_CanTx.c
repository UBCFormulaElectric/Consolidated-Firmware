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
    DIM_Warnings_Signals DIM_Warnings_signals;
    DIM_Faults_Signals DIM_Faults_signals;
} DIM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static DIM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(DIM_TxMsgs));
    App_CanTx_DIM_Vitals_Heartbeat_Set(CANSIG_DIM_VITALS_HEARTBEAT_START_VAL);
    App_CanTx_DIM_Vitals_State_Set(CANSIG_DIM_VITALS_STATE_START_VAL);
    App_CanTx_DIM_Switches_StartSwitch_Set(CANSIG_DIM_SWITCHES_START_SWITCH_START_VAL);
    App_CanTx_DIM_Switches_AuxSwitch_Set(CANSIG_DIM_SWITCHES_AUX_SWITCH_START_VAL);
    App_CanTx_DIM_AlertsContext_TxOverflowCount_Set(CANSIG_DIM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_AlertsContext_RxOverflowCount_Set(CANSIG_DIM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_START_VAL);
    App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Set(CANSIG_DIM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_START_VAL);
    App_CanTx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Set(CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_START_VAL);
    App_CanTx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Set(CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_START_VAL);
}

void App_CanTx_DIM_Vitals_Heartbeat_Set(bool value)
{
    tx_table.DIM_Vitals_signals.Heartbeat_value = value;
}

void App_CanTx_DIM_Vitals_State_Set(DimState value)
{
    tx_table.DIM_Vitals_signals.State_value = (value > CANSIG_DIM_VITALS_STATE_MAX) ? CANSIG_DIM_VITALS_STATE_MAX : value;
}

void App_CanTx_DIM_Switches_StartSwitch_Set(SwitchState value)
{
    tx_table.DIM_Switches_signals.StartSwitch_value = (value > CANSIG_DIM_SWITCHES_START_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_START_SWITCH_MAX : value;
}

void App_CanTx_DIM_Switches_AuxSwitch_Set(SwitchState value)
{
    tx_table.DIM_Switches_signals.AuxSwitch_value = (value > CANSIG_DIM_SWITCHES_AUX_SWITCH_MAX) ? CANSIG_DIM_SWITCHES_AUX_SWITCH_MAX : value;
}

void App_CanTx_DIM_AlertsContext_TxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_AlertsContext_signals.TxOverflowCount_value = (value > CANSIG_DIM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_AlertsContext_RxOverflowCount_Set(uint32_t value)
{
    tx_table.DIM_AlertsContext_signals.RxOverflowCount_value = (value > CANSIG_DIM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_MAX : value;
}

void App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Set(RtosTaskName value)
{
    tx_table.DIM_AlertsContext_signals.WatchdogTimeoutTaskName_value = (value > CANSIG_DIM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_MAX) ? CANSIG_DIM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_MAX : value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_WATCHDOG_TIMEOUT_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_TX_OVERFLOW_value = value;
}

void App_CanTx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Set(bool value)
{
    tx_table.DIM_Warnings_signals.DIM_WARNING_RX_OVERFLOW_value = value;
}

void App_CanTx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Set(bool value)
{
    tx_table.DIM_Faults_signals.DIM_FAULT_MISSING_HEARTBEAT_value = value;
}

bool App_CanTx_DIM_Vitals_Heartbeat_Get()
{
    return tx_table.DIM_Vitals_signals.Heartbeat_value;
}

DimState App_CanTx_DIM_Vitals_State_Get()
{
    return tx_table.DIM_Vitals_signals.State_value;
}

SwitchState App_CanTx_DIM_Switches_StartSwitch_Get()
{
    return tx_table.DIM_Switches_signals.StartSwitch_value;
}

SwitchState App_CanTx_DIM_Switches_AuxSwitch_Get()
{
    return tx_table.DIM_Switches_signals.AuxSwitch_value;
}

uint32_t App_CanTx_DIM_AlertsContext_TxOverflowCount_Get()
{
    return tx_table.DIM_AlertsContext_signals.TxOverflowCount_value;
}

uint32_t App_CanTx_DIM_AlertsContext_RxOverflowCount_Get()
{
    return tx_table.DIM_AlertsContext_signals.RxOverflowCount_value;
}

RtosTaskName App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Get()
{
    return tx_table.DIM_AlertsContext_signals.WatchdogTimeoutTaskName_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_WATCHDOG_TIMEOUT_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_WATCHDOG_TIMEOUT_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_TX_OVERFLOW_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_TX_OVERFLOW_value;
}

bool App_CanTx_DIM_Warnings_DIM_WARNING_RX_OVERFLOW_Get()
{
    return tx_table.DIM_Warnings_signals.DIM_WARNING_RX_OVERFLOW_value;
}

bool App_CanTx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Get()
{
    return tx_table.DIM_Faults_signals.DIM_FAULT_MISSING_HEARTBEAT_value;
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

const DIM_Warnings_Signals* App_CanTx_DIM_Warnings_GetData()
{
    return &tx_table.DIM_Warnings_signals;
}

const DIM_Faults_Signals* App_CanTx_DIM_Faults_GetData()
{
    return &tx_table.DIM_Faults_signals;
}

