/**
	@file	   	APPS.c
	@brief	  	FSM_2018 - Function library for the Accelerator Pedal Position Sensors (APPS)
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#include "APPS.h"
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


volatile uint32_t APPSFaultCounter = 0; // Used to "time" when to trigger a fault
volatile uint32_t APPSFaultState = 0; // Used to send error CAN messages and track faults
volatile uint32_t APPS_BPPC_FLAG = 0; // Used to trigger the APPS / Brake Pedal Plausibility Check


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

uint16_t GetAcceleratorPedalPosition(int Mode)
{
	float RawPrimaryAPPSValue = 0.0;
	float RawSecondaryAPPSValue = 0.0;
	float PercentPrimaryAPPSValue = 0.0;
	float PercentSecondaryAPPSValue = 0.0;
	
	int TemporaryAPPSFaultCounter = 0;
	uint8_t FaultFlag = 0;
	uint16_t AcceleratorPedalPosition = 0;

	// Save APPSFaultCounter in Mode = 0
	if(Mode == 0)
	{
		TemporaryAPPSFaultCounter = APPSFaultCounter;
	}
	
	if(APPSFaultState == APPS_Fault_State_Prim_Sec_Differ)
	{
		AcceleratorPedalPosition = 0;
	}
	else
	{
		// Read latest APPS position values, from timer counter.
		RawPrimaryAPPSValue =  (float) TIM2->CNT; 
		RawSecondaryAPPSValue = 65535 - (float) TIM3->CNT;
		
		// Set a deadzone to handle underflow
		if(((65535 - RawPrimaryAPPSValue) < PRIMARY_APPS_DEADZONE) || (RawPrimaryAPPSValue < PRIMARY_APPS_DEADZONE))
		{
			RawPrimaryAPPSValue = 0.0;
		}
		
		if(((65535 - RawPrimaryAPPSValue) < SECONDARY_APPS_DEADZONE) || (RawPrimaryAPPSValue < SECONDARY_APPS_DEADZONE))
		{
			RawSecondaryAPPSValue = 0.0;
		}

		// Calculate ratio between APPS position readings and calibrated maximum values
		PercentPrimaryAPPSValue = RawPrimaryAPPSValue / PRIMARY_APPS_MAX_VALUE;
		PercentSecondaryAPPSValue = RawSecondaryAPPSValue / SECONDARY_APPS_MAX_VALUE;
	 
		// Set torque request to zero whenever brake pedal is pushed 
		// This satisfies shutting off the motors whenever the brake pedal harness is disconnected (EV2.4.4)
		// Pins are defined as User Labels in cube.
		if(HAL_GPIO_ReadPin( BSPD_BRAKE_THRES_GPIO_Port, BSPD_BRAKE_THRES_Pin))
		{
			AcceleratorPedalPosition = 0;
		}
		// Check for position readings outside bounds (when pedal is released OR when pedal 
		// is pushed passed max rotation beyond calibrated pedal box)
		else if(PercentPrimaryAPPSValue > 1.0 || PercentSecondaryAPPSValue > 1.0)
		{
			// Set pedal position to zero when encoder underflows OR when pedal pushes passed max. pedal box rotation
			AcceleratorPedalPosition = 0;
		}
		else if(PercentPrimaryAPPSValue > PEDAL_SATURATION_POINT && PercentPrimaryAPPSValue <= 1.0)
		{
			// Saturate pedal position to 100% if close to being fully pressed (accounts for deflection in pedal cluster)
			AcceleratorPedalPosition = 1023;
		}
		else if(PercentPrimaryAPPSValue < PEDAL_RELEASE_POINT)
		{
			// Map to 0 when pedal is close to the release point
			AcceleratorPedalPosition = 0;
		}
		else
		{
			// Map accelerator pedal position to 10-bit value using primary APPS reading
			AcceleratorPedalPosition = PercentPrimaryAPPSValue * 1023;
		} 
		
		// Prevent FSM_APPS_Fault_State_1 and FSM_APPS_Fault_State_2 from changing the APPSFaultState 
		// when APPSFaultState is in FSM_APPS_Fault_State_3 or FSM_APPS_Fault_State_4
		// Reason: Rules state that pedal must be released back to less than 5% before car can be operational (EV2.5.1)
		if(APPSFaultState != APPS_Fault_State_Brake_Activated && APPSFaultState != APPS_Fault_State_Stuck_Max)
		{
			// Check for improperly connected APPS encoders (open/short circuit of APPS lines)
		if((HAL_GPIO_ReadPin(PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin ) == GPIO_PIN_SET) || (HAL_GPIO_ReadPin(SECONDARY_APPS_ALARM_GPIO_Port, SECONDARY_APPS_ALARM_Pin) == GPIO_PIN_SET))
			{
				// Check if implausibility occurs after 100msec
				if(APPSFaultCounter > MAX_APPS_FAULTS)
				{
					APPSFaultState = APPS_Fault_State_Prim_Sec_Differ;
				}
				APPSFaultCounter++;
				FaultFlag = 1;
			} 
			
			// Check for implausibility between the primary and secondary APPS position readings (>10% difference)
			if(((PercentPrimaryAPPSValue - PercentSecondaryAPPSValue) > 0.1) || ((PercentSecondaryAPPSValue - PercentPrimaryAPPSValue) > 0.1))
			{
				// Check if implausibility occurs after 100msec
				if(APPSFaultCounter > MAX_APPS_FAULTS)
				{
					APPSFaultState = APPS_Fault_State_Brake_Activated;
				}
				APPSFaultCounter++;
				FaultFlag = 1;
			}
		}
		
		// Check for APPS/Brake Pedal Plausibility Check, where accelerator pedal is pushed >25% while brake is pushed (EV2.5)
		if((PercentPrimaryAPPSValue > APPS_BPPC_THRESHOLD) && HAL_GPIO_ReadPin(BSPD_BRAKE_THRES_GPIO_Port, BSPD_BRAKE_THRES_Pin))
		{
			// Check if implausibility occurs after 100msec
			if(APPSFaultCounter > MAX_APPS_FAULTS)
			{
				APPSFaultState = APPS_Fault_State_Brake_Activated;
			}
			APPSFaultCounter++;
			FaultFlag = 1;
		}  
		
		// Check if pedal is at max. torque request
		if(AcceleratorPedalPosition == 1023)
		{
			// Check if pedal is stuck at max. torque after 10 secs
			if(APPSFaultCounter > MAX_SATURATION_FAULTS)
			{
				APPSFaultState = APPS_Fault_State_Stuck_Max;
			}
			APPSFaultCounter++;
			FaultFlag = 1;
		}
		
		// Check for "APPS / Brake Pedal Plausibility Check" fault state and ensure APPS returns to 5% pedal travel (EV2.5.1)
		if((APPSFaultState == APPS_Fault_State_Brake_Activated) || (APPSFaultState == APPS_Fault_State_Stuck_Max))
		{
			AcceleratorPedalPosition = 0;
			if(PercentPrimaryAPPSValue < 0.05)
			{
				// Reset fault variables to normal operational state
				APPSFaultState = 0;
				APPSFaultCounter = 0;
			}
		}
		else if((APPSFaultState == APPS_Fault_State_Open_Short_Circuit || APPSFaultState == APPS_Fault_State_Prim_Sec_Differ) && (FaultFlag == 1))
		{
			// Set pedal position to zero if in error state AND fault flag is triggered
			// Need the fault flag check since FSM may latch onto these error states
			AcceleratorPedalPosition = 0;
		}
		else if(FaultFlag)
		{
			// Keep fault state at normal operation
			// BUT allow APPSFaultCounter to increase
			APPSFaultState = 0;
		}
		else
		{
			// Reset fault variables to normal operating state
			APPSFaultState = APPS_Fault_State_Normal;
			APPSFaultCounter = 0;
		}
	}

	// Reload APPSFaultCounter in Mode = 0
	if(Mode == 0)
	{
		APPSFaultCounter = TemporaryAPPSFaultCounter;
	}

	return AcceleratorPedalPosition;
}        
