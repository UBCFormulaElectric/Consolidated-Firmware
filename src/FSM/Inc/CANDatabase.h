/**
	@file		CANDatabase.h
	@brief		CAN message definitions for the 2017 vehicle
	@author		UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef CAN_DATABASE_H
#define CAN_DATABASE_H
#include "stdint.h"

// Note: DLC must be > 0

// CAN messages for inverters - they receive on RX, transmit on TX
static const uint32_t LeftInverterRX_StandardID = 0x210;
static const uint32_t RightInverterRX_StandardID = 0x211;
static const uint32_t LeftInverterTX_StandardID = 0x190;
static const uint32_t RightInverterTX_StandardID = 0x191;

// CAN message for torque request
static const uint8_t TorqueRequest_DLC = 3;

// CAN message for read request from inverter
static const uint8_t ReadRequestInverter_DLC = 3;

// CAN message for N-Lim- write to inverter
static const uint8_t NLimWriteInverter_DLC = 5;

// Use enum to automate counting standard IDs from 0x20 upwards
typedef enum {
	AccumulatorStatus_StandardID = 0x20,
	AccumulatorDischargePowerLimit_StandardID,
	AccumulatorRegenPowerLimit_StandardID,
	BrakeAPPS_StandardID,
	SteeringAngle_StandardID,
	RightWheelSpeed_StandardID,
	LeftWheelSpeed_StandardID,
	AccumulatorSOC_StandardID,
	TractionControl_LeftSlipRatio_StandardID,
	TractionControl_RightSlipRatio_StandardID,
	TractionControl_LeftMotorOutput_StandardID,
	TractionControl_RightMotorOutput_StandardID,
	TractionControl_LeftSlipError_StandardID,
	TractionControl_RightSlipError_StandardID,
	General_Error_StandardID,
	Motor_Shutdown_Error_StandardID,
	Motor_ReEnable_StandardID,
	AIR_Shutdown_Error_StandardID,
	Latch_Reset_StandardID,
	Heartbeat_StandardID,
	CellVoltage_StandardID,
	BalanceMosfet_StandardID, 
	Startup_Status_StandardID, 
	BMS_TurnOn_AllFets
} CAN_StandardIDs;

// CAN message for accumulator status
static const uint32_t AccumulatorStatus_ExtendedID = 0;
static const uint8_t AccumulatorStatus_DLC = 5;

// CAN message for accumulator discharge power limits
static const uint32_t AccumulatorDischargePowerLimit_ExtendedID = 0;
static const uint8_t AccumulatorDischargePowerLimit_DLC = 4;

// CAN message for accumulator regen power limits
static const uint32_t AccumulatorRegenPowerLimit_ExtendedID = 0;
static const uint8_t AccumulatorRegenPowerLimit_DLC = 4;

// CAN message for brake and accelerator pedal position
static const uint32_t BrakeAPPS_ExtendedID = 0;
static const uint8_t BrakeAPPS_DLC = 4;

// CAN message for steering angle
static const uint32_t SteeringAngle_ExtendedID = 0;
static const uint8_t SteeringAngle_DLC = 4;

// CAN message for right wheel speed
static const uint32_t RightWheelSpeed_ExtendedID = 0;
static const uint8_t RightWheelSpeed_DLC = 4;

// CAN message for left wheel speed
static const uint32_t LeftWheelSpeed_ExtendedID = 0;
static const uint8_t LeftWheelSpeed_DLC = 4;

// CAN message for state of charge (SOC)
static const uint32_t AccumulatorSOC_ExtendedID = 0;
static const uint8_t AccumulatorSOC_DLC = 4;

// CAN messages for traction control - Left slip ratio
static const uint32_t TractionControl_LeftSlipRatio_ExtendedID = 0;
static const uint8_t TractionControl_LeftSlipRatio_DLC = 4;

// CAN messages for traction control - Right slip ratio
static const uint32_t TractionControl_RightSlipRatio_ExtendedID = 0;
static const uint8_t TractionControl_RightSlipRatio_DLC = 4;

// CAN messages for traction control - Left motor PWM output
static const uint32_t TractionControl_LeftMotorOutput_ExtendedID = 0;
static const uint8_t TractionControl_LeftMotorOutput_DLC = 4;

// CAN messages for traction control - Right motor PWM output
static const uint32_t TractionControl_RightMotorOutput_ExtendedID = 0;
static const uint8_t TractionControl_RightMotorOutput_DLC = 4;

// CAN messages for cell voltages
static const uint32_t CellVoltage_ExtendedID = 0;
static const uint8_t CellVoltage_DLC = 4;

// CAN messages for discharge mosfets
static const uint32_t BalanceMosfet_ExtendedID = 0;
static const uint8_t BalanceMosfet_DLC = 5;

// CAN messages for pre charge completion message
static const uint32_t PreChargeComplete_ExtendedID = 0;
static const uint8_t PreChargeComplete_DLC = 1;

// CAN messages for startup message
static const uint32_t Startup_Status_ExtendedID = 0;
static const uint8_t Startup_Status_DLC = 1;

/************************** CAN ERROR MESSAGES **************************/
// CAN messages for traction control - Left slip error (desired - actual)
static const uint32_t TractionControl_LeftSlipError_ExtendedID = 0;
static const uint8_t TractionControl_LeftSlipError_DLC = 4;

