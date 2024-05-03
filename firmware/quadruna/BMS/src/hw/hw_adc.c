#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "hw_utils.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[ADC1_IN4_AUX_TSENSE] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_IN4_AUX_TSENSE]);
    adc_voltages[ADC1_IN5_TS_ISENSE_50A] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_IN5_TS_ISENSE_50A]);
    adc_voltages[ADC1_IN9_TS_ISENSE_400A] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_IN9_TS_ISENSE_400A]);
    adc_voltages[ADC1_IN10_TS_VSENSE_P] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, true, raw_adc_values[ADC1_IN10_TS_VSENSE_P]);
    adc_voltages[ADC1_IN11_TS_VSENSE_N] =
        hw_adcConversions_rawAdcValueToVoltage(hadc, true, raw_adc_values[ADC1_IN11_TS_VSENSE_N]);
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
