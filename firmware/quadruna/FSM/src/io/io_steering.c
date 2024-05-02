#include "io_steering.h"
#include "hw_adc.h"

#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.5f)

static SteeringConfig *config;

void io_steering_init(SteeringConfig *steering_config)
{
    config = steering_config;
}

float io_steering_getAngleDegrees(void)
{
    // Steering angle = (ADC Voltage - V    oltage Offset) * Degree Per Volt
    const float STEERING_ANGLE_VOLTAGE_OFFSET = 1.85f;
    const float DEGREE_PER_VOLT               = 360.0f / 3.3f;
    float       steering_voltage              = hw_adc_getVoltage(config->steering);
    return DEGREE_PER_VOLT * (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET);
}

bool io_steering_sensorOCSC(void)
{
    float steering_voltage = hw_adc_getVoltage(config->steering);
    return !(MIN_STEERING_VOLTAGE <= steering_voltage && steering_voltage <= MAX_STEERING_VOLTAGE);
}
