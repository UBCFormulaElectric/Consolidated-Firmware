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
    JctVitals_Signals JctVitals_signals;
    JctWarnings_Signals JctWarnings_signals;
    JctAirShutdownErrors_Signals JctAirShutdownErrors_signals;
    JctMotorShutdownErrors_Signals JctMotorShutdownErrors_signals;
    JctStatus_Signals JctStatus_signals;
    JCT_Warnings_Signals JCT_Warnings_signals;
    JCT_Faults_Signals JCT_Faults_signals;
} JCT_TxMsgs;

/* -------------------------- Private Variables --------------------------- */

static JCT_TxMsgs tx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanTx_Init()
{
    memset(&tx_table, 0, sizeof(JCT_TxMsgs));
    App_CanTx_JctVitals_Heartbeat_Set(CANSIG_JCT_VITALS_HEARTBEAT_START_VAL);
    App_CanTx_JctVitals_Timestamp_Set(CANSIG_JCT_VITALS_TIMESTAMP_START_VAL);
    App_CanTx_JctWarnings_WatchdogTimeout_Set(CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_START_VAL);
    App_CanTx_JctWarnings_BoardOvertemp_Set(CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_START_VAL);
    App_CanTx_JctWarnings_BoardOvervoltage_Set(CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_START_VAL);
    App_CanTx_JctAirShutdownErrors_DummyAirShutdown_Set(CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_START_VAL);
    App_CanTx_JctMotorShutdownErrors_DummyMotorShutdown_Set(CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_START_VAL);
    App_CanTx_JctStatus_ContactorsClosed_Set(CANSIG_JCT_STATUS_CONTACTORS_CLOSED_START_VAL);
    App_CanTx_JctStatus_Current_Set(CANSIG_JCT_STATUS_CURRENT_START_VAL);
    App_CanTx_JctStatus_Voltage_Set(CANSIG_JCT_STATUS_VOLTAGE_START_VAL);
    App_CanTx_JctStatus_UnsignedTester_Set(CANSIG_JCT_STATUS_UNSIGNED_TESTER_START_VAL);
    App_CanTx_JCT_Warnings_JCT_TEST_Set(CANSIG_JCT_WARNINGS_JCT_TEST_START_VAL);
}

void App_CanTx_JctVitals_Heartbeat_Set(bool value)
{
    tx_table.JctVitals_signals.Heartbeat_value = value;
}

void App_CanTx_JctVitals_Timestamp_Set(uint32_t value)
{
    tx_table.JctVitals_signals.Timestamp_value = value;
}

void App_CanTx_JctWarnings_WatchdogTimeout_Set(bool value)
{
    tx_table.JctWarnings_signals.WatchdogTimeout_value = value;
}

void App_CanTx_JctWarnings_BoardOvertemp_Set(bool value)
{
    tx_table.JctWarnings_signals.BoardOvertemp_value = value;
}

void App_CanTx_JctWarnings_BoardOvervoltage_Set(bool value)
{
    tx_table.JctWarnings_signals.BoardOvervoltage_value = value;
}

void App_CanTx_JctAirShutdownErrors_DummyAirShutdown_Set(bool value)
{
    tx_table.JctAirShutdownErrors_signals.DummyAirShutdown_value = value;
}

void App_CanTx_JctMotorShutdownErrors_DummyMotorShutdown_Set(bool value)
{
    tx_table.JctMotorShutdownErrors_signals.DummyMotorShutdown_value = value;
}

void App_CanTx_JctStatus_ContactorsClosed_Set(AirState value)
{
    tx_table.JctStatus_signals.ContactorsClosed_value = (value > CANSIG_JCT_STATUS_CONTACTORS_CLOSED_MAX) ? CANSIG_JCT_STATUS_CONTACTORS_CLOSED_MAX : value;
}

void App_CanTx_JctStatus_Current_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_CURRENT_MIN ? CANSIG_JCT_STATUS_CURRENT_MIN : value;
    tx_table.JctStatus_signals.Current_value = tmp > CANSIG_JCT_STATUS_CURRENT_MAX ? CANSIG_JCT_STATUS_CURRENT_MAX : tmp;
}

