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
// #define CAN_MSG_BMS_VITALS_ID (100)
#define CANMSG_BMS_VITALS_ID (100)
#define CANMSG_BMS_ALERTS_CONTEXT_ID (104)
#define CANMSG_BMS_IMD_STATUS_ID (105)
#define CANMSG_BMS_IMD_DATA_ID (106)
#define CANMSG_BMS_CHARGER_ID (107)
#define CANMSG_BMS_BRUSA_CONTROLS_ID (1560)
#define CANMSG_BMS_OK_STATUSES_ID (111)
#define CANMSG_BMS_CONTACTORS_ID (112)
#define CANMSG_BMS_CELL_TEMPERATURES_ID (120)
#define CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_ID (121)
#define CANMSG_BMS_CELL_STATS_ID (123)
#define CANMSG_BMS_TRACTIVE_SYSTEM_ID (130)
#define CANMSG_BMS_COMMIT_INFO_ID (140)
#define CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_ID (141)
#define CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_ID (142)
#define CANMSG_BMS_BMS_SHDN_NODE_STATUS_ID (143)
#define CANMSG_BMS_WARNINGS_ID (150)
#define CANMSG_BMS_FAULTS_ID (151)
#define CANMSG_BMS_WARNINGS_COUNTS_ID (152)
#define CANMSG_BMS_FAULTS_COUNTS_ID (153)
#define CANMSG_FSM_VITALS_ID (300)
#define CANMSG_FSM_ALERTS_CONTEXT_ID (301)
#define CANMSG_FSM_COMMIT_INFO_ID (302)
#define CANMSG_FSM_STEERING_ID (303)
#define CANMSG_FSM_WHEELS_ID (304)
#define CANMSG_FSM_APPS_ID (305)
#define CANMSG_FSM_BRAKE_ID (306)
#define CANMSG_FSM_FSM_SHDN_NODE_STATUS_ID (309)
#define CANMSG_FSM_WARNINGS_ID (396)
#define CANMSG_FSM_FAULTS_ID (397)
#define CANMSG_FSM_WARNINGS_COUNTS_ID (398)
#define CANMSG_FSM_FAULTS_COUNTS_ID (399)
#define CANMSG_VC_VITALS_ID (200)
#define CANMSG_VC_LEFT_INVERTER_COMMAND_ID (32)
#define CANMSG_VC_INVL_READ_WRITE_PARAM_COMMAND_ID (33)
#define CANMSG_VC_RIGHT_INVERTER_COMMAND_ID (82)
#define CANMSG_VC_INVR_READ_WRITE_PARAM_COMMAND_ID (83)
#define CANMSG_VC_ALERTS_CONTEXT_ID (202)
#define CANMSG_VC_ELLIPSE_STATUS_ID (223)
#define CANMSG_VC_ELLIPSE_TIME_ID (209)
#define CANMSG_VC_ELLIPSE_EULER_ANGLES_ID (222)
#define CANMSG_VC_COMMIT_INFO_ID (204)
#define CANMSG_VC_POWER_ESTIMATE_ID (211)
#define CANMSG_VC_POWER_ESTIMATE_PID_ID (212)
#define CANMSG_VC_PID_SLIP_RATIO_ID (213)
#define CANMSG_VC_SLIP_RATIOS_ID (214)
#define CANMSG_VC_ACTIVE_DIFF_OUTPUTS_ID (215)
#define CANMSG_VC_POWER_LIMIT_ID (216)
#define CANMSG_VC_VEHICLE_DYNAMICS_ID (226)
#define CANMSG_VC_ELLIPSE_EKF_NAV_VELOCITY_ID (218)
#define CANMSG_VC_VOLTAGES_ID (219)
#define CANMSG_VC_CURRENT_SENSING_ID (220)
#define CANMSG_VC_EFUSE_ID (221)
#define CANMSG_VC_IMU_LINEAR_ACCELERATION_ID (225)
#define CANMSG_VC_COOLANT_PUMP_FLOW_RATE_ID (224)
#define CANMSG_VC_VC_SHDN_NODE_STATUS_ID (228)
#define CANMSG_VC_GLOBAL_SHDN_NODE_STATUS_ID (229)
#define CANMSG_VC_PEDAL_PERCENTAGE_ID (230)
#define CANMSG_VC_WARNINGS_ID (205)
#define CANMSG_VC_FAULTS_ID (206)
#define CANMSG_VC_WARNINGS_COUNTS_ID (207)
#define CANMSG_VC_FAULTS_COUNTS_ID (208)


// Msg lengths (in bytes)
#define CANMSG_BMS_VITALS_BYTES (1)
#define CANMSG_BMS_ALERTS_CONTEXT_BYTES (7)
#define CANMSG_BMS_IMD_STATUS_BYTES (5)
#define CANMSG_BMS_IMD_DATA_BYTES (8)
#define CANMSG_BMS_CHARGER_BYTES (1)
#define CANMSG_BMS_BRUSA_CONTROLS_BYTES (7)
#define CANMSG_BMS_OK_STATUSES_BYTES (1)
#define CANMSG_BMS_CONTACTORS_BYTES (1)
#define CANMSG_BMS_CELL_TEMPERATURES_BYTES (3)
#define CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_BYTES (7)
#define CANMSG_BMS_CELL_STATS_BYTES (4)
#define CANMSG_BMS_TRACTIVE_SYSTEM_BYTES (7)
#define CANMSG_BMS_COMMIT_INFO_BYTES (5)
#define CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_BYTES (6)
#define CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_BYTES (4)
#define CANMSG_BMS_BMS_SHDN_NODE_STATUS_BYTES (1)
#define CANMSG_VC_VITALS_BYTES (3)


