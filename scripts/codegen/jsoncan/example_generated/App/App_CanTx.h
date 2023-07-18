/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "App_CanUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize TX signals to their starting values.
 */
void App_CanTx_Init(void);

/**
 * Update value stored in TX table of signal Heartbeat in msg JctVitals.
 */
void App_CanTx_JctVitals_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal Timestamp in msg JctVitals.
 */
void App_CanTx_JctVitals_Timestamp_Set(uint32_t value);

/**
 * Update value stored in TX table of signal WatchdogTimeout in msg JctWarnings.
 */
void App_CanTx_JctWarnings_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal BoardOvertemp in msg JctWarnings.
 */
void App_CanTx_JctWarnings_BoardOvertemp_Set(bool value);

/**
 * Update value stored in TX table of signal BoardOvervoltage in msg JctWarnings.
 */
void App_CanTx_JctWarnings_BoardOvervoltage_Set(bool value);

/**
 * Update value stored in TX table of signal DummyAirShutdown in msg JctAirShutdownErrors.
 */
void App_CanTx_JctAirShutdownErrors_DummyAirShutdown_Set(bool value);

/**
 * Update value stored in TX table of signal DummyMotorShutdown in msg JctMotorShutdownErrors.
 */
void App_CanTx_JctMotorShutdownErrors_DummyMotorShutdown_Set(bool value);

/**
 * Update value stored in TX table of signal ContactorsClosed in msg JctStatus.
 */
void App_CanTx_JctStatus_ContactorsClosed_Set(AirState value);

/**
 * Update value stored in TX table of signal Current in msg JctStatus.
 */
void App_CanTx_JctStatus_Current_Set(float value);

/**
 * Update value stored in TX table of signal Voltage in msg JctStatus.
 */
void App_CanTx_JctStatus_Voltage_Set(float value);

/**
 * Update value stored in TX table of signal UnsignedTester in msg JctStatus.
 */
void App_CanTx_JctStatus_UnsignedTester_Set(int value);

/**
 * Update value stored in TX table of signal JCT_TEST in msg JCT_Warnings.
 */
void App_CanTx_JCT_Warnings_JCT_TEST_Set(bool value);

/**
 * Return value from TX table of signal Heartbeat in msg JctVitals.
 */
bool App_CanTx_JctVitals_Heartbeat_Get(void);

/**
 * Return value from TX table of signal Timestamp in msg JctVitals.
 */
uint32_t App_CanTx_JctVitals_Timestamp_Get(void);

/**
 * Return value from TX table of signal WatchdogTimeout in msg JctWarnings.
 */
bool App_CanTx_JctWarnings_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal BoardOvertemp in msg JctWarnings.
 */
bool App_CanTx_JctWarnings_BoardOvertemp_Get(void);

/**
 * Return value from TX table of signal BoardOvervoltage in msg JctWarnings.
 */
bool App_CanTx_JctWarnings_BoardOvervoltage_Get(void);

/**
 * Return value from TX table of signal DummyAirShutdown in msg JctAirShutdownErrors.
 */
bool App_CanTx_JctAirShutdownErrors_DummyAirShutdown_Get(void);

/**
 * Return value from TX table of signal DummyMotorShutdown in msg JctMotorShutdownErrors.
 */
bool App_CanTx_JctMotorShutdownErrors_DummyMotorShutdown_Get(void);

/**
 * Return value from TX table of signal ContactorsClosed in msg JctStatus.
 */
AirState App_CanTx_JctStatus_ContactorsClosed_Get(void);

/**
 * Return value from TX table of signal Current in msg JctStatus.
 */
float App_CanTx_JctStatus_Current_Get(void);

/**
 * Return value from TX table of signal Voltage in msg JctStatus.
 */
float App_CanTx_JctStatus_Voltage_Get(void);

/**
 * Return value from TX table of signal UnsignedTester in msg JctStatus.
 */
int App_CanTx_JctStatus_UnsignedTester_Get(void);

/**
 * Return value from TX table of signal JCT_TEST in msg JCT_Warnings.
 */
bool App_CanTx_JCT_Warnings_JCT_TEST_Get(void);

/**
 * Returns pointer to the struct of msg JctVitals signals in the TX table.
 */
const JctVitals_Signals* App_CanTx_JctVitals_GetData(void);

/**
 * Returns pointer to the struct of msg JctWarnings signals in the TX table.
 */
const JctWarnings_Signals* App_CanTx_JctWarnings_GetData(void);

/**
 * Returns pointer to the struct of msg JctAirShutdownErrors signals in the TX table.
 */
const JctAirShutdownErrors_Signals* App_CanTx_JctAirShutdownErrors_GetData(void);

/**
 * Returns pointer to the struct of msg JctMotorShutdownErrors signals in the TX table.
 */
const JctMotorShutdownErrors_Signals* App_CanTx_JctMotorShutdownErrors_GetData(void);

/**
 * Returns pointer to the struct of msg JctStatus signals in the TX table.
 */
const JctStatus_Signals* App_CanTx_JctStatus_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_Warnings signals in the TX table.
 */
const JCT_Warnings_Signals* App_CanTx_JCT_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_Faults signals in the TX table.
 */
const JCT_Faults_Signals* App_CanTx_JCT_Faults_GetData(void);

