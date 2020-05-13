#pragma once

#define VREFIN_CAL_ADDR ((uint16_t*)(uint32_t)0x1FF0F44A)
#define	ADC_12_BIT_FULL_SCALE 4095

enum
{
    VREFINT,
    CHANNEL_2,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    NUM_ADC_CHANNELS
};

float Calc_VDDA(uint16_t vrefint_data);
float Convert_ADC_Voltage(uint16_t ADC_Data, float Vdda);
