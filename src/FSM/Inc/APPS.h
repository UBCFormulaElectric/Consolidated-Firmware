/**
	@file	   	APPS.h
	@brief	  	FSM_2017 - Function library for the Accelerator Pedal Position Sensors (APPS)
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef APPS_H
#define APPS_H

#include "stm32f0xx_it.h"
#include "main.h"
#include "stdint.h"

#define APPS_NORMAL_MODE 		0
#define APPS_CONTROL_LOOP_MODE 	1


// Function declarations
uint16_t GetAcceleratorPedalPosition(int Mode);

#endif
