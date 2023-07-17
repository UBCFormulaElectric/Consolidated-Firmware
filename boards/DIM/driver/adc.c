#include "adc.h"
#include <stm32f4xx.h>

#define SINGLE_ENDED_ADC_V_SCALE (3.3f)
#define DIFFERENTIAL_ADC_V_SCALE (6.6f)

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float adc_voltages[NUM_ADC_CHANNELS];

static float adc_convertRawAdcValueToVoltage(ADC_HandleTypeDef *hadc, bool is_differential, uint16_t raw_adc_value)
{
    uint16_t full_scale;
    switch (hadc->Init.Resolution)
    {
        case ADC_RESOLUTION_6B:
            full_scale = MAX_6_BITS_VALUE;
            break;

        case ADC_RESOLUTION_8B:
            full_scale = MAX_8_BITS_VALUE;
            break;

        case ADC_RESOLUTION_10B:
            full_scale = MAX_10_BITS_VALUE;
            break;

        case ADC_RESOLUTION_12B:
            full_scale = MAX_12_BITS_VALUE;
            break;

        default:
            full_scale = MAX_12_BITS_VALUE;
            break;
    }

    // Taken from the STM32 manual, the formula to convert the raw ADC
    // measurements to an absolute voltage value is as follows:
    //
    //                   V_SCALE (VDDA or 2VDDA) x ADC_DATAx
    //    V_CHANNELx = ---------------------------------------
    //                               FULL_SCALE
    // where:
    // - V_SCALE depends on whether the ADC is configured as a single ended or
    // differential input
    // - ADC_DATAx is the value measured by the ADC on channel x (right-aligned)
    // - FULL_SCALE is the maximum digital value of the ADC output. For example
    //   with 12-bit resolution, it will be 2^12 -1 = 4095 or with 8-bit
    //   resolution, 2^8 - 1 = 255.

    const float scale = (is_differential) ? DIFFERENTIAL_ADC_V_SCALE : SINGLE_ENDED_ADC_V_SCALE;

    // Offsets to the raw ADC value should be configured via cube for
    // differential ADC mode
    return scale * raw_adc_value / (float)full_scale;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    for(int i = 0; i < NUM_ADC_CHANNELS; i++) 
    {
        adc_voltages[i] = adc_convertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC_CHANNEL_5]);
    }
}

void adc_init(Adc *adc, AdcChannel channel) 
{
    adc->channel = channel;
}

uint16_t *adc_getRawAdcValuesBuffer(void)
{
    return raw_adc_values;
}

float adc_getVoltage(Adc *adc)
{
    return adc_voltages[ADC_CHANNEL_5];
}
