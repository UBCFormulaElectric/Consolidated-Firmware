#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adc.h"

// CHECK MIN/MAX VALUES FOR RSM

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

float travelFromVoltage(float voltage)
{
    // TODO: implement
    return voltage;
}

float io_suspension_getRearLeftTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN1_SUSP_TRAVEL_RL);
}

float io_suspension_getRearRightTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN4_SUSP_TRAVEL_RR);
}

bool io_suspension_leftSensorOCSC(void)
{
    float leftSensorVoltage = hw_adc_getVoltage(ADC1_IN1_SUSP_TRAVEL_RL);
    return !(MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage && leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}

bool io_suspension_rightSensorOCSC(void)
{
    float rightSensorVoltage = hw_adc_getVoltage(ADC1_IN4_SUSP_TRAVEL_RR);
    return !(MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage && rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}