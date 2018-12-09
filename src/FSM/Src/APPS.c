/**
	@file	   	APPS.c
	@brief	  	FSM_2017 - Function library for the Accelerator Pedal Position Sensors (APPS)
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#include "APPS.h"
#include "main.h"
#include "CANDatabase.h"

// Maximum APPS values (calibrated to pedal box)
static const float PRIMARY_APPS_MAX_VALUE = 1400;
static const float SECONDARY_APPS_MAX_VALUE = 1950;

static const float APPS_PERCENT_DEADZONE = 0.03;
static const float PRIMARY_APPS_DEADZONE = PRIMARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE;
static const float SECONDARY_APPS_DEADZONE = SECONDARY_APPS_MAX_VALUE * APPS_PERCENT_DEADZONE;

// Accelerator pedal saturation point (%)
static const float PEDAL_SATURATION_POINT = 0.80;
static const float PEDAL_RELEASE_POINT = 0.04;

// Fault handling variables
static const int PEDAL_SATURATION_TIMEOUT 	=	10; 	// (sec)
static const float APPS_IMPLAUSIBILITY_DELAY 	=	1.0; 	// (sec)
static const float APPS_BPPC_THRESHOLD 				=	0.25; // (decimal %)
//CONTROL_LOOP_FREQUENCY_CUBE is set in CubeMX as a user constant.
static const int MAX_APPS_FAULTS = APPS_IMPLAUSIBILITY_DELAY * (float) CONTROL_LOOP_FREQUENCY;
static const int MAX_SATURATION_FAULTS = PEDAL_SATURATION_TIMEOUT * (int) CONTROL_LOOP_FREQUENCY;


__IO int APPSFaultCounter = 0; // Used to "time" when to trigger a fault
__IO int APPSFaultState = 0; // Used to send error CAN messages and track faults
__IO int APPS_BPPC_FLAG = 0; // Used to trigger the APPS / Brake Pedal Plausibility Check


/**
	@brief	  Gets the latest accelerator pedal position
				1. Reads the latest APPS (encoder) position values from TIM2 and TIM3
				2. Checks for improperly connected APPS encoders
				3. Checks for out-of-bounds APPS readings (outside calibrated maximum values)
				4. Checks for implausible APPS readings (>10% difference between the primary and secondary readings as per EV2.3.6)
				5. Checks for APPS/Brake Pedal Plausibility Check, where accelerator pedal is pushed >25% while brake is pushed (EV2.5)
				6. Checks for accelerator pedal stuck at max. torque for longer than 10 secs
				7. Maps the APPS readings to a 10-bit number (0 = unpressed, 1023 = fully pressed)

								APPS Fault States
								APPSFaultState = 0			Operating normally
								APPSFaultState = 0			Operating normally (Pedal deflects passed resting point and underflows) OR pedal pushed passed max. rotation
								APPSFaultState = 1			Open/short circuit on encoder differential lines
								APPSFaultState = 2			>10% difference in primary and secondary APPS readings (EV2.3.6)
								APPSFaultState = 3			>25% pedal travel while activating brakes "APPS / Brake Pedal Plausibility Check" (EV2.5)
								APPSFaultState = 4			Pedal stuck at max torque for greater than 10 secs

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
	
	if(APPSFaultState == 2)
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
		if(APPSFaultState != FSM_APPS_Fault_State_3 && APPSFaultState != FSM_APPS_Fault_State_4)
		{
			// Check for improperly connected APPS encoders (open/short circuit of APPS lines)
		if((HAL_GPIO_ReadPin(PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin ) == GPIO_PIN_SET) || (HAL_GPIO_ReadPin(SECONDARY_APPS_ALARM_GPIO_Port, SECONDARY_APPS_ALARM_Pin) == GPIO_PIN_SET))
			{
				// Check if implausibility occurs after 100msec
				if(APPSFaultCounter > MAX_APPS_FAULTS)
				{
					APPSFaultState = FSM_APPS_Fault_State_1;
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
					APPSFaultState = FSM_APPS_Fault_State_2;
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
				APPSFaultState = FSM_APPS_Fault_State_3;
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
				APPSFaultState = FSM_APPS_Fault_State_4;
			}
			APPSFaultCounter++;
			FaultFlag = 1;
		}
		
		// Check for "APPS / Brake Pedal Plausibility Check" fault state and ensure APPS returns to 5% pedal travel (EV2.5.1)
		if((APPSFaultState == 3) || (APPSFaultState == 4))
		{
			AcceleratorPedalPosition = 0;
			if(PercentPrimaryAPPSValue < 0.05)
			{
				// Reset fault variables to normal operational state
				APPSFaultState = 0;
				APPSFaultCounter = 0;
			}
		}
		else if((APPSFaultState == 1 || APPSFaultState == 2) && (FaultFlag == 1))
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
			APPSFaultState = 0;
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
