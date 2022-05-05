#include "Io_Adc.h"
#include "Io_SteeringAngleSensor.h"

float Io_SteeringAngleSensor_GetAngleDegree(void)
{
    // Steering angle = (ADC Voltage - Voltage Offset) * Degree Per Volt

    const float STEERING_ANGLE_VOLTAGE_OFFSET = 1.85f;
    const float DEGREE_PER_VOLT               = 360.0f / 3.3f;
    return DEGREE_PER_VOLT * (Io_Adc_GetChannel1Voltage() - STEERING_ANGLE_VOLTAGE_OFFSET);
}
