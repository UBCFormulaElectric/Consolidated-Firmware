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
#define CAN_MSG_BOOT_BMS_ID (499)
#define CAN_MSG_BOOT_FSM_ID (599)
#define CAN_MSG_BOOT_VC_ID (699)
#define CAN_MSG_BOOT_RSM_ID (799)
#define CAN_MSG_BOOT_CRIT_ID (899)
#define CAN_MSG_BOOT_H7_DEV_ID (1199)
#define CAN_MSG_BMS_VITALS_ID (400)
#define CAN_MSG_BMS_LATCHED_FAULTS_ID (410)
#define CAN_MSG_BMS_WARNINGS_ID (401)
#define CAN_MSG_BMS_FAULTS_ID (402)
#define CAN_MSG_BMS_WARNINGS_COUNTS_ID (403)
#define CAN_MSG_BMS_FAULTS_COUNTS_ID (404)
#define CAN_MSG_FSM_VITALS_ID (500)
#define CAN_MSG_FSM_WARNINGS_ID (501)
#define CAN_MSG_FSM_FAULTS_ID (502)
#define CAN_MSG_FSM_WARNINGS_COUNTS_ID (503)
#define CAN_MSG_FSM_FAULTS_COUNTS_ID (504)
#define CAN_MSG_CRIT_VITALS_ID (800)
#define CAN_MSG_CRIT_SWITCHES_ID (810)
#define CAN_MSG_CRIT_DRIVE_MODE_ID (811)
#define CAN_MSG_CRIT_ALERTS_CONTEXT_ID (812)
#define CAN_MSG_CRIT_COMMIT_INFO_ID (813)
#define CAN_MSG_CRIT_CRIT_SHDN_NODE_STATUS_ID (814)
#define CAN_MSG_CRIT_WATERMARK_LEVEL_ID (815)
#define CAN_MSG_CRIT_WARNINGS_ID (801)
#define CAN_MSG_CRIT_FAULTS_ID (802)
#define CAN_MSG_CRIT_WARNINGS_COUNTS_ID (803)
#define CAN_MSG_CRIT_FAULTS_COUNTS_ID (804)
#define CAN_MSG_VC_VITALS_ID (600)
#define CAN_MSG_VC_VEHICLE_DYNAMICS_ID (610)
#define CAN_MSG_VC_GLOBAL_SHDN_NODE_STATUS_ID (611)
#define CAN_MSG_VC_WARNINGS_ID (601)
#define CAN_MSG_VC_FAULTS_ID (602)
#define CAN_MSG_VC_WARNINGS_COUNTS_ID (603)
#define CAN_MSG_VC_FAULTS_COUNTS_ID (604)
#define CAN_MSG_RSM_VITALS_ID (700)
#define CAN_MSG_RSM_WARNINGS_ID (701)
#define CAN_MSG_RSM_FAULTS_ID (702)
#define CAN_MSG_RSM_WARNINGS_COUNTS_ID (703)
#define CAN_MSG_RSM_FAULTS_COUNTS_ID (704)
#define CAN_MSG_DEBUG_CAN_MODE_ID (900)
#define CAN_MSG_DEBUG_CHARGING_ID (901)
#define CAN_MSG_DEBUG_CELL_BALANCING_ID (902)
#define CAN_MSG_DEBUG_RESET_SOC_ID (903)
#define CAN_MSG_DEBUG_SET_COOLANT_PUMP_ID (904)
#define CAN_MSG_DEBUG_SET_BSPD_TEST_CURRENT_ID (905)

// Msg lengths (in bytes)
#define CAN_MSG_RSM_VITALS_BYTES (1)
#define CAN_MSG_RSM_WARNINGS_BYTES (0)
#define CAN_MSG_RSM_FAULTS_BYTES (1)
#define CAN_MSG_RSM_WARNINGS_COUNTS_BYTES (0)
#define CAN_MSG_RSM_FAULTS_COUNTS_BYTES (1)

// Periodic msg cycle times (in MS)
#define CAN_MSG_RSM_VITALS_CYCLE_TIME_MS (100)
#define CAN_MSG_RSM_WARNINGS_CYCLE_TIME_MS (1000)
#define CAN_MSG_RSM_FAULTS_CYCLE_TIME_MS (100)
#define CAN_MSG_RSM_WARNINGS_COUNTS_CYCLE_TIME_MS (1000)
#define CAN_MSG_RSM_FAULTS_COUNTS_CYCLE_TIME_MS (100)

// Signal starting values
#define CANSIG_RSM_VITALS_RSM_HEARTBEAT_START_VAL (0)
#define CANSIG_RSM_VITALS_RSM_STATE_START_VAL (0)
#define CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_START_VAL (0)
#define CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_START_VAL (0)
#define CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_START_VAL (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_START_VAL (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_START_VAL (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_START_VAL (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_START_VAL (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_START_VAL (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_START_VAL (0)
#define CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_START_VAL (0)

// Scale/offset values for encoding/decoding signals
#define CANSIG_RSM_VITALS_RSM_HEARTBEAT_SCALE (1)
#define CANSIG_RSM_VITALS_RSM_HEARTBEAT_OFFSET (0)
#define CANSIG_RSM_VITALS_RSM_STATE_SCALE (1)
#define CANSIG_RSM_VITALS_RSM_STATE_OFFSET (0)
#define CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_SCALE (1)
#define CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_OFFSET (0)
#define CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_SCALE (1)
#define CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_OFFSET (0)
#define CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_SCALE (1)
#define CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_OFFSET (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_SCALE (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_OFFSET (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_SCALE (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_OFFSET (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_SCALE (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_OFFSET (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_SCALE (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_OFFSET (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_SCALE (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_OFFSET (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_SCALE (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_OFFSET (0)
#define CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_SCALE (1)
#define CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_OFFSET (0)

