#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adc.h"

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

float travelFromVoltage(float voltage)
{
    // TODO: implement
    return voltage;
}

float io_suspension_getLeftTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN8_SUSP_TRAVEL_FL);
}

float io_suspension_getRightTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN9_SUSP_TRAVEL_FR);
}

bool io_suspension_leftSensorOCSC(void)
{
    float leftSensorVoltage = hw_adc_getVoltage(ADC1_IN8_SUSP_TRAVEL_FL);
    return !(MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage && leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}

bool io_suspension_rightSensorOCSC(void)
{
    float rightSensorVoltage = hw_adc_getVoltage(ADC1_IN9_SUSP_TRAVEL_FR);
    return !(MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage && rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}