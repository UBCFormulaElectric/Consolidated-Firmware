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

// Msg lengths (in bytes)

// Periodic msg cycle times (in MS)

// Signal starting values

// Scale/offset values for encoding/decoding signals

// Min/max allowed values for signals


/* -------------------------------- Enums --------------------------------- */

typedef enum
{
    SWITCH_OFF = 0,
    SWITCH_ON = 1,
    NUM_SWITCH_STATE_CHOICES = 2,
} SwitchState;

typedef enum
{
    RTOS_TASK_1HZ = 0,
    RTOS_TASK_100HZ = 1,
    RTOS_TASK_1KHZ = 2,
    RTOS_TASK_CAN_TX = 3,
    RTOS_TASK_CAN_RX = 4,
    NUM_RTOS_TASK_NAME_CHOICES = 5,
} RtosTaskName;

typedef enum
{
    INVERTER_VSM_START_STATE = 0,
    INVERTER_VSM_PRECHARGE_INIT_STATE = 1,
    INVERTER_VSM_PRECHARGE_ACTIVE_STATE = 2,
    INVERTER_VSM_PRECHARGE_COMPLETE_STATE = 3,
    INVERTER_VSM_WAIT_STATE = 4,
    INVERTER_VSM_READY_STATE = 5,
    INVERTER_VSM_MOTOR_RUNNING_STATE = 6,
    INVERTER_VSM_BLINK_FAULT_CODE_STATE = 7,
    INVERTER_VSM_SHUTDOWN_IN_PROCESS = 14,
    INVERTER_VSM_RECYCLE_POWER_STATE = 15,
    NUM_INVERTER_VSM_STATE_CHOICES = 10,
} InverterVsmState;

typedef enum
{
    INVERTER_POWER_ON_STATE = 0,
    INVERTER_STOP_STATE = 1,
    INVERTER_OPEN_LOOP_STATE = 2,
    INVERTER_CLOSED_LOOP_STATE = 3,
    INVERTER_WAIT_STATE = 4,
    INVERTER_IDLE_RUN_STATE = 8,
    INVERTER_IDLE_STOP_STATE = 9,
    NUM_INVERTER_STATE_CHOICES = 7,
} InverterState;

typedef enum
{
    INVERTER_ACTIVE_DISCHARGE_DISABLED = 0,
    INVERTER_ACTIVE_DISCHARGE_ENABLED_WAITING = 1,
    INVERTER_ACTIVE_DISCHARGE_PERFORMING_SPEED_CHECK = 2,
    INVERTER_ACTIVE_DISCHARGE_OCCURING = 3,
    INVERTER_ACTIVE_DISCHARGE_COMPLETED = 4,
    NUM_INVERTER_ACTIVE_DISCHARGE_STATE_CHOICES = 5,
} InverterActiveDischargeState;

typedef enum
{
    INVERTER_CAN_MODE = 0,
    INVERTER_VSM_MODE = 1,
    NUM_INVERTER_COMMAND_MODE_CHOICES = 2,
} InverterCommandMode;

typedef enum
{
    INVERTER_LOCKOUT_UNLOCKED = 0,
    INVERTER_LOCKOUT_LOCKED = 1,
    NUM_INVERTER_LOCKOUT_CHOICES = 2,
} InverterLockout;

typedef enum
{
    INVERTER_REVERSE_DIRECTION = 0,
    INVERTER_FORWARD_DIRECTION = 1,
    NUM_INVERTER_DIRECTION_CHOICES = 2,
} InverterDirection;

typedef enum
{
    INVERTER_READ_COMMAND = 0,
    INVERTER_WRITE_COMMAND = 1,
    NUM_INVERTER_READ_WRITE_COMMAND_CHOICES = 2,
} InverterReadWriteCommand;

typedef enum
{
    RANGE_CHECK_OK = 0,
    RANGE_CHECK_UNDERFLOW = 1,
    RANGE_CHECK_OVERFLOW = 2,
    NUM_RANGE_CHECK_CHOICES = 3,
} RangeCheck;


/* ------------------------------- Structs -------------------------------- */


/* ------------------------- Function Prototypes -------------------------- */

