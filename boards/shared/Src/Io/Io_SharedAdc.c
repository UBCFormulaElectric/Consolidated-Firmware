#include "App_SharedConstants.h"
#include "Io_SharedAdc.h"

float Io_SharedAdc_ConvertRawAdcValueToVoltage(
    ADC_HandleTypeDef *hadc,
    uint16_t           raw_adc_value)
{
    uint32_t full_scale;

    float single_diff_scaling_ratio = 6.6f;
    // TODO: Add switch statement that grabs adc single/diff setting and change
    // scaling factor appropriately switch (hadc-> <single/diff setting> case
    // ADC_SINGLE:
    // single_diff_scaling_ratio = 3.3f;
    //
    // case ADC_DIFF:
    // single_diff_scaling_ratio = 6.6f;

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

    if (raw_adc_value > 4095)
    {
        return 0.0f;
    }
    else
    {
        // TODO: change scaling ratio to be adjusted from single-ended to
        // differential-ended
        return (single_diff_scaling_ratio * raw_adc_value) / (float)full_scale;
    }
    UNUSED(full_scale);
}
