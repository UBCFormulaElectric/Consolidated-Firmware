#include "Io_Adc.h"

extern uint16_t adc_readings[NUM_ADC_CHANNELS];
extern float vdda;
extern float v1, v2, v3, v4;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // If you were to over-sample this is where you can perform:
    // Filter_ADC_Readings(ADC_CHANNEL_COUNT, ADC_FILTER_SIZE, Unfiltered_ADC_Readings, Filtered_ADC_Readings);

    // Calculate value of analog voltage supply
    vdda = Calc_VDDA(adc_readings[VREFINT]);

    v1 = Convert_ADC_Voltage(adc_readings[CHANNEL_2], vdda);
    v2 = Convert_ADC_Voltage(adc_readings[CHANNEL_7], vdda);
    v3 = Convert_ADC_Voltage(adc_readings[CHANNEL_8], vdda);
    v4 = Convert_ADC_Voltage(adc_readings[CHANNEL_9], vdda);
}

float Calc_VDDA(uint16_t vrefint_data)
{
    float VDDA = 3.3f * (*VREFIN_CAL_ADDR)/vrefint_data;
    return VDDA;
}

float Convert_ADC_Voltage(uint16_t ADC_Data, float Vdda)
{
    float voltage = (Vdda/ADC_12_BIT_FULL_SCALE)*ADC_Data;
    return voltage;
}