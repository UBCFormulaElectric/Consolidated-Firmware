#include "Io_Adc.h"
#include "configs/App_SteeringAngleThresholds.h"
#include "Io_SteeringAngleSensor.h"

float Io_SteeringAngleSensor_GetAngle(void)
{
    return 360.0f / 3.3f *
           (Io_Adc_GetChannel1Voltage() - STEERING_ANGLE_VOLTAGE_OFFSET);
}
