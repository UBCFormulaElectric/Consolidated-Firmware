/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------- Macros -------------------------------- */

// Msg IDs
#define CAN_MSG_JCT_VITALS_ID (601)
#define CAN_MSG_JCT_WARNINGS_TEST_ID (602)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_ID (603)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_ID (604)
#define CAN_MSG_JCT_STATUS_ID (608)
#define CAN_MSG_JCT_WARNINGS_ID (51)
#define CAN_MSG_JCT_FAULTS_ID (52)
#define CAN_MSG_JCT_WARNINGS_COUNTS_ID (53)
#define CAN_MSG_JCT_FAULTS_COUNTS_ID (54)
#define CAN_MSG_FSM_APPS_ID (151)
#define CAN_MSG_FSM_WARNINGS_ID (41)
#define CAN_MSG_FSM_FAULTS_ID (42)

// Msg lengths (in bytes)
#define CAN_MSG_JCT_VITALS_BYTES (5)
#define CAN_MSG_JCT_WARNINGS_TEST_BYTES (1)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_BYTES (1)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_BYTES (1)
#define CAN_MSG_JCT_STATUS_BYTES (5)
#define CAN_MSG_JCT_WARNINGS_BYTES (1)
#define CAN_MSG_JCT_FAULTS_BYTES (0)
#define CAN_MSG_JCT_WARNINGS_COUNTS_BYTES (1)
#define CAN_MSG_JCT_FAULTS_COUNTS_BYTES (0)

// Periodic msg cycle times (in MS)
#define CAN_MSG_JCT_VITALS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_CYCLE_TIME_MS (100)
#define CAN_MSG_JCT_STATUS_CYCLE_TIME_MS (100)
#define CAN_MSG_JCT_WARNINGS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_FAULTS_CYCLE_TIME_MS (100)
#define CAN_MSG_JCT_WARNINGS_COUNTS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_FAULTS_COUNTS_CYCLE_TIME_MS (100)

// Signal starting values
#define CANSIG_JCT_VITALS_JCT_HEARTBEAT_START_VAL (0)
#define CANSIG_JCT_VITALS_JCT_TIMESTAMP_START_VAL (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_START_VAL (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_START_VAL (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_START_VAL (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_START_VAL (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_START_VAL (0)
#define CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_START_VAL (0)
#define CANSIG_JCT_STATUS_JCT_CURRENT_START_VAL (50) // A
#define CANSIG_JCT_STATUS_JCT_VOLTAGE_START_VAL (0) // V
#define CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_START_VAL (0) // V
#define CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_START_VAL (0)
#define CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_START_VAL (0)
#define CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL (0) // %
#define CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL (0) // %
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_START_VAL (0)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_START_VAL (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_START_VAL (0)

// Scale/offset values for encoding/decoding signals
#define CANSIG_JCT_VITALS_JCT_HEARTBEAT_SCALE (1.0f)
#define CANSIG_JCT_VITALS_JCT_HEARTBEAT_OFFSET (0)
#define CANSIG_JCT_VITALS_JCT_TIMESTAMP_SCALE (1)
#define CANSIG_JCT_VITALS_JCT_TIMESTAMP_OFFSET (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_SCALE (1)
#define CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_OFFSET (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_SCALE (1)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_OFFSET (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_SCALE (1)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_OFFSET (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_SCALE (1)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_OFFSET (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_SCALE (1)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_OFFSET (0)
#define CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_SCALE (1)
#define CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_OFFSET (0)
#define CANSIG_JCT_STATUS_JCT_CURRENT_SCALE (0.02442002442002442f)
#define CANSIG_JCT_STATUS_JCT_CURRENT_OFFSET (0)
#define CANSIG_JCT_STATUS_JCT_VOLTAGE_SCALE (0.1221001221001221f)
#define CANSIG_JCT_STATUS_JCT_VOLTAGE_OFFSET (0)
#define CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_SCALE (1)
#define CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_OFFSET (0)
#define CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_SCALE (1)
#define CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_OFFSET (0)
#define CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_SCALE (1)
#define CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_OFFSET (0)
#define CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE (2.3283064370807974e-08f)
#define CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET (0)
#define CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE (2.3283064370807974e-08f)
#define CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_SCALE (1)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_OFFSET (0)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_SCALE (1)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_OFFSET (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_SCALE (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_OFFSET (0)

// Min/max allowed values for signals
#define CANSIG_JCT_VITALS_JCT_HEARTBEAT_MIN (0)
#define CANSIG_JCT_VITALS_JCT_HEARTBEAT_MAX (1)
#define CANSIG_JCT_VITALS_JCT_TIMESTAMP_MIN (0)
#define CANSIG_JCT_VITALS_JCT_TIMESTAMP_MAX (4294967295)
#define CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_MIN (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_MAX (1)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_MIN (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_MAX (1)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_MIN (0)
#define CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_MAX (1)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_MIN (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_MAX (1)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_MIN (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_MAX (1)
#define CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MIN (0)
#define CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_MAX (1)
#define CANSIG_JCT_STATUS_JCT_CURRENT_MIN (0) // A
#define CANSIG_JCT_STATUS_JCT_CURRENT_MAX (100) // A
#define CANSIG_JCT_STATUS_JCT_VOLTAGE_MIN (0) // V
#define CANSIG_JCT_STATUS_JCT_VOLTAGE_MAX (500) // V
#define CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MIN (0) // V
#define CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_MAX (4095) // V
#define CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_MIN (0)
#define CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_MAX (1)
#define CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MIN (0)
#define CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_MAX (7)
#define CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN (0) // %
#define CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX (100) // %
#define CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN (0) // %
#define CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX (100) // %
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_MIN (0)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_MAX (1)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_MIN (0)
#define CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_MAX (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_MIN (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_MAX (1)


/* -------------------------------- Enums --------------------------------- */

typedef enum
{
    FALSE = 0,
    TRUE = 1,
    NUM_MISSING_HEARTBEAT_CHOICES = 2,
} MissingHeartbeat;

typedef enum
{
    AIR_OPEN = 0,
    AIR_CLOSED = 1,
    NUM_AIR_STATE_CHOICES = 2,
} AirState;


/* ------------------------------- Structs -------------------------------- */

/**
 * Signals in CAN msg JCT_Vitals.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_Heartbeat_value;
    /**
     * Range: 0 to 4294967295
     */
    uint32_t JCT_Timestamp_value;
} JCT_Vitals_Signals;

