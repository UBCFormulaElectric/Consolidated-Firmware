#include "Io_Adc.h"
#include "configs/App_SteeringAngleConstants.h"
#include "Io_SteeringAngleSensor.h"

float Io_SteeringAngleSensor_GetAngleDegree(void)
{
    // Subtract the measured voltage by the steering angle sensor voltage
    // offset. Multiply the result with VOLTAGE_TO_DEGREES to calculate the
    // steering angle in degrees.
    //
    // The scale factor used to convert the voltage to an angle is given by 360
    // deg / VDDA (3.3V)

    const float VOLTAGE_TO_DEGREES = 360.0f / 3.3f;
    return VOLTAGE_TO_DEGREES *
           (Io_Adc_GetChannel1Voltage() - STEERING_ANGLE_VOLTAGE_OFFSET);
}
