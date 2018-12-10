/**
	@file	   	Apps.h
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

// Function declarations
uint16_t GetAcceleratorPedalPosition(int Mode);

#endif
