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
 * Update value stored in TX table of signal JCT_Heartbeat in msg JCT_Vitals.
 */
void app_canTx_JCT_Heartbeat_set(bool value);

/**
 * Update value stored in TX table of signal JCT_Timestamp in msg JCT_Vitals.
 */
void app_canTx_JCT_Timestamp_set(uint32_t value);

/**
 * Update value stored in TX table of signal JCT_WatchdogTimeout in msg JCT_WarningsTest.
 */
void app_canTx_JCT_WatchdogTimeout_set(bool value);

/**
 * Update value stored in TX table of signal JCT_BoardOvertemp in msg JCT_WarningsTest.
 */
void app_canTx_JCT_BoardOvertemp_set(bool value);

/**
 * Update value stored in TX table of signal JCT_BoardOvervoltage in msg JCT_WarningsTest.
 */
void app_canTx_JCT_BoardOvervoltage_set(bool value);

/**
 * Update value stored in TX table of signal JCT_DummyAirShutdown in msg JCT_AirShutdownErrors.
 */
void app_canTx_JCT_DummyAirShutdown_set(bool value);

/**
 * Update value stored in TX table of signal JCT_DummyMotorShutdown in msg JCT_MotorShutdownErrors.
 */
void app_canTx_JCT_DummyMotorShutdown_set(bool value);

/**
 * Update value stored in TX table of signal JCT_ContactorsClosed in msg JCT_Status.
 */
void app_canTx_JCT_ContactorsClosed_set(AirState value);

/**
 * Update value stored in TX table of signal JCT_Current in msg JCT_Status.
 */
void app_canTx_JCT_Current_set(float value);

/**
 * Update value stored in TX table of signal JCT_Voltage in msg JCT_Status.
 */
void app_canTx_JCT_Voltage_set(float value);

/**
 * Update value stored in TX table of signal JCT_UnsignedTester in msg JCT_Status.
 */
void app_canTx_JCT_UnsignedTester_set(uint32_t value);

/**
 * Update value stored in TX table of signal JCT_Warning_Warning_Test in msg JCT_Warnings.
 */
void app_canTx_JCT_Warning_Warning_Test_set(bool value);

/**
 * Update value stored in TX table of signal JCT_Warning_Warning_TestCount in msg JCT_WarningsCounts.
 */
void app_canTx_JCT_Warning_Warning_TestCount_set(uint32_t value);

/**
 * Return value from TX table of signal JCT_Heartbeat in msg JCT_Vitals.
 */
bool app_canTx_JCT_Heartbeat_get(void);

/**
 * Return value from TX table of signal JCT_Timestamp in msg JCT_Vitals.
 */
uint32_t app_canTx_JCT_Timestamp_get(void);

/**
 * Return value from TX table of signal JCT_WatchdogTimeout in msg JCT_WarningsTest.
 */
bool app_canTx_JCT_WatchdogTimeout_get(void);

/**
 * Return value from TX table of signal JCT_BoardOvertemp in msg JCT_WarningsTest.
 */
bool app_canTx_JCT_BoardOvertemp_get(void);

/**
 * Return value from TX table of signal JCT_BoardOvervoltage in msg JCT_WarningsTest.
 */
bool app_canTx_JCT_BoardOvervoltage_get(void);

/**
 * Return value from TX table of signal JCT_DummyAirShutdown in msg JCT_AirShutdownErrors.
 */
bool app_canTx_JCT_DummyAirShutdown_get(void);

/**
 * Return value from TX table of signal JCT_DummyMotorShutdown in msg JCT_MotorShutdownErrors.
 */
bool app_canTx_JCT_DummyMotorShutdown_get(void);

/**
 * Return value from TX table of signal JCT_ContactorsClosed in msg JCT_Status.
 */
AirState app_canTx_JCT_ContactorsClosed_get(void);

/**
 * Return value from TX table of signal JCT_Current in msg JCT_Status.
 */
float app_canTx_JCT_Current_get(void);

/**
 * Return value from TX table of signal JCT_Voltage in msg JCT_Status.
 */
float app_canTx_JCT_Voltage_get(void);

/**
 * Return value from TX table of signal JCT_UnsignedTester in msg JCT_Status.
 */
uint32_t app_canTx_JCT_UnsignedTester_get(void);

/**
 * Return value from TX table of signal JCT_Warning_Warning_Test in msg JCT_Warnings.
 */
bool app_canTx_JCT_Warning_Warning_Test_get(void);

/**
 * Return value from TX table of signal JCT_Warning_Warning_TestCount in msg JCT_WarningsCounts.
 */
uint32_t app_canTx_JCT_Warning_Warning_TestCount_get(void);

/**
 * Returns pointer to the struct of msg JCT_Vitals signals in the TX table.
 */
const JCT_Vitals_Signals* app_canTx_JCT_Vitals_getData(void);

/**
 * Returns pointer to the struct of msg JCT_WarningsTest signals in the TX table.
 */
const JCT_WarningsTest_Signals* app_canTx_JCT_WarningsTest_getData(void);

/**
 * Returns pointer to the struct of msg JCT_AirShutdownErrors signals in the TX table.
 */
const JCT_AirShutdownErrors_Signals* app_canTx_JCT_AirShutdownErrors_getData(void);

/**
 * Returns pointer to the struct of msg JCT_MotorShutdownErrors signals in the TX table.
 */
const JCT_MotorShutdownErrors_Signals* app_canTx_JCT_MotorShutdownErrors_getData(void);

/**
 * Returns pointer to the struct of msg JCT_Status signals in the TX table.
 */
const JCT_Status_Signals* app_canTx_JCT_Status_getData(void);

/**
 * Returns pointer to the struct of msg JCT_Warnings signals in the TX table.
 */
const JCT_Warnings_Signals* app_canTx_JCT_Warnings_getData(void);

/**
 * Returns pointer to the struct of msg JCT_Faults signals in the TX table.
 */
const JCT_Faults_Signals* app_canTx_JCT_Faults_getData(void);

/**
 * Returns pointer to the struct of msg JCT_WarningsCounts signals in the TX table.
 */
const JCT_WarningsCounts_Signals* app_canTx_JCT_WarningsCounts_getData(void);

/**
 * Returns pointer to the struct of msg JCT_FaultsCounts signals in the TX table.
 */
const JCT_FaultsCounts_Signals* app_canTx_JCT_FaultsCounts_getData(void);

