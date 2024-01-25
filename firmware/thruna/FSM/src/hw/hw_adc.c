#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[ADC1_CHANNEL_0] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_0]);
    adc_voltages[ADC1_CHANNEL_1] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_1]);
    adc_voltages[ADC1_CHANNEL_2] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_2]);
    adc_voltages[ADC1_CHANNEL_3] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_3]);
    adc_voltages[ADC1_CHANNEL_4] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_4]);
    adc_voltages[ADC1_CHANNEL_5] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_5]);
    adc_voltages[ADC1_CHANNEL_6] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_6]);
    adc_voltages[ADC1_CHANNEL_7] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_7]);
    adc_voltages[ADC1_CHANNEL_8] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_8]);
    adc_voltages[ADC1_CHANNEL_9] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_9]);
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
