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
 * Update value stored in TX table of signal JCT_Heartbeat in msg JCT_Vitals.
 */
void App_CanTx_JCT_Heartbeat_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_Timestamp in msg JCT_Vitals.
 */
void App_CanTx_JCT_Timestamp_Set(uint32_t value);

/**
 * Update value stored in TX table of signal JCT_WatchdogTimeout in msg JCT_WarningsTest.
 */
void App_CanTx_JCT_WatchdogTimeout_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_BoardOvertemp in msg JCT_WarningsTest.
 */
void App_CanTx_JCT_BoardOvertemp_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_BoardOvervoltage in msg JCT_WarningsTest.
 */
void App_CanTx_JCT_BoardOvervoltage_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_DummyAirShutdown in msg JCT_AirShutdownErrors.
 */
void App_CanTx_JCT_DummyAirShutdown_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_DummyMotorShutdown in msg JCT_MotorShutdownErrors.
 */
void App_CanTx_JCT_DummyMotorShutdown_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_ContactorsClosed in msg JCT_Status.
 */
void App_CanTx_JCT_ContactorsClosed_Set(AirState value);

/**
 * Update value stored in TX table of signal JCT_Current in msg JCT_Status.
 */
void App_CanTx_JCT_Current_Set(float value);

/**
 * Update value stored in TX table of signal JCT_Voltage in msg JCT_Status.
 */
void App_CanTx_JCT_Voltage_Set(float value);

/**
 * Update value stored in TX table of signal JCT_UnsignedTester in msg JCT_Status.
 */
void App_CanTx_JCT_UnsignedTester_Set(int value);

/**
 * Update value stored in TX table of signal JCT_Warning_Warning_Test in msg JCT_Warnings.
 */
void App_CanTx_JCT_Warning_Warning_Test_Set(bool value);

/**
 * Update value stored in TX table of signal JCT_Warning_Warning_TestCount in msg JCT_WarningsCounts.
 */
void App_CanTx_JCT_Warning_Warning_TestCount_Set(uint32_t value);

/**
 * Return value from TX table of signal JCT_Heartbeat in msg JCT_Vitals.
 */
bool App_CanTx_JCT_Heartbeat_Get(void);

/**
 * Return value from TX table of signal JCT_Timestamp in msg JCT_Vitals.
 */
uint32_t App_CanTx_JCT_Timestamp_Get(void);

/**
 * Return value from TX table of signal JCT_WatchdogTimeout in msg JCT_WarningsTest.
 */
bool App_CanTx_JCT_WatchdogTimeout_Get(void);

/**
 * Return value from TX table of signal JCT_BoardOvertemp in msg JCT_WarningsTest.
 */
bool App_CanTx_JCT_BoardOvertemp_Get(void);

/**
 * Return value from TX table of signal JCT_BoardOvervoltage in msg JCT_WarningsTest.
 */
bool App_CanTx_JCT_BoardOvervoltage_Get(void);

/**
 * Return value from TX table of signal JCT_DummyAirShutdown in msg JCT_AirShutdownErrors.
 */
bool App_CanTx_JCT_DummyAirShutdown_Get(void);

/**
 * Return value from TX table of signal JCT_DummyMotorShutdown in msg JCT_MotorShutdownErrors.
 */
bool App_CanTx_JCT_DummyMotorShutdown_Get(void);

/**
 * Return value from TX table of signal JCT_ContactorsClosed in msg JCT_Status.
 */
AirState App_CanTx_JCT_ContactorsClosed_Get(void);

/**
 * Return value from TX table of signal JCT_Current in msg JCT_Status.
 */
float App_CanTx_JCT_Current_Get(void);

/**
 * Return value from TX table of signal JCT_Voltage in msg JCT_Status.
 */
float App_CanTx_JCT_Voltage_Get(void);

/**
 * Return value from TX table of signal JCT_UnsignedTester in msg JCT_Status.
 */
int App_CanTx_JCT_UnsignedTester_Get(void);

/**
 * Return value from TX table of signal JCT_Warning_Warning_Test in msg JCT_Warnings.
 */
bool App_CanTx_JCT_Warning_Warning_Test_Get(void);

/**
 * Return value from TX table of signal JCT_Warning_Warning_TestCount in msg JCT_WarningsCounts.
 */
uint32_t App_CanTx_JCT_Warning_Warning_TestCount_Get(void);

/**
 * Returns pointer to the struct of msg JCT_Vitals signals in the TX table.
 */
const JCT_Vitals_Signals* App_CanTx_JCT_Vitals_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_WarningsTest signals in the TX table.
 */
const JCT_WarningsTest_Signals* App_CanTx_JCT_WarningsTest_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_AirShutdownErrors signals in the TX table.
 */
const JCT_AirShutdownErrors_Signals* App_CanTx_JCT_AirShutdownErrors_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_MotorShutdownErrors signals in the TX table.
 */
const JCT_MotorShutdownErrors_Signals* App_CanTx_JCT_MotorShutdownErrors_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_Status signals in the TX table.
 */
const JCT_Status_Signals* App_CanTx_JCT_Status_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_Warnings signals in the TX table.
 */
const JCT_Warnings_Signals* App_CanTx_JCT_Warnings_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_Faults signals in the TX table.
 */
const JCT_Faults_Signals* App_CanTx_JCT_Faults_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_WarningsCounts signals in the TX table.
 */
const JCT_WarningsCounts_Signals* App_CanTx_JCT_WarningsCounts_GetData(void);

/**
 * Returns pointer to the struct of msg JCT_FaultsCounts signals in the TX table.
 */
const JCT_FaultsCounts_Signals* App_CanTx_JCT_FaultsCounts_GetData(void);

