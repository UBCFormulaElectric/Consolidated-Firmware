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
 * Struct for holding all messages transmitted by JCT (i.e. the TX table).
 */
typedef struct
{
    JCT_Vitals_Signals JCT_Vitals_signals;
    JCT_WarningsTest_Signals JCT_WarningsTest_signals;
    JCT_AirShutdownErrors_Signals JCT_AirShutdownErrors_signals;
    JCT_MotorShutdownErrors_Signals JCT_MotorShutdownErrors_signals;
    JCT_Status_Signals JCT_Status_signals;
    JCT_Warnings_Signals JCT_Warnings_signals;
    JCT_Faults_Signals JCT_Faults_signals;
    JCT_WarningsCounts_Signals JCT_WarningsCounts_signals;
    JCT_FaultsCounts_Signals JCT_FaultsCounts_signals;
} JCT_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static JCT_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canTx_init()
{
    memset(&tx_table, 0, sizeof(JCT_TxMsgs));
    app_canTx_JCT_Heartbeat_set(CANSIG_JCT_VITALS_JCT_HEARTBEAT_START_VAL);
    app_canTx_JCT_Timestamp_set(CANSIG_JCT_VITALS_JCT_TIMESTAMP_START_VAL);
    app_canTx_JCT_WatchdogTimeout_set(CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_START_VAL);
    app_canTx_JCT_BoardOvertemp_set(CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_START_VAL);
    app_canTx_JCT_BoardOvervoltage_set(CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_START_VAL);
    app_canTx_JCT_DummyAirShutdown_set(CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_START_VAL);
    app_canTx_JCT_DummyMotorShutdown_set(CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_START_VAL);
    app_canTx_JCT_ContactorsClosed_set(CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_START_VAL);
    app_canTx_JCT_Current_set(CANSIG_JCT_STATUS_JCT_CURRENT_START_VAL);
    app_canTx_JCT_Voltage_set(CANSIG_JCT_STATUS_JCT_VOLTAGE_START_VAL);
    app_canTx_JCT_UnsignedTester_set(CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_START_VAL);
    app_canTx_JCT_Warning_Warning_Test_set(CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_START_VAL);
    app_canTx_JCT_Warning_Warning_TestCount_set(CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_START_VAL);
}

void app_canTx_JCT_Heartbeat_set(bool value)
{
    tx_table.JCT_Vitals_signals.JCT_Heartbeat_value = value;
}

void app_canTx_JCT_Timestamp_set(uint32_t value)
{
    tx_table.JCT_Vitals_signals.JCT_Timestamp_value = value;
}

void app_canTx_JCT_WatchdogTimeout_set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_WatchdogTimeout_value = value;
}

void app_canTx_JCT_BoardOvertemp_set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_BoardOvertemp_value = value;
}

void app_canTx_JCT_BoardOvervoltage_set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_BoardOvervoltage_value = value;
}

void app_canTx_JCT_DummyAirShutdown_set(bool value)
{
    tx_table.JCT_AirShutdownErrors_signals.JCT_DummyAirShutdown_value = value;
}

void app_canTx_JCT_DummyMotorShutdown_set(bool value)
{
    tx_table.JCT_MotorShutdownErrors_signals.JCT_DummyMotorShutdown_value = value;
}

void app_canTx_JCT_ContactorsClosed_set(AirState value)
{
    tx_table.JCT_Status_signals.JCT_ContactorsClosed_value = (value > CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MAX) ? CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MAX : value;
}

void app_canTx_JCT_Current_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_JCT_CURRENT_MIN ? CANSIG_JCT_STATUS_JCT_CURRENT_MIN : value;
    tx_table.JCT_Status_signals.JCT_Current_value = tmp > CANSIG_JCT_STATUS_JCT_CURRENT_MAX ? CANSIG_JCT_STATUS_JCT_CURRENT_MAX : tmp;
}

void app_canTx_JCT_Voltage_set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_JCT_VOLTAGE_MIN ? CANSIG_JCT_STATUS_JCT_VOLTAGE_MIN : value;
    tx_table.JCT_Status_signals.JCT_Voltage_value = tmp > CANSIG_JCT_STATUS_JCT_VOLTAGE_MAX ? CANSIG_JCT_STATUS_JCT_VOLTAGE_MAX : tmp;
}

