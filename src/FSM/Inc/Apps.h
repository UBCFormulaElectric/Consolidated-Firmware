/**
	@file	   	Apps.h
	@brief	  	FSM_2018 - Function library for the Accelerator Pedal Position Sensors (APPS)
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef APPS_H
#define APPS_H
#include "main.h"


// Maximum APPS values (calibrated to pedal box)
#define PRIMARY_APPS_MAX_VALUE (float) 1400
#define SECONDARY_APPS_MAX_VALUE (float) 1950

#define  APPS_PERCENT_DEADZONE (float) 0.03
#define PRIMARY_APPS_DEADZONE (float) PRIMARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE 
#define SECONDARY_APPS_DEADZONE (float) SECONDARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE

// Accelerator pedal saturation point (%)
#define PEDAL_SATURATION_POINT (float) 0.80
#define PEDAL_RELEASE_POINT (float) 0.04

// Fault handling variables
#define PEDAL_SATURATION_TIMEOUT 	(int)	10 	// (sec)
#define APPS_IMPLAUSIBILITY_DELAY  (float)	1.0 	// (sec)
#define APPS_BPPC_THRESHOLD 	(float) 0.25 // (decimal %)
#define MAX_APPS_FAULTS  (float) APPS_IMPLAUSIBILITY_DELAY * (float) CONTROL_LOOP_FREQUENCY
#define MAX_SATURATION_FAULTS (int) PEDAL_SATURATION_TIMEOUT * (int) CONTROL_LOOP_FREQUENCY


extern volatile uint32_t APPSFaultState;

typedef enum
{
    APPS_NORMAL_MODE,
    APPS_CONTROL_LOOP_MODE
} APPS_Mode_Enum;

/**
	@brief	  Gets the latest accelerator pedal position
				1. Reads the latest APPS (encoder) position values from TIM2 and TIM3
				2. Checks for improperly connected APPS encoders
				3. Checks for out-of-bounds APPS readings (outside calibrated maximum values)
				4. Checks for implausible APPS readings (>10% difference between the primary and secondary readings as per EV2.3.6)
				5. Checks for APPS/Brake Pedal Plausibility Check, where accelerator pedal is pushed >25% while brake is pushed (EV2.5)
				6. Checks for accelerator pedal stuck at max. torque for longer than 10 secs
				7. Maps the APPS readings to a 10-bit number (0 = unpressed, 1023 = fully pressed)

	@param	  Mode		0 - Normal reading without affecting APPSFaultCounter
						1 - Control loop reading (affects APPSFaultCounter)
	@return	 Accelerator pedal position (10-bit)
*/
uint16_t GetAcceleratorPedalPosition(int Mode);

#endif
