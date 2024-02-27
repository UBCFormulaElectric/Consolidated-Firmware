#include "io_steering.h"
#include "hw_adc.h"

// TODO: actual hardware values
#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.5f)

float io_steering_getAngleDegrees(void)
{
    // Steering angle = (ADC Voltage - Voltage Offset) * Degree Per Volt
    const float STEERING_ANGLE_VOLTAGE_OFFSET = 1.85f;
    const float DEGREE_PER_VOLT               = 360.0f / 3.3f;
    float       steering_voltage              = hw_adc_getVoltage(ADC1_IN11_STEERING_ANGLE);
    return DEGREE_PER_VOLT * (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET);
}

bool io_steering_sensorOCSC(void)
{
    float steering_voltage = hw_adc_getVoltage(ADC1_IN11_STEERING_ANGLE);
    return !(MIN_STEERING_VOLTAGE <= steering_voltage && steering_voltage <= MAX_STEERING_VOLTAGE);
}