void app_canTx_JCT_UnsignedTester_set(int value)
{
    const int tmp = value < CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MIN ? CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MIN : value;
    tx_table.JCT_Status_signals.JCT_UnsignedTester_value = tmp > CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MAX ? CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MAX : tmp;
}

void app_canTx_JCT_Warning_Warning_Test_set(bool value)
{
    tx_table.JCT_Warnings_signals.JCT_Warning_Warning_Test_value = value;
}

void app_canTx_JCT_Warning_Warning_TestCount_set(uint32_t value)
{
    tx_table.JCT_WarningsCounts_signals.JCT_Warning_Warning_TestCount_value = (value > CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MAX) ? CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MAX : value;
}

bool app_canTx_JCT_Heartbeat_get()
{
    return tx_table.JCT_Vitals_signals.JCT_Heartbeat_value;
}

uint32_t app_canTx_JCT_Timestamp_get()
{
    return tx_table.JCT_Vitals_signals.JCT_Timestamp_value;
}

bool app_canTx_JCT_WatchdogTimeout_get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_WatchdogTimeout_value;
}

bool app_canTx_JCT_BoardOvertemp_get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_BoardOvertemp_value;
}

bool app_canTx_JCT_BoardOvervoltage_get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_BoardOvervoltage_value;
}

bool app_canTx_JCT_DummyAirShutdown_get()
{
    return tx_table.JCT_AirShutdownErrors_signals.JCT_DummyAirShutdown_value;
}

bool app_canTx_JCT_DummyMotorShutdown_get()
{
    return tx_table.JCT_MotorShutdownErrors_signals.JCT_DummyMotorShutdown_value;
}

AirState app_canTx_JCT_ContactorsClosed_get()
{
    return tx_table.JCT_Status_signals.JCT_ContactorsClosed_value;
}

float app_canTx_JCT_Current_get()
{
    return tx_table.JCT_Status_signals.JCT_Current_value;
}

float app_canTx_JCT_Voltage_get()
{
    return tx_table.JCT_Status_signals.JCT_Voltage_value;
}

int app_canTx_JCT_UnsignedTester_get()
{
    return tx_table.JCT_Status_signals.JCT_UnsignedTester_value;
}

bool app_canTx_JCT_Warning_Warning_Test_get()
{
    return tx_table.JCT_Warnings_signals.JCT_Warning_Warning_Test_value;
}

uint32_t app_canTx_JCT_Warning_Warning_TestCount_get()
{
    return tx_table.JCT_WarningsCounts_signals.JCT_Warning_Warning_TestCount_value;
}

const JCT_Vitals_Signals* app_canTx_JCT_Vitals_getData()
{
    return &tx_table.JCT_Vitals_signals;
}

const JCT_WarningsTest_Signals* app_canTx_JCT_WarningsTest_getData()
{
    return &tx_table.JCT_WarningsTest_signals;
}

const JCT_AirShutdownErrors_Signals* app_canTx_JCT_AirShutdownErrors_getData()
{
    return &tx_table.JCT_AirShutdownErrors_signals;
}

const JCT_MotorShutdownErrors_Signals* app_canTx_JCT_MotorShutdownErrors_getData()
{
    return &tx_table.JCT_MotorShutdownErrors_signals;
}

const JCT_Status_Signals* app_canTx_JCT_Status_getData()
{
    return &tx_table.JCT_Status_signals;
}

const JCT_Warnings_Signals* app_canTx_JCT_Warnings_getData()
{
    return &tx_table.JCT_Warnings_signals;
}

const JCT_Faults_Signals* app_canTx_JCT_Faults_getData()
{
    return &tx_table.JCT_Faults_signals;
}

const JCT_WarningsCounts_Signals* app_canTx_JCT_WarningsCounts_getData()
{
    return &tx_table.JCT_WarningsCounts_signals;
}

const JCT_FaultsCounts_Signals* app_canTx_JCT_FaultsCounts_getData()
{
    return &tx_table.JCT_FaultsCounts_signals;
}

