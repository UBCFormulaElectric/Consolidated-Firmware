/**
	@file	   	APPS.h
	@brief	  	FSM_2018 - Function library for the Accelerator Pedal Position Sensors (APPS)
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef APPS_H
#define APPS_H

#include "stm32f0xx_it.h"
#include "main.h"
#include "stdint.h"

extern volatile uint32_t APPSFaultState;

typedef enum
{
    APPS_NORMAL_MODE,
    APPS_CONTROL_LOOP_MODE
} APPS_Mode_Enum;


typedef enum
{
	APPS_Fault_State_Normal, // 											Operating normally; Operating normally (Pedal deflects passed resting point and underflows) OR pedal pushed passed max. rotation
	APPS_Fault_State_Open_Short_Circuit, // 		Open/short circuit on encoder differential lines
	APPS_Fault_State_Prim_Sec_Differ, // 				>10% difference in primary and secondary APPS readings (EV2.3.6)
	APPS_Fault_State_Brake_Activated, //					>25% pedal travel while activating brakes "APPS / Brake Pedal Plausibility Check" (EV2.5)
	APPS_Fault_State_Stuck_Max //									Pedal stuck at max torque for greater than 10 secs
} APPS_Fault_State_Enum;

// Function declarations
uint16_t GetAcceleratorPedalPosition(int Mode);

#endif
