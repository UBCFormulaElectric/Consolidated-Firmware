/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize TX signals to their starting values.
 */
void app_canTx_init(void);

/**
 * Update value stored in TX table of signal RSM_Heartbeat in msg RSM_Vitals.
 */
void app_canTx_RSM_Heartbeat_set(bool value);

/**
 * Update value stored in TX table of signal RSM_State in msg RSM_Vitals.
 */
void app_canTx_RSM_State_set(RsmStates value);

/**
 * Update value stored in TX table of signal RSM_Fault_DummyFault in msg RSM_Faults.
 */
void app_canTx_RSM_Fault_DummyFault_set(bool value);

/**
 * Update value stored in TX table of signal RSM_Fault_DummyFaultCount in msg RSM_FaultsCounts.
 */
void app_canTx_RSM_Fault_DummyFaultCount_set(uint32_t value);

/**
 * Return value from TX table of signal RSM_Heartbeat in msg RSM_Vitals.
 */
bool app_canTx_RSM_Heartbeat_get(void);

/**
 * Return value from TX table of signal RSM_State in msg RSM_Vitals.
 */
RsmStates app_canTx_RSM_State_get(void);

/**
 * Return value from TX table of signal RSM_Fault_DummyFault in msg RSM_Faults.
 */
bool app_canTx_RSM_Fault_DummyFault_get(void);

/**
 * Return value from TX table of signal RSM_Fault_DummyFaultCount in msg RSM_FaultsCounts.
 */
uint32_t app_canTx_RSM_Fault_DummyFaultCount_get(void);

/**
 * Returns pointer to the struct of msg RSM_Vitals signals in the TX table.
 */
const RSM_Vitals_Signals* app_canTx_RSM_Vitals_getData(void);

/**
 * Returns pointer to the struct of msg RSM_Warnings signals in the TX table.
 */
const RSM_Warnings_Signals* app_canTx_RSM_Warnings_getData(void);

/**
 * Returns pointer to the struct of msg RSM_Faults signals in the TX table.
 */
const RSM_Faults_Signals* app_canTx_RSM_Faults_getData(void);

/**
 * Returns pointer to the struct of msg RSM_WarningsCounts signals in the TX table.
 */
const RSM_WarningsCounts_Signals* app_canTx_RSM_WarningsCounts_getData(void);

/**
 * Returns pointer to the struct of msg RSM_FaultsCounts signals in the TX table.
 */
const RSM_FaultsCounts_Signals* app_canTx_RSM_FaultsCounts_getData(void);

