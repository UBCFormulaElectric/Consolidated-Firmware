/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize RX signals to their starting values.
 */
void App_CanRx_Init(void);

/**
 * Update value stored in RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_PappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
void App_CanRx_FSM_SappsMappedPedalPercentage_Update(float value);

/**
 * Update value stored in RX table of signal FSM_Warning_Warning_Test1 in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_Warning_Test1_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Warning_Warning_Test2 in msg FSM_Warnings.
 */
void App_CanRx_FSM_Warning_Warning_Test2_Update(bool value);

/**
 * Update value stored in RX table of signal FSM_Fault_Fault_Test3 in msg FSM_Faults.
 */
void App_CanRx_FSM_Fault_Fault_Test3_Update(bool value);

/**
 * Return value from RX table of signal FSM_PappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_PappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_SappsMappedPedalPercentage in msg FSM_Apps.
 */
float App_CanRx_FSM_SappsMappedPedalPercentage_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_Warning_Test1 in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_Warning_Test1_Get(void);

/**
 * Return value from RX table of signal FSM_Warning_Warning_Test2 in msg FSM_Warnings.
 */
bool App_CanRx_FSM_Warning_Warning_Test2_Get(void);

/**
 * Return value from RX table of signal FSM_Fault_Fault_Test3 in msg FSM_Faults.
 */
bool App_CanRx_FSM_Fault_Fault_Test3_Get(void);

