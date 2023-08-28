#include "hw_adc.h"
#include <stm32f4xx.h>
#include "Io_SharedAdc.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[ADC_1_CHANNEL_5] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC_1_CHANNEL_5]);
}

uint16_t *hw_adc_getRawValuesBuffer()
{
    return raw_adc_values;
}

float Io_Adc_GetChannel5Voltage()
{
    return adc_voltages[ADC_1_CHANNEL_5];
}
