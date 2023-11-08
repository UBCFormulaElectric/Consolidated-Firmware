/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanTx.h"


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

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(JCT_TxMsgs));
    App_CanTx_JCT_Heartbeat_Set(CANSIG_JCT_VITALS_JCT_HEARTBEAT_START_VAL);
    App_CanTx_JCT_Timestamp_Set(CANSIG_JCT_VITALS_JCT_TIMESTAMP_START_VAL);
    App_CanTx_JCT_WatchdogTimeout_Set(CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_JCT_BoardOvertemp_Set(CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_START_VAL);
    App_CanTx_JCT_BoardOvervoltage_Set(CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_START_VAL);
    App_CanTx_JCT_DummyAirShutdown_Set(CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_START_VAL);
    App_CanTx_JCT_DummyMotorShutdown_Set(CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_START_VAL);
    App_CanTx_JCT_ContactorsClosed_Set(CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_START_VAL);
    App_CanTx_JCT_Current_Set(CANSIG_JCT_STATUS_JCT_CURRENT_START_VAL);
    App_CanTx_JCT_Voltage_Set(CANSIG_JCT_STATUS_JCT_VOLTAGE_START_VAL);
    App_CanTx_JCT_UnsignedTester_Set(CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_START_VAL);
    App_CanTx_JCT_Warning_Warning_Test_Set(CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_START_VAL);
    App_CanTx_JCT_Warning_Warning_TestCount_Set(CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_START_VAL);
}

void App_CanTx_JCT_Heartbeat_Set(bool value)
{
    tx_table.JCT_Vitals_signals.JCT_Heartbeat_value = value;
}

void App_CanTx_JCT_Timestamp_Set(uint32_t value)
{
    tx_table.JCT_Vitals_signals.JCT_Timestamp_value = value;
}

void App_CanTx_JCT_WatchdogTimeout_Set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_WatchdogTimeout_value = value;
}

void App_CanTx_JCT_BoardOvertemp_Set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_BoardOvertemp_value = value;
}

void App_CanTx_JCT_BoardOvervoltage_Set(bool value)
{
    tx_table.JCT_WarningsTest_signals.JCT_BoardOvervoltage_value = value;
}

void App_CanTx_JCT_DummyAirShutdown_Set(bool value)
{
    tx_table.JCT_AirShutdownErrors_signals.JCT_DummyAirShutdown_value = value;
}

void App_CanTx_JCT_DummyMotorShutdown_Set(bool value)
{
    tx_table.JCT_MotorShutdownErrors_signals.JCT_DummyMotorShutdown_value = value;
}

void App_CanTx_JCT_ContactorsClosed_Set(AirState value)
{
    tx_table.JCT_Status_signals.JCT_ContactorsClosed_value = (value > CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MAX) ? CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MAX : value;
}

void App_CanTx_JCT_Current_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_JCT_CURRENT_MIN ? CANSIG_JCT_STATUS_JCT_CURRENT_MIN : value;
    tx_table.JCT_Status_signals.JCT_Current_value = tmp > CANSIG_JCT_STATUS_JCT_CURRENT_MAX ? CANSIG_JCT_STATUS_JCT_CURRENT_MAX : tmp;
}

void App_CanTx_JCT_Voltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_JCT_VOLTAGE_MIN ? CANSIG_JCT_STATUS_JCT_VOLTAGE_MIN : value;
    tx_table.JCT_Status_signals.JCT_Voltage_value = tmp > CANSIG_JCT_STATUS_JCT_VOLTAGE_MAX ? CANSIG_JCT_STATUS_JCT_VOLTAGE_MAX : tmp;
}

void App_CanTx_JCT_UnsignedTester_Set(int value)
{
    const int tmp = value < CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MIN ? CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MIN : value;
    tx_table.JCT_Status_signals.JCT_UnsignedTester_value = tmp > CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MAX ? CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MAX : tmp;
}

