#include "io_steering.h"
#include "hw_adc.h"

// Voltage thresholds for the steering sensor (in Volts)
#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.5f)

// Constants for steering angle calculation
#define STEERING_ANGLE_VOLTAGE_OFFSET (2.21f)
#define DEGREE_PER_VOLT (360.0f / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE))

static const SteeringConfig *config = NULL;

void io_steering_init(const SteeringConfig *steering_config)
{
    config = steering_config;
}

float io_steering_getAngleDegrees(void)
{
    float steering_voltage = hw_adc_getVoltage(config->steering); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT * (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET); // Calculate and return the steering angle in degrees.
}

bool io_steering_sensorOCSC(void)
{
    float steering_voltage = hw_adc_getVoltage(config->steering); // Read the current steering sensor voltage.
    return !(MIN_STEERING_VOLTAGE <= steering_voltage && steering_voltage <= MAX_STEERING_VOLTAGE); // Return true if the voltage is outside the expected range.
}
