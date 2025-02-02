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
 * Struct for holding all messages transmitted by RSM (i.e. the TX table).
 */
typedef struct
{
    RSM_Vitals_Signals RSM_Vitals_signals;
    RSM_Warnings_Signals RSM_Warnings_signals;
    RSM_Faults_Signals RSM_Faults_signals;
    RSM_WarningsCounts_Signals RSM_WarningsCounts_signals;
    RSM_FaultsCounts_Signals RSM_FaultsCounts_signals;
} RSM_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static RSM_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canTx_init()
{
    memset(&tx_table, 0, sizeof(RSM_TxMsgs));
    app_canTx_RSM_Heartbeat_set(CANSIG_RSM_VITALS_RSM_HEARTBEAT_START_VAL);
    app_canTx_RSM_State_set(CANSIG_RSM_VITALS_RSM_STATE_START_VAL);
    app_canTx_RSM_Fault_DummyFault_set(CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_START_VAL);
    app_canTx_RSM_Fault_DummyFaultCount_set(CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_START_VAL);
}

void app_canTx_RSM_Heartbeat_set(bool value)
{
    tx_table.RSM_Vitals_signals.RSM_Heartbeat_value = value;
}

void app_canTx_RSM_State_set(RsmStates value)
{
    tx_table.RSM_Vitals_signals.RSM_State_value = (value > CANSIG_RSM_VITALS_RSM_STATE_MAX) ? CANSIG_RSM_VITALS_RSM_STATE_MAX : value;
}

void app_canTx_RSM_Fault_DummyFault_set(bool value)
{
    tx_table.RSM_Faults_signals.RSM_Fault_DummyFault_value = value;
}

void app_canTx_RSM_Fault_DummyFaultCount_set(uint32_t value)
{
    tx_table.RSM_FaultsCounts_signals.RSM_Fault_DummyFaultCount_value = (value > CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_MAX) ? CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_MAX : value;
}

bool app_canTx_RSM_Heartbeat_get()
{
    return tx_table.RSM_Vitals_signals.RSM_Heartbeat_value;
}

RsmStates app_canTx_RSM_State_get()
{
    return tx_table.RSM_Vitals_signals.RSM_State_value;
}

bool app_canTx_RSM_Fault_DummyFault_get()
{
    return tx_table.RSM_Faults_signals.RSM_Fault_DummyFault_value;
}

uint32_t app_canTx_RSM_Fault_DummyFaultCount_get()
{
    return tx_table.RSM_FaultsCounts_signals.RSM_Fault_DummyFaultCount_value;
}

const RSM_Vitals_Signals* app_canTx_RSM_Vitals_getData()
{
    return &tx_table.RSM_Vitals_signals;
}

const RSM_Warnings_Signals* app_canTx_RSM_Warnings_getData()
{
    return &tx_table.RSM_Warnings_signals;
}

const RSM_Faults_Signals* app_canTx_RSM_Faults_getData()
{
    return &tx_table.RSM_Faults_signals;
}

const RSM_WarningsCounts_Signals* app_canTx_RSM_WarningsCounts_getData()
{
    return &tx_table.RSM_WarningsCounts_signals;
}

const RSM_FaultsCounts_Signals* app_canTx_RSM_FaultsCounts_getData()
{
    return &tx_table.RSM_FaultsCounts_signals;
}

