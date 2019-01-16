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
static const float PI = 3.1415926535;
// Conversion factor from m/s to km/h
static const float KMH_CONVERSION_FACTOR = 3.6;		 

// Vehicle dimensions and parameters
// Number of teeth on the reluctor rings mounted to the front hubs
static const int RELUCTOR_RING_TOOTH_COUNT = 48;		
// Diameter of the tire (m)
static const float TIRE_DIAMETER_IN_METERS = 0.52;				

// Wheel speed limits (km/h)
static const float MAX_WHEEL_SPEED = 200.0;

// TIM16 input capture variables (for right wheel speed)
static volatile uint16_t CurrentTIM16Value = 0;
static volatile uint16_t PreviousTIM16Value = 0;

// TIM17 input capture variables (for left wheel speed)
static volatile uint16_t CurrentTIM17Value = 0;
static volatile uint16_t PreviousTIM17Value = 0;

// Global variables for front wheel speeds (km/h)
volatile float front_left_wheel_speed = 0;
volatile float front_right_wheel_speed = 0;

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
	Distance = (PI * TIRE_DIAMETER_IN_METERS * KMH_CONVERSION_FACTOR) / RELUCTOR_RING_TOOTH_COUNT;
	Time = DeltaTimerValue / (float)(PRESCALED_WHEEL_SPEED_TIMER_FREQUENCY);
	WheelSpeed = Distance / Time;

	return WheelSpeed;
}


void SetWheelSpeed(Wheel_Enum Wheel){
	if(Wheel == F_R_WHEEL){
		// Get latest input capture value
		CurrentTIM16Value = HAL_TIM_ReadCapturedValue(&F_R_WHEELSPEED_TIMER, TIM_CHANNEL_1);

		// Calculate front right wheel speed
		front_right_wheel_speed = CalculateWheelSpeed(CurrentTIM16Value, PreviousTIM16Value);

		// Store current input capture value for next calculation
		PreviousTIM16Value = CurrentTIM16Value;		
	}else if(Wheel == FL_WHEEL){
		// Get latest input capture value
		CurrentTIM17Value = HAL_TIM_ReadCapturedValue(&F_L_WHEELSPEED_TIMER, TIM_CHANNEL_1);

		// Calculate front right wheel speed
		front_left_wheel_speed = CalculateWheelSpeed(CurrentTIM17Value, PreviousTIM17Value);

		// Store current input capture value for next calculation
		PreviousTIM17Value = CurrentTIM17Value;
	}
}


uint8_t GetWheelSpeedFaultState(void)
{
	return (front_left_wheel_speed > MAX_WHEEL_SPEED || front_right_wheel_speed > MAX_WHEEL_SPEED);
}