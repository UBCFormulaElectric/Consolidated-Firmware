/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by RSM (i.e. the RX table).
 */
typedef struct
{
    BMS_Warnings_Signals BMS_Warnings_signals;
    BMS_Faults_Signals BMS_Faults_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
    CRIT_Warnings_Signals CRIT_Warnings_signals;
    CRIT_Faults_Signals CRIT_Faults_signals;
    VC_Warnings_Signals VC_Warnings_signals;
    VC_Faults_Signals VC_Faults_signals;
} RSM_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static RSM_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(RSM_RxMsgs));
    app_canRx_BMS_Fault_DummyFault_update(CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_START_VAL);
    app_canRx_FSM_Fault_DummyFault_update(CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_START_VAL);
    app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL);
    app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL);
    app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL);
    app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL);
    app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL);
    app_canRx_CRIT_Warning_WatchdogTimeout_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_START_VAL);
    app_canRx_CRIT_Warning_TxOverflow_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_START_VAL);
    app_canRx_CRIT_Warning_RxOverflow_update(CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_START_VAL);
    app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_START_VAL);
    app_canRx_CRIT_Fault_MissingFSMHeartbeat_update(CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_START_VAL);
    app_canRx_CRIT_Fault_MissingVCHeartbeat_update(CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_START_VAL);
    app_canRx_CRIT_Fault_MissingRSMHeartbeat_update(CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_START_VAL);
    app_canRx_VC_Fault_DummyFault_update(CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_START_VAL);
}

void app_canRx_BMS_Fault_DummyFault_update(bool value)
{
    rx_table.BMS_Faults_signals.BMS_Fault_DummyFault_value = value;
}

void app_canRx_FSM_Fault_DummyFault_update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_Fault_DummyFault_value = value;
}

void app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask1Hz_value = value;
}

void app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask100Hz_value = value;
}

void app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask1kHz_value = value;
}

void app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTaskCanRx_value = value;
}

void app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTaskCanTx_value = value;
}

void app_canRx_CRIT_Warning_WatchdogTimeout_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_WatchdogTimeout_value = value;
}

void app_canRx_CRIT_Warning_TxOverflow_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_TxOverflow_value = value;
}

void app_canRx_CRIT_Warning_RxOverflow_update(bool value)
{
    rx_table.CRIT_Warnings_signals.CRIT_Warning_RxOverflow_value = value;
}

void app_canRx_CRIT_Fault_MissingBMSHeartbeat_update(bool value)
{
    rx_table.CRIT_Faults_signals.CRIT_Fault_MissingBMSHeartbeat_value = value;
}

void app_canRx_CRIT_Fault_MissingFSMHeartbeat_update(bool value)
{
    rx_table.CRIT_Faults_signals.CRIT_Fault_MissingFSMHeartbeat_value = value;
}

void app_canRx_CRIT_Fault_MissingVCHeartbeat_update(bool value)
{
    rx_table.CRIT_Faults_signals.CRIT_Fault_MissingVCHeartbeat_value = value;
}

void app_canRx_CRIT_Fault_MissingRSMHeartbeat_update(bool value)
{
    rx_table.CRIT_Faults_signals.CRIT_Fault_MissingRSMHeartbeat_value = value;
}

void app_canRx_VC_Fault_DummyFault_update(bool value)
{
    rx_table.VC_Faults_signals.VC_Fault_DummyFault_value = value;
}

bool app_canRx_BMS_Fault_DummyFault_get()
{
    return rx_table.BMS_Faults_signals.BMS_Fault_DummyFault_value;
}

bool app_canRx_FSM_Fault_DummyFault_get()
{
    return rx_table.FSM_Faults_signals.FSM_Fault_DummyFault_value;
}

bool app_canRx_CRIT_Warning_StackWaterMarkHighTask1Hz_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask1Hz_value;
}

bool app_canRx_CRIT_Warning_StackWaterMarkHighTask100Hz_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask100Hz_value;
}

bool app_canRx_CRIT_Warning_StackWaterMarkHighTask1kHz_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTask1kHz_value;
}

bool app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanRx_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTaskCanRx_value;
}

bool app_canRx_CRIT_Warning_StackWaterMarkHighTaskCanTx_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_StackWaterMarkHighTaskCanTx_value;
}

bool app_canRx_CRIT_Warning_WatchdogTimeout_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_WatchdogTimeout_value;
}

bool app_canRx_CRIT_Warning_TxOverflow_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_TxOverflow_value;
}

bool app_canRx_CRIT_Warning_RxOverflow_get()
{
    return rx_table.CRIT_Warnings_signals.CRIT_Warning_RxOverflow_value;
}

bool app_canRx_CRIT_Fault_MissingBMSHeartbeat_get()
{
    return rx_table.CRIT_Faults_signals.CRIT_Fault_MissingBMSHeartbeat_value;
}

bool app_canRx_CRIT_Fault_MissingFSMHeartbeat_get()
{
    return rx_table.CRIT_Faults_signals.CRIT_Fault_MissingFSMHeartbeat_value;
}

bool app_canRx_CRIT_Fault_MissingVCHeartbeat_get()
{
    return rx_table.CRIT_Faults_signals.CRIT_Fault_MissingVCHeartbeat_value;
}

bool app_canRx_CRIT_Fault_MissingRSMHeartbeat_get()
{
    return rx_table.CRIT_Faults_signals.CRIT_Fault_MissingRSMHeartbeat_value;
}

bool app_canRx_VC_Fault_DummyFault_get()
{
    return rx_table.VC_Faults_signals.VC_Fault_DummyFault_value;
}