// CAN messages for traction control - Right slip error (desired - actual)
static const uint32_t TractionControl_RightSlipError_ExtendedID = 0;
static const uint8_t TractionControl_RightSlipError_DLC = 4;

// CAN messages for general errors
static const uint32_t General_Error_ExtendedID = 0;
static const uint8_t General_Error_DLC = 6;

// CAN messages for motor shutdown errors
static const uint32_t Motor_Shutdown_Error_ExtendedID = 0;
static const uint8_t Motor_Shutdown_Error_DLC = 6;

// CAN messages for motor re-enable
static const uint32_t Motor_ReEnable_ExtendedID = 0;
static const uint8_t Motor_ReEnable_DLC = 1;

// CAN messages for AIR shutdown errors
static const uint32_t AIR_Shutdown_Error_ExtendedID = 0;
static const uint8_t AIR_Shutdown_Error_DLC = 6;

// CAN messages for latch reset
static const uint32_t Latch_Reset_ExtendedID = 0;
static const uint8_t Latch_Reset_DLC = 1;

// CAN messages for latch reset
static const uint32_t BMS_FET_ExtendedID = 0;
static const uint8_t BMS_FET_DLC = 1;

// CAN byte allocation for modules
typedef enum {
	Battery_Management_System = 0,
	Drive_Control_Module,
	Power_Distribution_Module,
	Front_Sensor_Module, 

	Systems_Count
} Module_Name;

// FSM General Errors
typedef enum {
	FSM_APPS_Fault_State_1 = 1, // Begin counting faults at 1 for boolean comparison
	FSM_APPS_Fault_State_2,
	FSM_APPS_Fault_State_3,
	FSM_APPS_Fault_State_4,
	FSM_SteeringBounds_Error,
	FSM_WheelSpeedBounds_Error,
	BMS_Heartbeat_Timeout_Error_On_FSM
} FSM_Errors;

// PDM General Errors
typedef enum {
	PDM_Efuse_Error,
	PDM_Misc_Error,
	BMS_Heartbeat_Timeout_Error_On_PDM
} PDM_Errors;

// DCM General Errors
typedef enum {
	DCM_Sensor_Range_Error,
	BMS_Heartbeat_Timeout_Error_On_DCM
} DCM_Errors;

/************************** HEARTBEATS **************************/

// CAN messages for heartbeats
static const uint32_t Heartbeat_ExtendedID = 0;
static const uint8_t Heartbeat_DLC = 1;

#endif
