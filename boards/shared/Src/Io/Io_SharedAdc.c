#include "App_SharedConstants.h"
#include "Io_SharedAdc.h"

float Io_SharedAdc_ConvertRawAdcValueToVoltage(
    ADC_HandleTypeDef *hadc,
    uint16_t           raw_adc_value)
{
    uint32_t full_scale;

    switch (hadc->Init.Resolution)
    {
        case ADC_RESOLUTION_6B:
        {
            full_scale = MAX_6_BITS_VALUE;
        }
        break;
        case ADC_RESOLUTION_8B:
        {
            full_scale = MAX_8_BITS_VALUE;
        }
        break;
        case ADC_RESOLUTION_10B:
        {
            full_scale = MAX_10_BITS_VALUE;
        }
        break;
        case ADC_RESOLUTION_12B:
        {
            full_scale = MAX_12_BITS_VALUE;
        }
        break;
    }

    // Taken from the STM32 manual, the formula to convert the raw ADC
    // measurements to an absolute voltage value is as follows:
    //
    //                   VDDA (3.3V) x ADC_DATAx
    //    V_CHANNELx = ----------------------------
    //                         FULL_SCALE
    // where:
    // - ADC_DATAx is the value measured by the ADC on channel x (right-aligned)
    // - FULL_SCALE is the maximum digital value of the ADC output. For example
    //   with 12-bit resolution, it will be 2^12 -1 = 4095 or with 8-bit
    //   resolution, 2^8 - 1 = 255.

    return (3.3f * raw_adc_value) / (float)full_scale;
}
