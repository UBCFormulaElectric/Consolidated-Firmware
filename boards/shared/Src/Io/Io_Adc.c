#include "Io_Adc.h"

extern uint16_t adc_readings[NUM_ADC_CHANNELS];
extern float    v1, v2, v3, v4;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // If you were to over-sample this is where you can perform:
    // Filter_ADC_Readings(ADC_CHANNEL_COUNT, ADC_FILTER_SIZE,
    // Unfiltered_ADC_Readings, Filtered_ADC_Readings);

    v1 = Convert_ADC_Voltage(adc_readings[CHANNEL_2]);
    v2 = Convert_ADC_Voltage(adc_readings[CHANNEL_7]);
    v3 = Convert_ADC_Voltage(adc_readings[CHANNEL_8]);
    v4 = Convert_ADC_Voltage(adc_readings[CHANNEL_9]);
}

float Convert_ADC_Voltage(uint16_t ADC_Data)
{
    float voltage = (3.3f / ADC_12_BIT_FULL_SCALE) * ADC_Data;
    return voltage;
}