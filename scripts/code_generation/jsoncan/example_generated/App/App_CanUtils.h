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
#define CAN_MSG_JCT_WARNINGS_ID (602)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_ID (603)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_ID (604)
#define CAN_MSG_JCT_STATUS_ID (608)
#define CAN_MSG_JCT_WARNINGS_ID (51)
#define CAN_MSG_JCT_FAULTS_ID (52)
#define CAN_MSG_FSM_APPS_ID (151)
#define CAN_MSG_FSM_WARNINGS_ID (300)
#define CAN_MSG_FSM_WARNINGS_ID (41)
#define CAN_MSG_FSM_FAULTS_ID (42)

// Msg lengths (in bytes)
#define CAN_MSG_JCT_VITALS_BYTES (5)
#define CAN_MSG_JCT_WARNINGS_BYTES (1)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_BYTES (1)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_BYTES (1)
#define CAN_MSG_JCT_STATUS_BYTES (5)
#define CAN_MSG_JCT_WARNINGS_BYTES (1)
#define CAN_MSG_JCT_FAULTS_BYTES (0)

// Periodic msg cycle times (in MS)
#define CAN_MSG_JCT_VITALS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_AIR_SHUTDOWN_ERRORS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_MOTOR_SHUTDOWN_ERRORS_CYCLE_TIME_MS (100)
#define CAN_MSG_JCT_STATUS_CYCLE_TIME_MS (100)
#define CAN_MSG_JCT_WARNINGS_CYCLE_TIME_MS (1000)
#define CAN_MSG_JCT_FAULTS_CYCLE_TIME_MS (10)

// Signal starting values
#define CANSIG_JCT_VITALS_HEARTBEAT_START_VAL (0)
#define CANSIG_JCT_VITALS_TIMESTAMP_START_VAL (0)
#define CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_START_VAL (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_START_VAL (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_START_VAL (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_START_VAL (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_START_VAL (0)
#define CANSIG_JCT_STATUS_CONTACTORS_CLOSED_START_VAL (0)
#define CANSIG_JCT_STATUS_CURRENT_START_VAL (50) // A
#define CANSIG_JCT_STATUS_VOLTAGE_START_VAL (0) // V
#define CANSIG_JCT_STATUS_UNSIGNED_TESTER_START_VAL (-2047) // V
#define CANSIG_JCT_WARNINGS_JCT_TEST_START_VAL (0)
#define CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL (0) // %
#define CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL (0) // %
#define CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_START_VAL (0)
#define CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_START_VAL (0)
#define CANSIG_FSM_WARNINGS_BSPD_FAULT_START_VAL (0)
#define CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_START_VAL (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_START_VAL (0)
#define CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_START_VAL (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST1_START_VAL (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST2_START_VAL (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_START_VAL (0)

// Scale/offset values for encoding/decoding signals
#define CANSIG_JCT_VITALS_HEARTBEAT_SCALE (1.0f)
#define CANSIG_JCT_VITALS_HEARTBEAT_OFFSET (0)
#define CANSIG_JCT_VITALS_TIMESTAMP_SCALE (1)
#define CANSIG_JCT_VITALS_TIMESTAMP_OFFSET (0)
#define CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_SCALE (1)
#define CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_OFFSET (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_SCALE (1)
#define CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_OFFSET (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_SCALE (1)
#define CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_OFFSET (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_SCALE (1)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_OFFSET (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_SCALE (1)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_OFFSET (0)
#define CANSIG_JCT_STATUS_CONTACTORS_CLOSED_SCALE (1)
#define CANSIG_JCT_STATUS_CONTACTORS_CLOSED_OFFSET (0)
#define CANSIG_JCT_STATUS_CURRENT_SCALE (0.02442002442002442f)
#define CANSIG_JCT_STATUS_CURRENT_OFFSET (0)
#define CANSIG_JCT_STATUS_VOLTAGE_SCALE (0.1221001221001221f)
#define CANSIG_JCT_STATUS_VOLTAGE_OFFSET (0)
#define CANSIG_JCT_STATUS_UNSIGNED_TESTER_SCALE (1)
#define CANSIG_JCT_STATUS_UNSIGNED_TESTER_OFFSET (0)
#define CANSIG_JCT_WARNINGS_JCT_TEST_SCALE (1)
#define CANSIG_JCT_WARNINGS_JCT_TEST_OFFSET (0)
#define CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE (2.3283064370807974e-08f)
#define CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET (0)
#define CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE (2.3283064370807974e-08f)
#define CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_SCALE (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_SCALE (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_SCALE (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_SCALE (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_OFFSET (0)
#define CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_SCALE (1)
#define CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_OFFSET (0)
#define CANSIG_FSM_WARNINGS_BSPD_FAULT_SCALE (1)
#define CANSIG_FSM_WARNINGS_BSPD_FAULT_OFFSET (0)
#define CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE (1)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_SCALE (1)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_OFFSET (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_SCALE (1)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_OFFSET (0)
#define CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_SCALE (1)
#define CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_OFFSET (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST1_SCALE (1)
#define CANSIG_FSM_WARNINGS_FSM_TEST1_OFFSET (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST2_SCALE (1)
#define CANSIG_FSM_WARNINGS_FSM_TEST2_OFFSET (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_SCALE (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_OFFSET (0)

