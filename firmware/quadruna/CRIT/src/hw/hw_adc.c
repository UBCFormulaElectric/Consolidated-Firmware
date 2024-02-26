#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    for (AdcChannel ch = (AdcChannel)0U; ch < NUM_ADC_CHANNELS; ch++)
    {
        adc_voltages[ch] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ch]);
    }
}

uint16_t *hw_adc_getRawValuesBuffer(void)
{
    return raw_adc_values;
}

float hw_adc_getVoltage(AdcChannel channel)
{
    assert(channel < NUM_ADC_CHANNELS);
    return adc_voltages[channel];
}