// Periodic msg cycle times (in MS)
#define CANMSG_BMS_VITALS_CYCLE_TIME (100)
#define CANMSG_BMS_ALERTS_CONTEXT_CYCLE_TIME (1000)
#define CANMSG_BMS_IMD_STATUS_CYCLE_TIME (100)
#define CANMSG_BMS_IMD_DATA_CYCLE_TIME (1000)
#define CANMSG_BMS_CHARGER_CYCLE_TIME (100)
#define CANMSG_BMS_BRUSA_CONTROLS_CYCLE_TIME (100)
#define CANMSG_BMS_OK_STATUSES_CYCLE_TIME (100)
#define CANMSG_BMS_CONTACTORS_CYCLE_TIME (100)
#define CANMSG_BMS_CELL_TEMPERATURES_CYCLE_TIME (1000)
#define CANMSG_BMS_VOLTAGE_AND_CHARGE_STATS_CYCLE_TIME (100)
#define CANMSG_BMS_CELL_STATS_CYCLE_TIME (100)
#define CANMSG_BMS_TRACTIVE_SYSTEM_CYCLE_TIME (100)
#define CANMSG_BMS_COMMIT_INFO_CYCLE_TIME (100)
#define CANMSG_BMS_OWC_SEGMENT0TO2_STATUS_CYCLE_TIME (100)
#define CANMSG_BMS_OWC_SEGMENT3TO4_STATUS_CYCLE_TIME (100)
#define CANMSG_BMS_BMS_SHDN_NODE_STATUS_CYCLE_TIME (100)
#define CANMSG_VC_VITALS_CYCLE_TIME (100)


// Signal starting values
// BMS_Vitals
#define CANSIG_BMS_HEARTBEAT_START_VAL (0)
#define CANSIG_BMS_STATE_START_VAL (0)

// BMS_AlertsContext
#define CANSIG_BMS_TX_OVERFLOW_COUNT_START_VAL (0)
#define CANSIG_BMS_RX_OVERFLOW_COUNT_START_VAL (0)
#define CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_START_VAL (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_START_VAL (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_START_VAL (0)

// BMS_ImdStatus
#define CANSIG_BMS_IMD_OK_HS_START_VAL (0)
#define CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_START_VAL (0)// ms
#define CANSIG_BMS_IMD_VALID_DUTY_CYCLE_START_VAL (0)
#define CANSIG_BMS_IMD_CONDITION_START_VAL (0)
#define CANSIG_BMS_IMD_DUTY_CYCLE_START_VAL (0)// %
#define CANSIG_BMS_IMD_FREQUENCY_START_VAL (0)// Hz

// BMS_ImdData
#define CANSIG_BMS_IMD_ACTIVE_FREQUENCY_START_VAL (0)
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_START_VAL (0)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_START_VAL (0)// kOhm
#define CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_START_VAL (0)

// BMS_Charger
#define CANSIG_BMS_CHARGER_CONNECTED_START_VAL (0)

// BMS_BrusaControls
#define CANSIG_BMS_CHARGER_ENABLE_START_VAL (0)
#define CANSIG_BMS_CLEAR_LATCH_START_VAL (0)
#define CANSIG_BMS_MAX_MAINS_CURRENT_START_VAL (0)
#define CANSIG_BMS_CHARGING_VOLTAGE_START_VAL (0)
#define CANSIG_BMS_CHARGING_CURRENT_START_VAL (0)

// BMS_OkStatuses
#define CANSIG_BMS_BMS_OK_START_VAL (0)
#define CANSIG_BMS_IMD_OK_START_VAL (0)
#define CANSIG_BMS_BSPD_OK_START_VAL (0)
#define CANSIG_BMS_BMS_LATCHED_FAULT_START_VAL (0)
#define CANSIG_BMS_IMD_LATCHED_FAULT_START_VAL (0)
#define CANSIG_BMS_BSPD_LATCHED_FAULT_START_VAL (0)

// BMS_Contactors
#define CANSIG_BMS_AIR_POSITIVE_START_VAL (0)
#define CANSIG_BMS_AIR_NEGATIVE_START_VAL (0)
#define CANSIG_BMS_PRECHARGE_RELAY_START_VAL (0)

// BMS_CellTemperatures
#define CANSIG_BMS_MIN_CELL_TEMPERATURE_START_VAL (0)// degC
#define CANSIG_BMS_MAX_CELL_TEMPERATURE_START_VAL (0)// degC

// BMS_VoltageAndChargeStats
#define CANSIG_BMS_PACK_VOLTAGE_START_VAL (0)// V
#define CANSIG_BMS_SOC_START_VAL (0)// %
#define CANSIG_BMS_SOC_CORRUPT_START_VAL (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_START_VAL (0)// V
#define CANSIG_BMS_MAX_CELL_VOLTAGE_START_VAL (0)// V

