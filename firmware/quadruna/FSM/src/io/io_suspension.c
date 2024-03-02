#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adc.h"

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
    return false;
}

bool io_suspension_rightSensorOCSC(void)
{
    return false;
}