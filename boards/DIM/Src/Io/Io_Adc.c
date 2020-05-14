#include <stm32f3xx.h>
#include "Io_SharedAdc.h"
#include "Io_Adc.h"

enum
{
    CHANNEL_12,
    NUM_ADC_CHANNLES
};

static uint16_t raw_adc_values[NUM_ADC_CHANNLES];
static float    adc_voltages[NUM_ADC_CHANNLES];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[CHANNEL_12] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_adc_values[CHANNEL_12]);
}

uint16_t *Io_Adc_GetRawAdcValues(void)
{
    return raw_adc_values;
}

float Io_Adc_GetChannel12Voltage(void)
{
    return adc_voltages[CHANNEL_12];
}
