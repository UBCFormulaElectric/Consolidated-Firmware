#include <stm32f3xx.h>
#include "App_SharedConstants.h"
#include "Io_Adc.h"

enum
{
    CHANNEL_2,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    NUM_ADC_CHANNELS
};

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

static float Convert_ADC_Voltage(ADC_HandleTypeDef *hadc, uint16_t ADC_Data)
{
    uint32_t full_scale;

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
    }

    return (3.3f / (float)full_scale) * ADC_Data;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // If you were to over-sample this is where you can perform:
    // Filter_ADC_Readings(ADC_CHANNEL_COUNT, ADC_FILTER_SIZE,
    // Unfiltered_ADC_Readings, Filtered_ADC_Readings);

    adc_voltages[CHANNEL_2] =
        Convert_ADC_Voltage(hadc, raw_adc_values[CHANNEL_2]);
    adc_voltages[CHANNEL_7] =
        Convert_ADC_Voltage(hadc, raw_adc_values[CHANNEL_7]);
    adc_voltages[CHANNEL_8] =
        Convert_ADC_Voltage(hadc, raw_adc_values[CHANNEL_8]);
    adc_voltages[CHANNEL_9] =
        Convert_ADC_Voltage(hadc, raw_adc_values[CHANNEL_9]);
}

uint16_t *Io_Adc_GetRawAdcReadings(void)
{
    return raw_adc_values;
}

float Io_Adc_GetChannel2Voltage(void)
{
    return adc_voltages[CHANNEL_2];
}

float Io_Adc_GetChannel7Voltage(void)
{
    return adc_voltages[CHANNEL_7];
}

float Io_Adc_GetChannel8Voltage(void)
{
    return adc_voltages[CHANNEL_8];
}

float Io_Adc_GetChannel9Voltage(void)
{
    return adc_voltages[CHANNEL_9];
}
