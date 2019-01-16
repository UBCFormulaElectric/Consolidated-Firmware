/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SteeringAngle.h"
#include "ADC.h"
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
// Steering angle potentiometer scaling factor in V/deg (3.3V / 360 degrees)
static const float STEERING_ANGLE_SCALING_FACTOR = 0.00917;
// Steering angle potentiometer offset voltage at zero steering angle
static const float STEERING_ANGLE_OFFSET_VOLTAGE = 1.9;

// Steering angle limits (degrees)
static const float MAX_STEERING_ANGLE = 110.0;
static const float MIN_STEERING_ANGLE = -110.0;

// Global variable for steering angle (degrees)
volatile float SteeringAngle = 0;

//Pointer to array containing the unfiltered SteeringAngleADC ADC readings
static volatile uint16_t SteeringAngleADCReadings[ADC_CHANNEL_COUNT * ADC_FILTER_SIZE];

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void CalculateSteeringAngle(float *FilteredADCReadings)
{
	float SteeringAngleVoltage = 0;

	// Calculate steering angle potentiometer voltage
	SteeringAngleVoltage = (float)(3.3 * (FilteredADCReadings[1] / 4095));

	// Convert steering angle potentiometer voltage into an angle in degrees
	SteeringAngle = (SteeringAngleVoltage - STEERING_ANGLE_OFFSET_VOLTAGE) / STEERING_ANGLE_SCALING_FACTOR;
}


uint8_t GetSteeringFaultState(void)
{
	return (SteeringAngle > MAX_STEERING_ANGLE || SteeringAngle < MIN_STEERING_ANGLE);
}


void GetSteeringAngle(void){
    float FilteredADCReadings[ADC_CHANNEL_COUNT];
    FilterADCReadings(ADC_CHANNEL_COUNT, ADC_FILTER_SIZE, SteeringAngleADCReadings, FilteredADCReadings);
    CalculateSteeringAngle(FilteredADCReadings);
}