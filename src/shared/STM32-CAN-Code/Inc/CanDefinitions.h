/**
 * @file  CANDefinitions.h
 * @brief CAN message header definitions
 */

#ifndef CAN_DEFINITIONS_H
#define CAN_DEFINITIONS_H

/** @brief PCB Names */
typedef enum
{
    Battery_Management_System = 0,
    Drive_Control_Module,
    Power_Distribution_Module,
    Front_Sensor_Module,
    PCB_Count
} Pcb_Enum;

/** @brief FSM Errors */
typedef enum
{    
	// Begin counting faults at 1 for boolean comparison
    FSM_APPS_FAULT_STATE_1,
    FSM_APPS_FAULT_STATE_2,
    FSM_APPS_FAULT_STATE_3,
    FSM_APPS_FAULT_STATE_4,
    FSM_STEERING_BOUND_ERROR,
    FSM_WHEEL_SPEED_BOUND_ERROR,
    FSM_BMS_HEARTBEAT_TIMEOUT_ERROR
} FsmError_Enum;

/** @brief PDM Errors */
typedef enum
{
    SYSTICK_INITIALISATION_ERROR = 0,
    MISSING_HEARTBEAT = 1,
    EFUSE_FAULT,
    CHARGER_FAULT,
    CELL_BALANCE_OVERVOLTAGE_FAULT,
    BOOST_PGOOD_FAULT,
    _12V_FAULT_OV,
    _12V_FAULT_UV,
    VBAT_FAULT,
    NUM_PDM_ERRORS
} PdmError_Enum;

/** @brief DCM Errors */
typedef enum
{
    DCM_SENSOR_RANGE_ERROR = 0,
    BMS_HEARTBEAT_TIMEOUT_ERROR_ON_DCM
} DcmError_Enum;

#endif