// Min/max allowed values for signals
#define CANSIG_JCT_VITALS_HEARTBEAT_MIN (0)
#define CANSIG_JCT_VITALS_HEARTBEAT_MAX (1)
#define CANSIG_JCT_VITALS_TIMESTAMP_MIN (0)
#define CANSIG_JCT_VITALS_TIMESTAMP_MAX (4294967295)
#define CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_MIN (0)
#define CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_MAX (1)
#define CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_MIN (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_MAX (1)
#define CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_MIN (0)
#define CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_MAX (1)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_MIN (0)
#define CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_MAX (1)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_MIN (0)
#define CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_MAX (1)
#define CANSIG_JCT_STATUS_CONTACTORS_CLOSED_MIN (0)
#define CANSIG_JCT_STATUS_CONTACTORS_CLOSED_MAX (1)
#define CANSIG_JCT_STATUS_CURRENT_MIN (0) // A
#define CANSIG_JCT_STATUS_CURRENT_MAX (100) // A
#define CANSIG_JCT_STATUS_VOLTAGE_MIN (0) // V
#define CANSIG_JCT_STATUS_VOLTAGE_MAX (500) // V
#define CANSIG_JCT_STATUS_UNSIGNED_TESTER_MIN (-2047) // V
#define CANSIG_JCT_STATUS_UNSIGNED_TESTER_MAX (2046) // V
#define CANSIG_JCT_WARNINGS_JCT_TEST_MIN (0)
#define CANSIG_JCT_WARNINGS_JCT_TEST_MAX (1)
#define CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN (0) // %
#define CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX (100) // %
#define CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN (0) // %
#define CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX (100) // %
#define CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_MAX (1)
#define CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_MAX (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_MIN (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_MAX (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_MIN (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_MAX (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_MIN (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_MAX (1)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_MIN (0)
#define CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_MAX (1)
#define CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_MIN (0)
#define CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_MAX (1)
#define CANSIG_FSM_WARNINGS_BSPD_FAULT_MIN (0)
#define CANSIG_FSM_WARNINGS_BSPD_FAULT_MAX (1)
#define CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MAX (3)
#define CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MAX (3)
#define CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_MAX (3)
#define CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_MAX (3)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_MIN (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_MAX (3)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_MIN (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_MAX (1)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_MIN (0)
#define CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_MAX (1)
#define CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_MIN (0)
#define CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_MAX (1)
#define CANSIG_FSM_WARNINGS_FSM_TEST1_MIN (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST1_MAX (1)
#define CANSIG_FSM_WARNINGS_FSM_TEST2_MIN (0)
#define CANSIG_FSM_WARNINGS_FSM_TEST2_MAX (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_MIN (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_MAX (1)


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
 * Signals in CAN msg JctVitals.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool Heartbeat_value;
    /**
     * Range: 0 to 4294967295
     */
    uint32_t Timestamp_value;
} JctVitals_Signals;

/**
 * Signals in CAN msg JctWarnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool WatchdogTimeout_value;
    /**
     * Range: 0 to 1
     */
    bool BoardOvertemp_value;
    /**
     * Range: 0 to 1
     */
    bool BoardOvervoltage_value;
} JctWarnings_Signals;

