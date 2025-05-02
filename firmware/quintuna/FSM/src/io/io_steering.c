#include "io_steering.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

// Voltage thresholds for the steering sensor (in Volts)
#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.2f) // TODO: need to be validated

// Constants for steering angle calculation
#define STEERING_ANGLE_VOLTAGE_OFFSET (2.21f) // TODO: still needs to be validated
#define DEGREE_PER_VOLT (360.0f / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE))

float io_steering_getAngleDegrees(void)
{
    const float steering_voltage = hw_adc_getVoltage(&str_angle); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT *
           (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET); // Calculate and return the steering angle in degrees.
}

bool io_steering_sensorOCSC(void)
{
    return !hw_gpio_readPin(&nstr_angle_ocsc);
}
