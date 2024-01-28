/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void app_canRx_init(void);

/**
 * Update value stored in RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
void app_canRx_FSM_PappsMappedPedalPercentage_update(float value);

/**
 * Update value stored in RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
void app_canRx_FSM_SappsMappedPedalPercentage_update(float value);

/**
 * Update value stored in RX table of signal FSM_Warning_Warning_Test1 in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_Warning_Test1_update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_Warning_Test2 in msg FSM_Warnings.
 */
void app_canRx_FSM_Warning_Warning_Test2_update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_Fault_Test3 in msg FSM_Faults.
 */
void app_canRx_FSM_Fault_Fault_Test3_update(bool value);

/**
 * Return value from RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
float app_canRx_FSM_PappsMappedPedalPercentage_get(void);

/**
 * Return value from RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
float app_canRx_FSM_SappsMappedPedalPercentage_get(void);

/**
 * Return value from RX table of signal FSM_Warning_Warning_Test1 in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_Warning_Test1_get(void);

/**
 * Return value from RX table of signal FSM_Warning_Warning_Test2 in msg FSM_Warnings.
 */
bool app_canRx_FSM_Warning_Warning_Test2_get(void);

/**
 * Return value from RX table of signal FSM_Fault_Fault_Test3 in msg FSM_Faults.
 */
bool app_canRx_FSM_Fault_Fault_Test3_get(void);

