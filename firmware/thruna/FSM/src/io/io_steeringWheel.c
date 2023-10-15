#include "io_steeringWheel.h"
#include "hw_adc.h"

#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.5f)

float Io_SteeringSensorVoltage(void)
{
    return Io_Adc_GetChannelVoltage(ADC1_CHANNEL_6);
}

float io_steeringWheel_getAngleDegrees(void)
{
    // Steering angle = (ADC Voltage - Voltage Offset) * Degree Per Volt

    const float STEERING_ANGLE_VOLTAGE_OFFSET = 1.85f;
    const float DEGREE_PER_VOLT               = 360.0f / 3.3f;
    float       steering_voltage              = Io_SteeringSensorVoltage();
    return DEGREE_PER_VOLT * (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET);
}

bool io_steeringWheel_sensorOCSC(void)
{
    float steering_voltage = Io_SteeringSensorVoltage();
    return !(MIN_STEERING_VOLTAGE <= steering_voltage && steering_voltage <= MAX_STEERING_VOLTAGE);
}
