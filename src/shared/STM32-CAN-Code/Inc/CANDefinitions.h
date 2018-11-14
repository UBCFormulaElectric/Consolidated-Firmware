/**
	@file		CANDefinitions.h
	@brief		CAN message header definitions for the 2017 vehicle
	@author		UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef CAN_HEADERS_H
#define CAN_HEADERS_H


// PCB Names
typedef enum {
	Battery_Management_System = 0,
	Drive_Control_Module,
	Power_Distribution_Module,
	Front_Sensor_Module, 
	PCB_Count	
} PCB_Enum;


// BMS Errors
//typedef enum {
// TODO
//} BmsError_Enum;

// FSM Errors
typedef enum {
	FSM_APPS_Fault_State_1 = 1, // Begin counting faults at 1 for boolean comparison
	FSM_APPS_Fault_State_2,
	FSM_APPS_Fault_State_3,
	FSM_APPS_Fault_State_4,
	FSM_SteeringBounds_Error,
	FSM_WheelSpeedBounds_Error,
	BMS_Heartbeat_Timeout_Error_On_FSM
} FsmError_Enum;

// PDM Errors
typedef enum {
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

// DCM Errors
typedef enum {
	DCM_Sensor_Range_Error = 0,
	BMS_Heartbeat_Timeout_Error_On_DCM
} DcmError_Enum;

#endif
