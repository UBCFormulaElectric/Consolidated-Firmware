#include "io_steering.h"

// Voltage thresholds for the steering sensor (in Volts)
#define MIN_STEERING_VOLTAGE (0.0f)
#define MAX_STEERING_VOLTAGE (5.0f) // TODO: these are still theoretical

// Constants for steering angle calculation
#define STEERING_ANGLE_VOLTAGE_OFFSET (2.21f) // TODO: still need to be verified
#define DEGREE_PER_VOLT (360.0f / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE))

static const SteeringConfig *config = NULL;

void io_steering_init(const SteeringConfig *steering_config)
{
    config = steering_config;
}

float io_steering_getAngleDegrees(void)
{
    float steering_voltage = hw_adc_getVoltage(config->steering); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT *
           (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET); // Calculate and return the steering angle in degrees.
}

bool io_steering_sensorOCSC(void)
{
    return hw_gpio_readPin(config->steering_ocsc);
}
