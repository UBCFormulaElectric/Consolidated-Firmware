#pragma once

typedef enum
{
    // ADC1 Channels
    TS_VOLTAGE = 0U,

    // ADC2 Channels
    HIGH_RES_TS_CURRENT_SENSE,
    LOW_RES_TS_CURRENT_SENSE,
    NUM_ADC_CHANNELS,
} AdcChannels_E;

void  adc_driver_init(void);
float adc_driver_getConvertedResult(AdcChannels_E adc_channel);