void App_CanTx_JctStatus_Voltage_Set(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_JCT_STATUS_VOLTAGE_MIN ? CANSIG_JCT_STATUS_VOLTAGE_MIN : value;
    tx_table.JctStatus_signals.Voltage_value = tmp > CANSIG_JCT_STATUS_VOLTAGE_MAX ? CANSIG_JCT_STATUS_VOLTAGE_MAX : tmp;
}

void App_CanTx_JctStatus_UnsignedTester_Set(int value)
{
    const int tmp = value < CANSIG_JCT_STATUS_UNSIGNED_TESTER_MIN ? CANSIG_JCT_STATUS_UNSIGNED_TESTER_MIN : value;
    tx_table.JctStatus_signals.UnsignedTester_value = tmp > CANSIG_JCT_STATUS_UNSIGNED_TESTER_MAX ? CANSIG_JCT_STATUS_UNSIGNED_TESTER_MAX : tmp;
}

void App_CanTx_JCT_Warnings_JCT_TEST_Set(bool value)
{
    tx_table.JCT_Warnings_signals.JCT_TEST_value = value;
}

bool App_CanTx_JctVitals_Heartbeat_Get()
{
    return tx_table.JctVitals_signals.Heartbeat_value;
}

uint32_t App_CanTx_JctVitals_Timestamp_Get()
{
    return tx_table.JctVitals_signals.Timestamp_value;
}

bool App_CanTx_JctWarnings_WatchdogTimeout_Get()
{
    return tx_table.JctWarnings_signals.WatchdogTimeout_value;
}

bool App_CanTx_JctWarnings_BoardOvertemp_Get()
{
    return tx_table.JctWarnings_signals.BoardOvertemp_value;
}

bool App_CanTx_JctWarnings_BoardOvervoltage_Get()
{
    return tx_table.JctWarnings_signals.BoardOvervoltage_value;
}

bool App_CanTx_JctAirShutdownErrors_DummyAirShutdown_Get()
{
    return tx_table.JctAirShutdownErrors_signals.DummyAirShutdown_value;
}

bool App_CanTx_JctMotorShutdownErrors_DummyMotorShutdown_Get()
{
    return tx_table.JctMotorShutdownErrors_signals.DummyMotorShutdown_value;
}

AirState App_CanTx_JctStatus_ContactorsClosed_Get()
{
    return tx_table.JctStatus_signals.ContactorsClosed_value;
}

float App_CanTx_JctStatus_Current_Get()
{
    return tx_table.JctStatus_signals.Current_value;
}

float App_CanTx_JctStatus_Voltage_Get()
{
    return tx_table.JctStatus_signals.Voltage_value;
}

int App_CanTx_JctStatus_UnsignedTester_Get()
{
    return tx_table.JctStatus_signals.UnsignedTester_value;
}

bool App_CanTx_JCT_Warnings_JCT_TEST_Get()
{
    return tx_table.JCT_Warnings_signals.JCT_TEST_value;
}

const JctVitals_Signals* App_CanTx_JctVitals_GetData()
{
    return &tx_table.JctVitals_signals;
}

const JctWarnings_Signals* App_CanTx_JctWarnings_GetData()
{
    return &tx_table.JctWarnings_signals;
}

const JctAirShutdownErrors_Signals* App_CanTx_JctAirShutdownErrors_GetData()
{
    return &tx_table.JctAirShutdownErrors_signals;
}

const JctMotorShutdownErrors_Signals* App_CanTx_JctMotorShutdownErrors_GetData()
{
    return &tx_table.JctMotorShutdownErrors_signals;
}

const JctStatus_Signals* App_CanTx_JctStatus_GetData()
{
    return &tx_table.JctStatus_signals;
}

const JCT_Warnings_Signals* App_CanTx_JCT_Warnings_GetData()
{
    return &tx_table.JCT_Warnings_signals;
}

const JCT_Faults_Signals* App_CanTx_JCT_Faults_GetData()
{
    return &tx_table.JCT_Faults_signals;
}