/**
 * Signals in CAN msg JCT_WarningsTest.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_WatchdogTimeout_value;
    /**
     * Range: 0 to 1
     */
    bool JCT_BoardOvertemp_value;
    /**
     * Range: 0 to 1
     */
    bool JCT_BoardOvervoltage_value;
} JCT_WarningsTest_Signals;

/**
 * Signals in CAN msg JCT_AirShutdownErrors.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_DummyAirShutdown_value;
} JCT_AirShutdownErrors_Signals;

/**
 * Signals in CAN msg JCT_MotorShutdownErrors.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_DummyMotorShutdown_value;
} JCT_MotorShutdownErrors_Signals;

/**
 * Signals in CAN msg JCT_Status.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    AirState JCT_ContactorsClosed_value;
    /**
     * Range: 0A to 100A
     */
    float JCT_Current_value;
    /**
     * Range: 0V to 500V
     */
    float JCT_Voltage_value;
    /**
     * Range: 0V to 4095V
     */
    uint32_t JCT_UnsignedTester_value;
} JCT_Status_Signals;

/**
 * Signals in CAN msg JCT_Warnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_Warning_Warning_Test_value;
} JCT_Warnings_Signals;

/**
 * Signals in CAN msg JCT_Faults.
 */
typedef struct
{
} JCT_Faults_Signals;

/**
 * Signals in CAN msg JCT_WarningsCounts.
 */
typedef struct
{
    /**
     * Range: 0 to 7
     */
    uint32_t JCT_Warning_Warning_TestCount_value;
} JCT_WarningsCounts_Signals;

/**
 * Signals in CAN msg JCT_FaultsCounts.
 */
typedef struct
{
} JCT_FaultsCounts_Signals;

/**
 * Signals in CAN msg FSM_Apps.
 */
typedef struct
{
    /**
     * Range: 0% to 100%
     */
    float FSM_PappsMappedPedalPercentage_value;
    /**
     * Range: 0% to 100%
     */
    float FSM_SappsMappedPedalPercentage_value;
} FSM_Apps_Signals;

/**
 * Signals in CAN msg FSM_Warnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool FSM_Warning_Warning_Test1_value;
    /**
     * Range: 0 to 1
     */
    bool FSM_Warning_Warning_Test2_value;
} FSM_Warnings_Signals;

/**
 * Signals in CAN msg FSM_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool FSM_Fault_Fault_Test3_value;
} FSM_Faults_Signals;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Pack signals into CAN payload for JCT_Vitals.
 */
void app_canUtils_JCT_Vitals_pack(const JCT_Vitals_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_WarningsTest.
 */
void app_canUtils_JCT_WarningsTest_pack(const JCT_WarningsTest_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_AirShutdownErrors.
 */
void app_canUtils_JCT_AirShutdownErrors_pack(const JCT_AirShutdownErrors_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_MotorShutdownErrors.
 */
void app_canUtils_JCT_MotorShutdownErrors_pack(const JCT_MotorShutdownErrors_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_Status.
 */
void app_canUtils_JCT_Status_pack(const JCT_Status_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_Warnings.
 */
void app_canUtils_JCT_Warnings_pack(const JCT_Warnings_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_Faults.
 */
void app_canUtils_JCT_Faults_pack(const JCT_Faults_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_WarningsCounts.
 */
void app_canUtils_JCT_WarningsCounts_pack(const JCT_WarningsCounts_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_FaultsCounts.
 */
void app_canUtils_JCT_FaultsCounts_pack(const JCT_FaultsCounts_Signals* const in_msg, uint8_t* const out_data);

/**
 * Unpack signals from CAN payload for FSM_Apps.
 */
void app_canUtils_FSM_Apps_unpack(const uint8_t* const in_data, FSM_Apps_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Warnings.
 */
void app_canUtils_FSM_Warnings_unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Faults.
 */
void app_canUtils_FSM_Faults_unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg);

