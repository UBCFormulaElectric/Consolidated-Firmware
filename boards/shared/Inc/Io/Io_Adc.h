#pragma once

#define ADC_12_BIT_FULL_SCALE 4095

enum
{
    CHANNEL_2,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    NUM_ADC_CHANNELS
};

float Convert_ADC_Voltage(uint16_t ADC_Data);
