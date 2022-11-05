#include <math.h>
#include "Io_Adc.h"
#include "Io_SteeringAngleSensor.h"

#define MIN_STEERING_VOLTAGE (0.0f)
#define MAX_STEERING_VOLTAGE (1.0f)

float Io_SteeringAngleSensor_GetAngleDegree(void)
{
    // Steering angle = (ADC Voltage - Voltage Offset) * Degree Per Volt

    const float STEERING_ANGLE_VOLTAGE_OFFSET = 1.85f;
    const float DEGREE_PER_VOLT               = 360.0f / 3.3f;
    float steering_voltage = Io_Adc_GetChannel1Voltage();
    if(!(MIN_STEERING_VOLTAGE <= steering_voltage && steering_voltage <= MAX_STEERING_VOLTAGE)){
        return NAN;
    }
    return DEGREE_PER_VOLT * (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET);
}