/**
 * Signals in CAN msg JctAirShutdownErrors.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool DummyAirShutdown_value;
} JctAirShutdownErrors_Signals;

/**
 * Signals in CAN msg JctMotorShutdownErrors.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool DummyMotorShutdown_value;
} JctMotorShutdownErrors_Signals;

/**
 * Signals in CAN msg JctStatus.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    AirState ContactorsClosed_value;
    /**
     * Range: 0A to 100A
     */
    float Current_value;
    /**
     * Range: 0V to 500V
     */
    float Voltage_value;
    /**
     * Range: -2047V to 2046V
     */
    int UnsignedTester_value;
} JctStatus_Signals;

/**
 * Signals in CAN msg JCT_Warnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool JCT_TEST_value;
} JCT_Warnings_Signals;

/**
 * Signals in CAN msg JCT_Faults.
 */
typedef struct
{
} JCT_Faults_Signals;

/**
 * Signals in CAN msg FsmApps.
 */
typedef struct
{
    /**
     * Range: 0% to 100%
     */
    float PappsMappedPedalPercentage_value;
    /**
     * Range: 0% to 100%
     */
    float SappsMappedPedalPercentage_value;
} FsmApps_Signals;

/**
 * Signals in CAN msg FsmWarnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool PappsOutOfRange_value;
    /**
     * Range: 0 to 1
     */
    bool SappsOutOfRange_value;
    /**
     * Range: 0 to 1
     */
    bool StackWatermarkAboveThresholdTask1Hz_value;
    /**
     * Range: 0 to 1
     */
    bool StackWatermarkAboveThresholdTask1kHz_value;
    /**
     * Range: 0 to 1
     */
    bool StackWatermarkAboveThresholdTaskCanRx_value;
    /**
     * Range: 0 to 1
     */
    bool StackWatermarkAboveThresholdTaskCanTx_value;
    /**
     * Range: 0 to 1
     */
    bool WatchdogFault_value;
    /**
     * Range: 0 to 1
     */
    bool BspdFault_value;
    /**
     * Range: 0 to 3
     */
    uint32_t LeftWheelSpeedOutOfRange_value;
    /**
     * Range: 0 to 3
     */
    uint32_t RightWheelSpeedOutOfRange_value;
    /**
     * Range: 0 to 3
     */
    uint32_t FlowRateOutOfRange_value;
    /**
     * Range: 0 to 3
     */
    uint32_t SteeringAngleOutOfRange_value;
    /**
     * Range: 0 to 3
     */
    uint32_t BrakePressureOutOfRange_value;
    /**
     * Range: 0 to 1
     */
    bool BrakePressureSc_value;
    /**
     * Range: 0 to 1
     */
    bool BrakePressureOc_value;
    /**
     * Range: 0 to 1
     */
    bool SteeringWheelBroke_value;
} FsmWarnings_Signals;

/**
 * Signals in CAN msg FSM_Warnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool FSM_TEST1_value;
    /**
     * Range: 0 to 1
     */
    bool FSM_TEST2_value;
} FSM_Warnings_Signals;

/**
 * Signals in CAN msg FSM_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool FSM_FAULT_TEST3_value;
} FSM_Faults_Signals;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Pack signals into CAN payload for JctVitals.
 */
void App_CanUtils_JctVitals_Pack(const JctVitals_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JctWarnings.
 */
void App_CanUtils_JctWarnings_Pack(const JctWarnings_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JctAirShutdownErrors.
 */
void App_CanUtils_JctAirShutdownErrors_Pack(const JctAirShutdownErrors_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JctMotorShutdownErrors.
 */
void App_CanUtils_JctMotorShutdownErrors_Pack(const JctMotorShutdownErrors_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JctStatus.
 */
void App_CanUtils_JctStatus_Pack(const JctStatus_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_Warnings.
 */
void App_CanUtils_JCT_Warnings_Pack(const JCT_Warnings_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for JCT_Faults.
 */
void App_CanUtils_JCT_Faults_Pack(const JCT_Faults_Signals* const in_msg, uint8_t* const out_data);

/**
 * Unpack signals from CAN payload for FsmApps.
 */
void App_CanUtils_FsmApps_Unpack(const uint8_t* const in_data, FsmApps_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FsmWarnings.
 */
void App_CanUtils_FsmWarnings_Unpack(const uint8_t* const in_data, FsmWarnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Warnings.
 */
void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Faults.
 */
void App_CanUtils_FSM_Faults_Unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg);

