/******************************************************************************
 * Includes
 ******************************************************************************/
#include "WheelSpeed.h"
#include "Timers.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// Mathematical constants
static const float PI = 3.1415;
static const float KMH_CONVERSION_FACTOR = 3.6;		 // Conversion factor from m/s to km/h

// Vehicle dimensions and parameters
static const int RELUCTOR_RING_TOOTH_COUNT = 48;		// Number of teeth on the reluctor rings mounted to the front hubs
static const float TIRE_DIAMETER = 0.52;				// Diameter of the tire (m)

// Wheel speed limits (km/h)
static const float MAX_WHEEL_SPEED = 200.0;

// TIM16 input capture variables (for right wheel speed)
static volatile uint16_t CurrentTIM16Value = 0;
static volatile uint16_t PreviousTIM16Value = 0;

// TIM17 input capture variables (for left wheel speed)
static volatile uint16_t CurrentTIM17Value = 0;
static volatile uint16_t PreviousTIM17Value = 0;

// Global variables for front wheel speeds (km/h)
volatile float FLWheelSpeed = 0;
volatile float FRWheelSpeed = 0;

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

float CalculateWheelSpeed(uint16_t CurrentTimerValue, uint16_t PreviousTimerValue)
{
	uint32_t DeltaTimerValue;
	float Distance;
	float Time;
	float WheelSpeed = 0;

	// Compute difference between the two timer values
	if(CurrentTimerValue > PreviousTimerValue)
	{
		DeltaTimerValue = CurrentTimerValue - PreviousTimerValue;
	}
	else
	{
		// Handle timer overflow
		DeltaTimerValue = (0x10000 - PreviousTimerValue) + CurrentTimerValue;
	}

	// Calculate wheel speed in km/h
	Distance = (PI * TIRE_DIAMETER * KMH_CONVERSION_FACTOR) / RELUCTOR_RING_TOOTH_COUNT;
	Time = DeltaTimerValue / (float)(PRESCALED_WHEEL_SPEED_TIMER_FREQUENCY);
	WheelSpeed = Distance / Time;

	return WheelSpeed;
}


void SetWheelSpeed(Wheel_Enum Wheel){
	if(Wheel == F_R_WHEEL){
		// Get latest input capture value
		CurrentTIM16Value = HAL_TIM_ReadCapturedValue(&F_R_WHEELSPEED_TIMER, TIM_CHANNEL_1);

		// Calculate front right wheel speed
		FRWheelSpeed = CalculateWheelSpeed(CurrentTIM16Value, PreviousTIM16Value);

		// Store current input capture value for next calculation
		PreviousTIM16Value = CurrentTIM16Value;		
	}else if(Wheel == FL_WHEEL){
		// Get latest input capture value
		CurrentTIM17Value = HAL_TIM_ReadCapturedValue(&F_L_WHEELSPEED_TIMER, TIM_CHANNEL_1);

		// Calculate front right wheel speed
		FLWheelSpeed = CalculateWheelSpeed(CurrentTIM17Value, PreviousTIM17Value);

		// Store current input capture value for next calculation
		PreviousTIM17Value = CurrentTIM17Value;
	}
}


uint8_t GetWheelSpeedFaultState(void)
{
	return (FLWheelSpeed > MAX_WHEEL_SPEED || FRWheelSpeed > MAX_WHEEL_SPEED);
}