// Min/max allowed values for signals
#define CANSIG_RSM_VITALS_RSM_HEARTBEAT_MIN (0)
#define CANSIG_RSM_VITALS_RSM_HEARTBEAT_MAX (1)
#define CANSIG_RSM_VITALS_RSM_STATE_MIN (0)
#define CANSIG_RSM_VITALS_RSM_STATE_MAX (1)
#define CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_MIN (0)
#define CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_MAX (1)
#define CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_MIN (0)
#define CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_MAX (7)
#define CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_MIN (0)
#define CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_MAX (1)
#define CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_MIN (0)
#define CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_MAX (1)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_MIN (0)
#define CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_MAX (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_MIN (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_MAX (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_MIN (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_MAX (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_MIN (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_MAX (1)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_MIN (0)
#define CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_MAX (1)
#define CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_MIN (0)
#define CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_MAX (1)


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
    RSM_INIT_STATE = 0,
    RSM_DRIVE_STATE = 1,
    NUM_RSM_STATES_CHOICES = 2,
} RsmStates;


/* ------------------------------- Structs -------------------------------- */

/**
 * Signals in CAN msg RSM_Vitals.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool RSM_Heartbeat_value;
    /**
     * Range: 0 to 1
     */
    RsmStates RSM_State_value;
} RSM_Vitals_Signals;

/**
 * Signals in CAN msg RSM_Warnings.
 */
typedef struct
{
    uint8_t _unused;
} RSM_Warnings_Signals;

/**
 * Signals in CAN msg RSM_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool RSM_Fault_DummyFault_value;
} RSM_Faults_Signals;

/**
 * Signals in CAN msg RSM_WarningsCounts.
 */
typedef struct
{
    uint8_t _unused;
} RSM_WarningsCounts_Signals;

/**
 * Signals in CAN msg RSM_FaultsCounts.
 */
typedef struct
{
    /**
     * Range: 0 to 7
     */
    uint32_t RSM_Fault_DummyFaultCount_value;
} RSM_FaultsCounts_Signals;

/**
 * Signals in CAN msg BMS_Warnings.
 */
typedef struct
{
    uint8_t _unused;
} BMS_Warnings_Signals;

/**
 * Signals in CAN msg BMS_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool BMS_Fault_DummyFault_value;
} BMS_Faults_Signals;

/**
 * Signals in CAN msg FSM_Warnings.
 */
typedef struct
{
    uint8_t _unused;
} FSM_Warnings_Signals;

/**
 * Signals in CAN msg FSM_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool FSM_Fault_DummyFault_value;
} FSM_Faults_Signals;

/**
 * Signals in CAN msg CRIT_Warnings.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_StackWaterMarkHighTask1Hz_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_StackWaterMarkHighTask100Hz_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_StackWaterMarkHighTask1kHz_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_StackWaterMarkHighTaskCanRx_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_StackWaterMarkHighTaskCanTx_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_WatchdogTimeout_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_TxOverflow_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Warning_RxOverflow_value;
} CRIT_Warnings_Signals;

/**
 * Signals in CAN msg CRIT_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool CRIT_Fault_MissingBMSHeartbeat_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Fault_MissingFSMHeartbeat_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Fault_MissingVCHeartbeat_value;
    /**
     * Range: 0 to 1
     */
    bool CRIT_Fault_MissingRSMHeartbeat_value;
} CRIT_Faults_Signals;

/**
 * Signals in CAN msg VC_Warnings.
 */
typedef struct
{
    uint8_t _unused;
} VC_Warnings_Signals;

/**
 * Signals in CAN msg VC_Faults.
 */
typedef struct
{
    /**
     * Range: 0 to 1
     */
    bool VC_Fault_DummyFault_value;
} VC_Faults_Signals;


/* ------------------------- Function Prototypes -------------------------- */

/**
 * Pack signals into CAN payload for RSM_Vitals.
 */
void app_canUtils_RSM_Vitals_pack(const RSM_Vitals_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for RSM_Warnings.
 */
void app_canUtils_RSM_Warnings_pack(const RSM_Warnings_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for RSM_Faults.
 */
void app_canUtils_RSM_Faults_pack(const RSM_Faults_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for RSM_WarningsCounts.
 */
void app_canUtils_RSM_WarningsCounts_pack(const RSM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data);

/**
 * Pack signals into CAN payload for RSM_FaultsCounts.
 */
void app_canUtils_RSM_FaultsCounts_pack(const RSM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data);

/**
 * Unpack signals from CAN payload for BMS_Warnings.
 */
void app_canUtils_BMS_Warnings_unpack(const uint8_t* const in_data, BMS_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for BMS_Faults.
 */
void app_canUtils_BMS_Faults_unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Warnings.
 */
void app_canUtils_FSM_Warnings_unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for FSM_Faults.
 */
void app_canUtils_FSM_Faults_unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for CRIT_Warnings.
 */
void app_canUtils_CRIT_Warnings_unpack(const uint8_t* const in_data, CRIT_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for CRIT_Faults.
 */
void app_canUtils_CRIT_Faults_unpack(const uint8_t* const in_data, CRIT_Faults_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for VC_Warnings.
 */
void app_canUtils_VC_Warnings_unpack(const uint8_t* const in_data, VC_Warnings_Signals* const out_msg);

/**
 * Unpack signals from CAN payload for VC_Faults.
 */
void app_canUtils_VC_Faults_unpack(const uint8_t* const in_data, VC_Faults_Signals* const out_msg);

