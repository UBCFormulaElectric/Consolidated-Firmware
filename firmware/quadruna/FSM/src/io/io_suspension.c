#include "io_suspension.h"
#include "hw_adc.h"

float io_suspension_getFrontLeftTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN8_SUSP_TRAVEL_FL);
}

float io_suspension_getFrontRightTravel(void)
{
    return hw_adc_getVoltage(ADC1_IN9_SUSP_TRAVEL_FR);
}
