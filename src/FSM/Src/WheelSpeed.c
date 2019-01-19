/******************************************************************************
 * Includes
 ******************************************************************************/
#include "WheelSpeed.h"
#include "Timers.h"
#include "Constants.h"
#include "arm_math.h"
/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
// Set timer clock prescaler to 192 to ensure stable wheel speed readings at high speeds (20-100km/hr) 
// and capture lowest speed of 0.48km/hr (48MHz / 192 = 250kHz; 1/250kHz = 0.000004sec)
// 2^16 = Number of ticks in TIM16/17
// 2^16 * 0.000004sec = 0.262144sec = The longest time TIM16/17 can detect between two input captures
// Slowest speed we can detect = distance Between Two Adjacent Teeth / Longest Detectable time =
// (pi * 0.52m / 48 teeth) / 0.262144sec = 0.129763m/s = 0.467148km/hr 
#define PRESCALED_WHEEL_SPEED_TIMER_FREQUENCY   SystemCoreClock / WHEEL_SPEED_TIMER_PRESCALER

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

// Conversion factor from m/s to km/h
static const float M_PER_S_TO_KM_PER_H_CONVERSION_FACTOR = 3.6;		 

// Vehicle dimensions and parameters
// Number of teeth on the reluctor rings mounted to the front hubs
static const int WHEEL_ENCODER_TICKS_PER_ROTATION = 48;		

// TIM16 input capture variables (for right wheel speed)
static volatile uint16_t prev_front_right_wheel_speed_timer_value = 0;

// TIM17 input capture variables (for left wheel speed)
static volatile uint16_t prev_front_left_wheel_speed_timer_value = 0;

// Global variables for front wheel speeds (km/h)
volatile float front_left_wheel_speed = 0;
volatile float front_right_wheel_speed = 0;

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
/**
 * @brief  Determines front wheel speed in km/h based on the period between two wheel speed sensor pulses
 * @param curr_timer_value    Current input capture value of TIM16/17
 * @param prev_timer_value    Previous input capture value of TIM16/17
 * @return  Wheel speed in km/h
 */
float CalculateWheelSpeed(uint16_t curr_timer_value, uint16_t prev_timer_value);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
float CalculateWheelSpeed(uint16_t curr_timer_value, uint16_t prev_timer_value)
{
	uint32_t delta_timer_value;
	float distance;
	float time;
	float wheelspeed = 0;

	// Compute difference between the two timer values
	if(curr_timer_value > prev_timer_value)
	{
		delta_timer_value = curr_timer_value - prev_timer_value;
	}
	else
	{
		// Handle timer overflow
		delta_timer_value = (0x10000 - prev_timer_value) + curr_timer_value;
	}

	// Calculate wheel speed in km/h
	distance = (PI * TIRE_DIAMETER_IN_METERS * M_PER_S_TO_KM_PER_H_CONVERSION_FACTOR) / WHEEL_ENCODER_TICKS_PER_ROTATION;
	time = delta_timer_value / (float)(PRESCALED_WHEEL_SPEED_TIMER_FREQUENCY);
	wheelspeed = distance / time;

	return wheelspeed;
}
/******************************************************************************
 * Function Definitions
 ******************************************************************************/




void SetWheelSpeed(Wheel_Enum Wheel, uint32_t curr_timer_val){
	if(Wheel == F_R_WHEEL){

		// Calculate front right wheel speed
		front_right_wheel_speed = CalculateWheelSpeed(curr_timer_val, prev_front_right_wheel_speed_timer_value);

		// Store current input capture value for next calculation
		prev_front_right_wheel_speed_timer_value = curr_timer_val;		

	}else if(Wheel == F_L_WHEEL){
		// Calculate front right wheel speed
		front_left_wheel_speed = CalculateWheelSpeed(curr_timer_val, prev_front_left_wheel_speed_timer_value);

		// Store current input capture value for next calculation
		prev_front_left_wheel_speed_timer_value = curr_timer_val;		
	}
}

bool GetWheelSpeedSensorsFaulted(void)
{
	return (front_left_wheel_speed > MAX_WHEEL_SPEED || front_right_wheel_speed > MAX_WHEEL_SPEED);
}


float GetLeftWheelSpeed(void){
	return front_left_wheel_speed;
}

float GetRightWheelSpeed(void){
	return front_right_wheel_speed;
}

void WS_HandleWheelSpeed(void){
	/** Transmit Wheel Speed over CAN**/
}