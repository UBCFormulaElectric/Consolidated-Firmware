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


// Periodic msg cycle times (in MS)


// Signal starting values

// Scale/offset values for encoding/decoding signals



// Min/max allowed values for signals
// #define CANSIG_BMS_VITALS_BMS_HEARTBEAT_MIN (0)



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


/* ------------------------------- Structs -------------------------------- */





/* ------------------------- Function Prototypes -------------------------- */









