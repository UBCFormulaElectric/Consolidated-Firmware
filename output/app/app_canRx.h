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
 * Update value stored in RX table of signal BRUSA_IsConnected in msg BRUSA_Vitals.
 */
void app_canRx_BRUSA_IsConnected_update(bool value);

/**
 * Update value stored in RX table of signal VC_Heartbeat in msg VC_Vitals.
 */
void app_canRx_VC_Heartbeat_update(bool value);/**
 * Update value stored in RX table of signal VC_State in msg VC_Vitals.
 */
void app_canRx_VC_State_update(VcState value);/**
 * Update value stored in RX table of signal VC_BuzzerOn in msg VC_Vitals.
 */
void app_canRx_VC_BuzzerOn_update(bool value);/**
 * Update value stored in RX table of signal VC_NumberOfCanDataLogs in msg VC_Vitals.
 */
void app_canRx_VC_NumberOfCanDataLogs_update(uint32_t value);/**
 * Update value stored in RX table of signal VC_CanLoggingRemainingErrors in msg VC_Vitals.
 */
void app_canRx_VC_CanLoggingRemainingErrors_update(uint32_t value);


/**
 * Get value stored in RX table of signal VC_Heartbeat in msg VC_Vitals.
 */
bool app_canRx_VC_Heartbeat_get(void);/**
 * Get value stored in RX table of signal VC_State in msg VC_Vitals.
 */
VcState app_canRx_VC_State_get(void);/**
 * Get value stored in RX table of signal VC_BuzzerOn in msg VC_Vitals.
 */
bool app_canRx_VC_BuzzerOn_get(void);/**
 * Get value stored in RX table of signal VC_NumberOfCanDataLogs in msg VC_Vitals.
 */
uint32_t app_canRx_VC_NumberOfCanDataLogs_get(void);/**
 * Get value stored in RX table of signal VC_CanLoggingRemainingErrors in msg VC_Vitals.
 */
uint32_t app_canRx_VC_CanLoggingRemainingErrors_get(void);
