#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "hw_utils.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[ADC1_CHANNEL_3] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_3]);
    adc_voltages[ADC1_CHANNEL_7] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_7]);
    adc_voltages[ADC1_CHANNEL_8] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_8]);
    adc_voltages[ADC1_CHANNEL_9] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_9]);
    adc_voltages[ADC1_CHANNEL_10] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_10]);
    adc_voltages[ADC1_CHANNEL_11] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_11]);
    adc_voltages[ADC1_CHANNEL_14] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_14]);
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