void App_CanTx_JCT_Warning_Warning_Test_Set(bool value)
{
    tx_table.JCT_Warnings_signals.JCT_Warning_Warning_Test_value = value;
}

void App_CanTx_JCT_Warning_Warning_TestCount_Set(uint32_t value)
{
    tx_table.JCT_WarningsCounts_signals.JCT_Warning_Warning_TestCount_value = (value > CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MAX) ? CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MAX : value;
}

bool App_CanTx_JCT_Heartbeat_Get()
{
    return tx_table.JCT_Vitals_signals.JCT_Heartbeat_value;
}

uint32_t App_CanTx_JCT_Timestamp_Get()
{
    return tx_table.JCT_Vitals_signals.JCT_Timestamp_value;
}

bool App_CanTx_JCT_WatchdogTimeout_Get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_WatchdogTimeout_value;
}

bool App_CanTx_JCT_BoardOvertemp_Get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_BoardOvertemp_value;
}

bool App_CanTx_JCT_BoardOvervoltage_Get()
{
    return tx_table.JCT_WarningsTest_signals.JCT_BoardOvervoltage_value;
}

bool App_CanTx_JCT_DummyAirShutdown_Get()
{
    return tx_table.JCT_AirShutdownErrors_signals.JCT_DummyAirShutdown_value;
}

bool App_CanTx_JCT_DummyMotorShutdown_Get()
{
    return tx_table.JCT_MotorShutdownErrors_signals.JCT_DummyMotorShutdown_value;
}

AirState App_CanTx_JCT_ContactorsClosed_Get()
{
    return tx_table.JCT_Status_signals.JCT_ContactorsClosed_value;
}

float App_CanTx_JCT_Current_Get()
{
    return tx_table.JCT_Status_signals.JCT_Current_value;
}

float App_CanTx_JCT_Voltage_Get()
{
    return tx_table.JCT_Status_signals.JCT_Voltage_value;
}

int App_CanTx_JCT_UnsignedTester_Get()
{
    return tx_table.JCT_Status_signals.JCT_UnsignedTester_value;
}

bool App_CanTx_JCT_Warning_Warning_Test_Get()
{
    return tx_table.JCT_Warnings_signals.JCT_Warning_Warning_Test_value;
}

uint32_t App_CanTx_JCT_Warning_Warning_TestCount_Get()
{
    return tx_table.JCT_WarningsCounts_signals.JCT_Warning_Warning_TestCount_value;
}

const JCT_Vitals_Signals* App_CanTx_JCT_Vitals_GetData()
{
    return &tx_table.JCT_Vitals_signals;
}

const JCT_WarningsTest_Signals* App_CanTx_JCT_WarningsTest_GetData()
{
    return &tx_table.JCT_WarningsTest_signals;
}

const JCT_AirShutdownErrors_Signals* App_CanTx_JCT_AirShutdownErrors_GetData()
{
    return &tx_table.JCT_AirShutdownErrors_signals;
}

const JCT_MotorShutdownErrors_Signals* App_CanTx_JCT_MotorShutdownErrors_GetData()
{
    return &tx_table.JCT_MotorShutdownErrors_signals;
}

const JCT_Status_Signals* App_CanTx_JCT_Status_GetData()
{
    return &tx_table.JCT_Status_signals;
}

const JCT_Warnings_Signals* App_CanTx_JCT_Warnings_GetData()
{
    return &tx_table.JCT_Warnings_signals;
}

const JCT_Faults_Signals* App_CanTx_JCT_Faults_GetData()
{
    return &tx_table.JCT_Faults_signals;
}

const JCT_WarningsCounts_Signals* App_CanTx_JCT_WarningsCounts_GetData()
{
    return &tx_table.JCT_WarningsCounts_signals;
}

const JCT_FaultsCounts_Signals* App_CanTx_JCT_FaultsCounts_GetData()
{
    return &tx_table.JCT_FaultsCounts_signals;
}