// BMS_CellStats
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_START_VAL (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_START_VAL (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_START_VAL (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_START_VAL (0)
#define CANSIG_BMS_MIN_TEMP_SEGMENT_START_VAL (0)
#define CANSIG_BMS_MIN_TEMP_IDX_START_VAL (0)
#define CANSIG_BMS_MAX_TEMP_SEGMENT_START_VAL (0)
#define CANSIG_BMS_MAX_TEMP_IDX_START_VAL (0)

// BMS_TractiveSystem
#define CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_START_VAL (0)// V
#define CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_START_VAL (-300)// A
#define CANSIG_BMS_TRACTIVE_SYSTEM_POWER_START_VAL (-100)// kW
#define CANSIG_BMS_AVAILABLE_POWER_START_VAL (0)// W
#define CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_START_VAL (0)

// BMS_CommitInfo
#define CANSIG_BMS_HASH_START_VAL (0)
#define CANSIG_BMS_CLEAN_START_VAL (0)

// BMS_OWC_Segment0to2_Status
#define CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_START_VAL (0)
#define CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_START_VAL (0)
#define CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_START_VAL (0)

// BMS_OWC_Segment3to4_Status
#define CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_START_VAL (0)
#define CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_START_VAL (0)

// BMS_BMSShdnNodeStatus
#define CANSIG_BMS_HVD_SHDN_OK_STATUS_START_VAL (0)
#define CANSIG_BMS_TS_ILCK_OK_STATUS_START_VAL (0)

// VC_Vitals
#define CANSIG_VC_HEARTBEAT_START_VAL (0)
#define CANSIG_VC_STATE_START_VAL (0)
#define CANSIG_VC_BUZZER_ON_START_VAL (0)
#define CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_START_VAL (0)
#define CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_START_VAL (0)


// Scale/offset values for encoding/decoding signals
// BMS_Vitals
#define CANSIG_BMS_HEARTBEAT_SCALE (1)
#define CANSIG_BMS_HEARTBEAT_OFFSET (0)
#define CANSIG_BMS_STATE_SCALE (1)
#define CANSIG_BMS_STATE_OFFSET (0)

// BMS_AlertsContext
#define CANSIG_BMS_TX_OVERFLOW_COUNT_SCALE (1)
#define CANSIG_BMS_TX_OVERFLOW_COUNT_OFFSET (0)
#define CANSIG_BMS_RX_OVERFLOW_COUNT_SCALE (1)
#define CANSIG_BMS_RX_OVERFLOW_COUNT_OFFSET (0)
#define CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_SCALE (1)
#define CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_SCALE (1)
#define CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_OFFSET (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_SCALE (1)
#define CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_OFFSET (0)

// BMS_ImdStatus
#define CANSIG_BMS_IMD_OK_HS_SCALE (1)
#define CANSIG_BMS_IMD_OK_HS_OFFSET (0)
#define CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_SCALE (1)// ms
#define CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_OFFSET (0)// ms
#define CANSIG_BMS_IMD_VALID_DUTY_CYCLE_SCALE (1)
#define CANSIG_BMS_IMD_VALID_DUTY_CYCLE_OFFSET (0)
#define CANSIG_BMS_IMD_CONDITION_SCALE (1)
#define CANSIG_BMS_IMD_CONDITION_OFFSET (0)
#define CANSIG_BMS_IMD_DUTY_CYCLE_SCALE (0.39215686274509803)// %
#define CANSIG_BMS_IMD_DUTY_CYCLE_OFFSET (0)// %
#define CANSIG_BMS_IMD_FREQUENCY_SCALE (0.39215686274509803)// Hz
#define CANSIG_BMS_IMD_FREQUENCY_OFFSET (0)// Hz

// BMS_ImdData
#define CANSIG_BMS_IMD_ACTIVE_FREQUENCY_SCALE (1)
#define CANSIG_BMS_IMD_ACTIVE_FREQUENCY_OFFSET (0)
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_SCALE (0.1)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_OFFSET (0)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_SCALE (0.1)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_OFFSET (0)// kOhm
#define CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_SCALE (0.1)
#define CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_OFFSET (0)

// BMS_Charger
#define CANSIG_BMS_CHARGER_CONNECTED_SCALE (1)
#define CANSIG_BMS_CHARGER_CONNECTED_OFFSET (0)

// BMS_BrusaControls
#define CANSIG_BMS_CHARGER_ENABLE_SCALE (1)
#define CANSIG_BMS_CHARGER_ENABLE_OFFSET (0)
#define CANSIG_BMS_CLEAR_LATCH_SCALE (1)
#define CANSIG_BMS_CLEAR_LATCH_OFFSET (0)
#define CANSIG_BMS_MAX_MAINS_CURRENT_SCALE (0.1)
#define CANSIG_BMS_MAX_MAINS_CURRENT_OFFSET (0)
#define CANSIG_BMS_CHARGING_VOLTAGE_SCALE (0.1)
#define CANSIG_BMS_CHARGING_VOLTAGE_OFFSET (0)
#define CANSIG_BMS_CHARGING_CURRENT_SCALE (0.1)
#define CANSIG_BMS_CHARGING_CURRENT_OFFSET (0)

// BMS_OkStatuses
#define CANSIG_BMS_BMS_OK_SCALE (1)
#define CANSIG_BMS_BMS_OK_OFFSET (0)
#define CANSIG_BMS_IMD_OK_SCALE (1)
#define CANSIG_BMS_IMD_OK_OFFSET (0)
#define CANSIG_BMS_BSPD_OK_SCALE (1)
#define CANSIG_BMS_BSPD_OK_OFFSET (0)
#define CANSIG_BMS_BMS_LATCHED_FAULT_SCALE (1)
#define CANSIG_BMS_BMS_LATCHED_FAULT_OFFSET (0)
#define CANSIG_BMS_IMD_LATCHED_FAULT_SCALE (1)
#define CANSIG_BMS_IMD_LATCHED_FAULT_OFFSET (0)
#define CANSIG_BMS_BSPD_LATCHED_FAULT_SCALE (1)
#define CANSIG_BMS_BSPD_LATCHED_FAULT_OFFSET (0)

// BMS_Contactors
#define CANSIG_BMS_AIR_POSITIVE_SCALE (1)
#define CANSIG_BMS_AIR_POSITIVE_OFFSET (0)
#define CANSIG_BMS_AIR_NEGATIVE_SCALE (1)
#define CANSIG_BMS_AIR_NEGATIVE_OFFSET (0)
#define CANSIG_BMS_PRECHARGE_RELAY_SCALE (1)
#define CANSIG_BMS_PRECHARGE_RELAY_OFFSET (0)

// BMS_CellTemperatures
#define CANSIG_BMS_MIN_CELL_TEMPERATURE_SCALE (0.1)// degC
#define CANSIG_BMS_MIN_CELL_TEMPERATURE_OFFSET (0)// degC
#define CANSIG_BMS_MAX_CELL_TEMPERATURE_SCALE (0.1)// degC
#define CANSIG_BMS_MAX_CELL_TEMPERATURE_OFFSET (0)// degC

// BMS_VoltageAndChargeStats
#define CANSIG_BMS_PACK_VOLTAGE_SCALE (0.01)// V
#define CANSIG_BMS_PACK_VOLTAGE_OFFSET (0)// V
#define CANSIG_BMS_SOC_SCALE (0.1)// %
#define CANSIG_BMS_SOC_OFFSET (0)// %
#define CANSIG_BMS_SOC_CORRUPT_SCALE (1)
#define CANSIG_BMS_SOC_CORRUPT_OFFSET (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SCALE (0.001)// V
#define CANSIG_BMS_MIN_CELL_VOLTAGE_OFFSET (0)// V
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SCALE (0.001)// V
#define CANSIG_BMS_MAX_CELL_VOLTAGE_OFFSET (0)// V

// BMS_CellStats
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_SCALE (1)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_OFFSET (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_SCALE (1)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_OFFSET (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_SCALE (1.0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_OFFSET (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_SCALE (1)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_OFFSET (0)
#define CANSIG_BMS_MIN_TEMP_SEGMENT_SCALE (1)
#define CANSIG_BMS_MIN_TEMP_SEGMENT_OFFSET (0)
#define CANSIG_BMS_MIN_TEMP_IDX_SCALE (1)
#define CANSIG_BMS_MIN_TEMP_IDX_OFFSET (0)
#define CANSIG_BMS_MAX_TEMP_SEGMENT_SCALE (1)
#define CANSIG_BMS_MAX_TEMP_SEGMENT_OFFSET (0)
#define CANSIG_BMS_MAX_TEMP_IDX_SCALE (1)
#define CANSIG_BMS_MAX_TEMP_IDX_OFFSET (0)

// BMS_TractiveSystem
#define CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE (0.1)// V
#define CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET (0)// V
#define CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE (0.1)// A
#define CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET (-300)// A
#define CANSIG_BMS_TRACTIVE_SYSTEM_POWER_SCALE (0.1)// kW
#define CANSIG_BMS_TRACTIVE_SYSTEM_POWER_OFFSET (-100)// kW
#define CANSIG_BMS_AVAILABLE_POWER_SCALE (1)// W
#define CANSIG_BMS_AVAILABLE_POWER_OFFSET (0)// W
#define CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_SCALE (1)
#define CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_OFFSET (0)

// BMS_CommitInfo
#define CANSIG_BMS_HASH_SCALE (1)
#define CANSIG_BMS_HASH_OFFSET (0)
#define CANSIG_BMS_CLEAN_SCALE (1)
#define CANSIG_BMS_CLEAN_OFFSET (0)

// BMS_OWC_Segment0to2_Status
#define CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_SCALE (1)
#define CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_OFFSET (0)
#define CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_SCALE (1)
#define CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_OFFSET (0)
#define CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_SCALE (1)
#define CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_OFFSET (0)

// BMS_OWC_Segment3to4_Status
#define CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_SCALE (1)
#define CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_OFFSET (0)
#define CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_SCALE (1)
#define CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_OFFSET (0)

// BMS_BMSShdnNodeStatus
#define CANSIG_BMS_HVD_SHDN_OK_STATUS_SCALE (1)
#define CANSIG_BMS_HVD_SHDN_OK_STATUS_OFFSET (0)
#define CANSIG_BMS_TS_ILCK_OK_STATUS_SCALE (1)
#define CANSIG_BMS_TS_ILCK_OK_STATUS_OFFSET (0)

// VC_Vitals
#define CANSIG_VC_HEARTBEAT_SCALE (1)
#define CANSIG_VC_HEARTBEAT_OFFSET (0)
#define CANSIG_VC_STATE_SCALE (1)
#define CANSIG_VC_STATE_OFFSET (0)
#define CANSIG_VC_BUZZER_ON_SCALE (1)
#define CANSIG_VC_BUZZER_ON_OFFSET (0)
#define CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_SCALE (1)
#define CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_OFFSET (0)
#define CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_SCALE (1)
#define CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_OFFSET (0)




// Min/max allowed values for signals
// #define CANSIG_BMS_VITALS_BMS_HEARTBEAT_MIN (0)
// BMS_Vitals
#define CANSIG_BMS_HEARTBEAT_MIN_VAL (0)
#define CANSIG_BMS_HEARTBEAT_MAX_VAL (1)
#define CANSIG_BMS_STATE_MIN_VAL (0)
#define CANSIG_BMS_STATE_MAX_VAL (6)

// BMS_AlertsContext
#define CANSIG_BMS_TX_OVERFLOW_COUNT_MIN_VAL (0)
#define CANSIG_BMS_TX_OVERFLOW_COUNT_MAX_VAL (65535)
#define CANSIG_BMS_RX_OVERFLOW_COUNT_MIN_VAL (0)
#define CANSIG_BMS_RX_OVERFLOW_COUNT_MAX_VAL (65535)
#define CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MIN_VAL (0)
#define CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_MAX_VAL (4)
#define CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MIN_VAL (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_MAX_VAL (65535)
#define CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MIN_VAL (0)
#define CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_MAX_VAL (1)

// BMS_ImdStatus
#define CANSIG_BMS_IMD_OK_HS_MIN_VAL (0)
#define CANSIG_BMS_IMD_OK_HS_MAX_VAL (1)
#define CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MIN_VAL (0)// ms
#define CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_MAX_VAL (10000)// ms
#define CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MIN_VAL (0)
#define CANSIG_BMS_IMD_VALID_DUTY_CYCLE_MAX_VAL (1)
#define CANSIG_BMS_IMD_CONDITION_MIN_VAL (0)
#define CANSIG_BMS_IMD_CONDITION_MAX_VAL (6)
#define CANSIG_BMS_IMD_DUTY_CYCLE_MIN_VAL (0)// %
#define CANSIG_BMS_IMD_DUTY_CYCLE_MAX_VAL (100)// %
#define CANSIG_BMS_IMD_FREQUENCY_MIN_VAL (0)// Hz
#define CANSIG_BMS_IMD_FREQUENCY_MAX_VAL (100)// Hz

// BMS_ImdData
#define CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MIN_VAL (0)
#define CANSIG_BMS_IMD_ACTIVE_FREQUENCY_MAX_VAL (5)
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MIN_VAL (0)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_MAX_VAL (65535)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MIN_VAL (0)// kOhm
#define CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_MAX_VAL (65535)// kOhm
#define CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MIN_VAL (0)
#define CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_MAX_VAL (65535)

// BMS_Charger
#define CANSIG_BMS_CHARGER_CONNECTED_MIN_VAL (0)
#define CANSIG_BMS_CHARGER_CONNECTED_MAX_VAL (1)

// BMS_BrusaControls
#define CANSIG_BMS_CHARGER_ENABLE_MIN_VAL (0)
#define CANSIG_BMS_CHARGER_ENABLE_MAX_VAL (1)
#define CANSIG_BMS_CLEAR_LATCH_MIN_VAL (0)
#define CANSIG_BMS_CLEAR_LATCH_MAX_VAL (1)
#define CANSIG_BMS_MAX_MAINS_CURRENT_MIN_VAL (0)
#define CANSIG_BMS_MAX_MAINS_CURRENT_MAX_VAL (6000)
#define CANSIG_BMS_CHARGING_VOLTAGE_MIN_VAL (0)
#define CANSIG_BMS_CHARGING_VOLTAGE_MAX_VAL (6000)
#define CANSIG_BMS_CHARGING_CURRENT_MIN_VAL (0)
#define CANSIG_BMS_CHARGING_CURRENT_MAX_VAL (6000)

// BMS_OkStatuses
#define CANSIG_BMS_BMS_OK_MIN_VAL (0)
#define CANSIG_BMS_BMS_OK_MAX_VAL (1)
#define CANSIG_BMS_IMD_OK_MIN_VAL (0)
#define CANSIG_BMS_IMD_OK_MAX_VAL (1)
#define CANSIG_BMS_BSPD_OK_MIN_VAL (0)
#define CANSIG_BMS_BSPD_OK_MAX_VAL (1)
#define CANSIG_BMS_BMS_LATCHED_FAULT_MIN_VAL (0)
#define CANSIG_BMS_BMS_LATCHED_FAULT_MAX_VAL (1)
#define CANSIG_BMS_IMD_LATCHED_FAULT_MIN_VAL (0)
#define CANSIG_BMS_IMD_LATCHED_FAULT_MAX_VAL (1)
#define CANSIG_BMS_BSPD_LATCHED_FAULT_MIN_VAL (0)
#define CANSIG_BMS_BSPD_LATCHED_FAULT_MAX_VAL (1)

// BMS_Contactors
#define CANSIG_BMS_AIR_POSITIVE_MIN_VAL (0)
#define CANSIG_BMS_AIR_POSITIVE_MAX_VAL (1)
#define CANSIG_BMS_AIR_NEGATIVE_MIN_VAL (0)
#define CANSIG_BMS_AIR_NEGATIVE_MAX_VAL (1)
#define CANSIG_BMS_PRECHARGE_RELAY_MIN_VAL (0)
#define CANSIG_BMS_PRECHARGE_RELAY_MAX_VAL (1)

// BMS_CellTemperatures
#define CANSIG_BMS_MIN_CELL_TEMPERATURE_MIN_VAL (0)// degC
#define CANSIG_BMS_MIN_CELL_TEMPERATURE_MAX_VAL (100)// degC
#define CANSIG_BMS_MAX_CELL_TEMPERATURE_MIN_VAL (0)// degC
#define CANSIG_BMS_MAX_CELL_TEMPERATURE_MAX_VAL (100)// degC

// BMS_VoltageAndChargeStats
#define CANSIG_BMS_PACK_VOLTAGE_MIN_VAL (0)// V
#define CANSIG_BMS_PACK_VOLTAGE_MAX_VAL (600)// V
#define CANSIG_BMS_SOC_MIN_VAL (0)// %
#define CANSIG_BMS_SOC_MAX_VAL (100)// %
#define CANSIG_BMS_SOC_CORRUPT_MIN_VAL (0)
#define CANSIG_BMS_SOC_CORRUPT_MAX_VAL (1)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_MIN_VAL (0)// V
#define CANSIG_BMS_MIN_CELL_VOLTAGE_MAX_VAL (5)// V
#define CANSIG_BMS_MAX_CELL_VOLTAGE_MIN_VAL (0)// V
#define CANSIG_BMS_MAX_CELL_VOLTAGE_MAX_VAL (5)// V

// BMS_CellStats
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MIN_VAL (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_MAX_VAL (7)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MIN_VAL (0)
#define CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_MAX_VAL (15)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MIN_VAL (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_MAX_VAL (7)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MIN_VAL (0)
#define CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_MAX_VAL (15)
#define CANSIG_BMS_MIN_TEMP_SEGMENT_MIN_VAL (0)
#define CANSIG_BMS_MIN_TEMP_SEGMENT_MAX_VAL (7)
#define CANSIG_BMS_MIN_TEMP_IDX_MIN_VAL (0)
#define CANSIG_BMS_MIN_TEMP_IDX_MAX_VAL (15)
#define CANSIG_BMS_MAX_TEMP_SEGMENT_MIN_VAL (0)
#define CANSIG_BMS_MAX_TEMP_SEGMENT_MAX_VAL (7)
#define CANSIG_BMS_MAX_TEMP_IDX_MIN_VAL (0)
#define CANSIG_BMS_MAX_TEMP_IDX_MAX_VAL (15)

// BMS_TractiveSystem
#define CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MIN_VAL (0)// V
#define CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_MAX_VAL (600)// V
#define CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MIN_VAL (-300)// A
#define CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_MAX_VAL (300)// A
#define CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MIN_VAL (-100)// kW
#define CANSIG_BMS_TRACTIVE_SYSTEM_POWER_MAX_VAL (100)// kW
#define CANSIG_BMS_AVAILABLE_POWER_MIN_VAL (0)// W
#define CANSIG_BMS_AVAILABLE_POWER_MAX_VAL (100000)// W
#define CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MIN_VAL (0)
#define CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_MAX_VAL (1)

// BMS_CommitInfo
#define CANSIG_BMS_HASH_MIN_VAL (0)
#define CANSIG_BMS_HASH_MAX_VAL (4294967295)
#define CANSIG_BMS_CLEAN_MIN_VAL (0)
#define CANSIG_BMS_CLEAN_MAX_VAL (1)

// BMS_OWC_Segment0to2_Status
#define CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MIN_VAL (0)
#define CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_MAX_VAL (65535)
#define CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MIN_VAL (0)
#define CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_MAX_VAL (65535)
#define CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MIN_VAL (0)
#define CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_MAX_VAL (65535)

// BMS_OWC_Segment3to4_Status
#define CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MIN_VAL (0)
#define CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_MAX_VAL (65535)
#define CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MIN_VAL (0)
#define CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_MAX_VAL (65535)

// BMS_BMSShdnNodeStatus
#define CANSIG_BMS_HVD_SHDN_OK_STATUS_MIN_VAL (0)
#define CANSIG_BMS_HVD_SHDN_OK_STATUS_MAX_VAL (1)
#define CANSIG_BMS_TS_ILCK_OK_STATUS_MIN_VAL (0)
#define CANSIG_BMS_TS_ILCK_OK_STATUS_MAX_VAL (1)

// VC_Vitals
#define CANSIG_VC_HEARTBEAT_MIN_VAL (0)
#define CANSIG_VC_HEARTBEAT_MAX_VAL (1)
#define CANSIG_VC_STATE_MIN_VAL (0)
#define CANSIG_VC_STATE_MAX_VAL (2)
#define CANSIG_VC_BUZZER_ON_MIN_VAL (0)
#define CANSIG_VC_BUZZER_ON_MAX_VAL (1)
#define CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MIN_VAL (0)
#define CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_MAX_VAL (1000)
#define CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MIN_VAL (0)
#define CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_MAX_VAL (10)




/* -------------------------------- Enums --------------------------------- */
typedef enum
{ 
    SWITCH_OFF = 0,
    SWITCH_ON = 1,
    
} SwitchState;

typedef enum
{ 
    RTOS_TASK_1HZ = 0,
    RTOS_TASK_100HZ = 1,
    RTOS_TASK_1KHZ = 2,
    RTOS_TASK_CAN_TX = 3,
    RTOS_TASK_CAN_RX = 4,
    
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
    
} InverterState;

typedef enum
{ 
    INVERTER_ACTIVE_DISCHARGE_DISABLED = 0,
    INVERTER_ACTIVE_DISCHARGE_ENABLED_WAITING = 1,
    INVERTER_ACTIVE_DISCHARGE_PERFORMING_SPEED_CHECK = 2,
    INVERTER_ACTIVE_DISCHARGE_OCCURING = 3,
    INVERTER_ACTIVE_DISCHARGE_COMPLETED = 4,
    
} InverterActiveDischargeState;

typedef enum
{ 
    INVERTER_CAN_MODE = 0,
    INVERTER_VSM_MODE = 1,
    
} InverterCommandMode;

typedef enum
{ 
    INVERTER_LOCKOUT_UNLOCKED = 0,
    INVERTER_LOCKOUT_LOCKED = 1,
    
} InverterLockout;

typedef enum
{ 
    INVERTER_REVERSE_DIRECTION = 0,
    INVERTER_FORWARD_DIRECTION = 1,
    
} InverterDirection;

typedef enum
{ 
    INVERTER_READ_COMMAND = 0,
    INVERTER_WRITE_COMMAND = 1,
    
} InverterReadWriteCommand;

typedef enum
{ 
    SHDN_BSPD_OK = 0,
    SHDN_BMS_OK = 1,
    SHDN_IMD_OK = 2,
    SHDN_TS_Ilck = 3,
    SHDN_Cockpit_EStop = 4,
    SHDN_Inertia = 5,
    SHDN_BOTS = 6,
    SHDN_L_EStop = 7,
    SHDN_R_EStop = 8,
    SHDN_TSMS = 9,
    SHDN_SB_ILCK = 10,
    SHDN_HVD_ILCK = 11,
    SHDN_OK = 12,
    
} ShutdownNode;

typedef enum
{ 
    BMS_INIT_STATE = 0,
    BMS_PRECHARGE_STATE = 1,
    BMS_CHARGE_STATE = 2,
    BMS_DRIVE_STATE = 3,
    BMS_BALANCING_STATE = 4,
    BMS_FAULT_STATE = 5,
    BMS_INVERTER_ON_STATE = 6,
    
} BmsState;

typedef enum
{ 
    CAN_GET_CELL_VOLTAGE_STATE = 0,
    CAN_GET_CELL_TEMPERATURE_STATE = 1,
    
} CAN_AccumulatorMonitorState;

typedef enum
{ 
    IMD_CONDITION_SHORT_CIRCUIT = 0,
    IMD_CONDITION_NORMAL = 1,
    IMD_CONDITION_UNDERVOLTAGE_DETECTED = 2,
    IMD_CONDITION_SST = 3,
    IMD_CONDITION_DEVICE_ERROR = 4,
    IMD_CONDITION_GROUND_FAULT = 5,
    IMD_CONDITION_INVALID = 6,
    
} ImdConditionName;

typedef enum
{ 
    IMD_0Hz = 0,
    IMD_10Hz = 1,
    IMD_20Hz = 2,
    IMD_30Hz = 3,
    IMD_40Hz = 4,
    IMD_50Hz = 5,
    
} ImdActiveFrequency;

typedef enum
{ 
    CONTACTOR_STATE_OPEN = 0,
    CONTACTOR_STATE_CLOSED = 1,
    
} ContactorState;

typedef enum
{ 
    VC_INIT_STATE = 0,
    VC_INVERTER_ON_STATE = 1,
    VC_DRIVE_STATE = 2,
    
} VcState;



/* ------------------------------- Structs -------------------------------- */


typedef struct
{
    bool BMS_Heartbeat_value;
    BmsState BMS_State_value;
    
} BMS_Vitals_Signals;

typedef struct
{
    uint32_t BMS_TxOverflowCount_value;
    uint32_t BMS_RxOverflowCount_value;
    RtosTaskName BMS_WatchdogTimeoutTaskName_value;
    uint32_t BMS_ModuleCommunication_NumCommTries_value;
    CAN_AccumulatorMonitorState BMS_ModuleCommunication_MonitorState_value;
    
} BMS_AlertsContext_Signals;

typedef struct
{
    bool BMS_ImdOkHs_value;
    uint32_t BMS_ImdTimeSincePowerOn_value;
    bool BMS_ImdValidDutyCycle_value;
    ImdConditionName BMS_ImdCondition_value;
    float BMS_ImdDutyCycle_value;
    float BMS_ImdFrequency_value;
    
} BMS_ImdStatus_Signals;

typedef struct
{
    ImdActiveFrequency BMS_ImdActiveFrequency_value;
    float BMS_ImdInsulationMeasurementDcp10Hz_value;
    float BMS_ImdInsulationMeasurementDcp20Hz_value;
    float BMS_ImdSpeedStartStatus30Hz_value;
    
} BMS_ImdData_Signals;

typedef struct
{
    bool BMS_ChargerConnected_value;
    
} BMS_Charger_Signals;

typedef struct
{
    bool BMS_ChargerEnable_value;
    bool BMS_ClearLatch_value;
    float BMS_MaxMainsCurrent_value;
    float BMS_ChargingVoltage_value;
    float BMS_ChargingCurrent_value;
    
} BMS_BrusaControls_Signals;

typedef struct
{
    bool BMS_BmsOk_value;
    bool BMS_ImdOk_value;
    bool BMS_BspdOk_value;
    bool BMS_BmsLatchedFault_value;
    bool BMS_ImdLatchedFault_value;
    bool BMS_BspdLatchedFault_value;
    
} BMS_OkStatuses_Signals;

typedef struct
{
    ContactorState BMS_AirPositive_value;
    ContactorState BMS_AirNegative_value;
    ContactorState BMS_PrechargeRelay_value;
    
} BMS_Contactors_Signals;

typedef struct
{
    float BMS_MinCellTemperature_value;
    float BMS_MaxCellTemperature_value;
    
} BMS_CellTemperatures_Signals;

typedef struct
{
    float BMS_PackVoltage_value;
    float BMS_Soc_value;
    bool BMS_SocCorrupt_value;
    float BMS_MinCellVoltage_value;
    float BMS_MaxCellVoltage_value;
    
} BMS_VoltageAndChargeStats_Signals;

typedef struct
{
    uint32_t BMS_MinCellVoltageSegment_value;
    uint32_t BMS_MinCellVoltageIdx_value;
    uint32_t BMS_MaxCellVoltageSegment_value;
    uint32_t BMS_MaxCellVoltageIdx_value;
    uint32_t BMS_MinTempSegment_value;
    uint32_t BMS_MinTempIdx_value;
    uint32_t BMS_MaxTempSegment_value;
    uint32_t BMS_MaxTempIdx_value;
    
} BMS_CellStats_Signals;

typedef struct
{
    float BMS_TractiveSystemVoltage_value;
    float BMS_TractiveSystemCurrent_value;
    float BMS_TractiveSystemPower_value;
    uint32_t BMS_AvailablePower_value;
    bool BMS_BSPDCurrentThresholdExceeded_value;
    
} BMS_TractiveSystem_Signals;

typedef struct
{
    uint32_t BMS_Hash_value;
    bool BMS_Clean_value;
    
} BMS_CommitInfo_Signals;

typedef struct
{
    uint32_t BMS_Segment0_OWC_Cells_Status_value;
    uint32_t BMS_Segment1_OWC_Cells_Status_value;
    uint32_t BMS_Segment2_OWC_Cells_Status_value;
    
} BMS_OWC_Segment0to2_Status_Signals;

typedef struct
{
    uint32_t BMS_Segment3_OWC_Cells_Status_value;
    uint32_t BMS_Segment4_OWC_Cells_Status_value;
    
} BMS_OWC_Segment3to4_Status_Signals;

typedef struct
{
    bool BMS_HVDShdnOKStatus_value;
    bool BMS_TSIlckOKStatus_value;
    
} BMS_BMSShdnNodeStatus_Signals;

typedef struct
{
    bool VC_Heartbeat_value;
    VcState VC_State_value;
    bool VC_BuzzerOn_value;
    uint32_t VC_NumberOfCanDataLogs_value;
    uint32_t VC_CanLoggingRemainingErrors_value;
    
} VC_Vitals_Signals;




/* ------------------------- Function Prototypes -------------------------- */


void app_canUtils_BMS_Vitals_pack(const BMS_Vitals_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_AlertsContext_pack(const BMS_AlertsContext_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_ImdStatus_pack(const BMS_ImdStatus_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_ImdData_pack(const BMS_ImdData_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_Charger_pack(const BMS_Charger_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_BrusaControls_pack(const BMS_BrusaControls_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_OkStatuses_pack(const BMS_OkStatuses_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_Contactors_pack(const BMS_Contactors_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_CellTemperatures_pack(const BMS_CellTemperatures_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_VoltageAndChargeStats_pack(const BMS_VoltageAndChargeStats_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_CellStats_pack(const BMS_CellStats_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_TractiveSystem_pack(const BMS_TractiveSystem_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_CommitInfo_pack(const BMS_CommitInfo_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_OWC_Segment0to2_Status_pack(const BMS_OWC_Segment0to2_Status_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_OWC_Segment3to4_Status_pack(const BMS_OWC_Segment3to4_Status_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_BMS_BMSShdnNodeStatus_pack(const BMS_BMSShdnNodeStatus_Signals* const in_msg, uint8_t* const out_data);
void app_canUtils_VC_Vitals_pack(const VC_Vitals_Signals* const in_msg, uint8_t* const out_data);





void app_canUtils_BMS_Vitals_unpack(const uint8_t* const in_data, BMS_Vitals_Signals* const out_msg);
void app_canUtils_BMS_AlertsContext_unpack(const uint8_t* const in_data, BMS_AlertsContext_Signals* const out_msg);
void app_canUtils_BMS_ImdStatus_unpack(const uint8_t* const in_data, BMS_ImdStatus_Signals* const out_msg);
void app_canUtils_BMS_ImdData_unpack(const uint8_t* const in_data, BMS_ImdData_Signals* const out_msg);
void app_canUtils_BMS_Charger_unpack(const uint8_t* const in_data, BMS_Charger_Signals* const out_msg);
void app_canUtils_BMS_BrusaControls_unpack(const uint8_t* const in_data, BMS_BrusaControls_Signals* const out_msg);
void app_canUtils_BMS_OkStatuses_unpack(const uint8_t* const in_data, BMS_OkStatuses_Signals* const out_msg);
void app_canUtils_BMS_Contactors_unpack(const uint8_t* const in_data, BMS_Contactors_Signals* const out_msg);
void app_canUtils_BMS_CellTemperatures_unpack(const uint8_t* const in_data, BMS_CellTemperatures_Signals* const out_msg);
void app_canUtils_BMS_VoltageAndChargeStats_unpack(const uint8_t* const in_data, BMS_VoltageAndChargeStats_Signals* const out_msg);
void app_canUtils_BMS_CellStats_unpack(const uint8_t* const in_data, BMS_CellStats_Signals* const out_msg);
void app_canUtils_BMS_TractiveSystem_unpack(const uint8_t* const in_data, BMS_TractiveSystem_Signals* const out_msg);
void app_canUtils_BMS_CommitInfo_unpack(const uint8_t* const in_data, BMS_CommitInfo_Signals* const out_msg);
void app_canUtils_BMS_OWC_Segment0to2_Status_unpack(const uint8_t* const in_data, BMS_OWC_Segment0to2_Status_Signals* const out_msg);
void app_canUtils_BMS_OWC_Segment3to4_Status_unpack(const uint8_t* const in_data, BMS_OWC_Segment3to4_Status_Signals* const out_msg);
void app_canUtils_BMS_BMSShdnNodeStatus_unpack(const uint8_t* const in_data, BMS_BMSShdnNodeStatus_Signals* const out_msg);
void app_canUtils_VC_Vitals_unpack(const uint8_t* const in_data, VC_Vitals_Signals* const out_msg);


