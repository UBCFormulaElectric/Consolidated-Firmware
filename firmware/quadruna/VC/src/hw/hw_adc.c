#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "hw_utils.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

static uint32_t adc3_channel = ADC3_IN0_AUX_PWR_I_SNS;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        for (AdcChannel i = ADC1_IN3_24V_ACC_SENSE; i < NUM_ADC_CHANNELS; i++)
        {
            adc_voltages[i] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[i]);
        }
    }
    else if (hadc == &hadc3)
    {
        raw_adc_values[adc3_channel] = (uint16_t)HAL_ADC_GetValue(&hadc3);
        adc_voltages[adc3_channel] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[adc3_channel]);

        ADC_ChannelConfTypeDef config = { 0 };
        config.Channel                = ADC_CHANNEL_0;
        config.Rank                   = ADC_REGULAR_RANK_1;
        config.SamplingTime           = ADC3_SAMPLETIME_2CYCLES_5;
        config.SingleDiff             = ADC_SINGLE_ENDED;
        config.OffsetNumber           = ADC_OFFSET_NONE;
        config.Offset                 = 0;
        config.OffsetSign             = ADC3_OFFSET_SIGN_NEGATIVE;

        if (adc3_channel == ADC3_IN0_AUX_PWR_I_SNS)
        {
            adc3_channel   = ADC3_IN1_PUMP_PWR_I_SNS;
            config.Channel = ADC_CHANNEL_1;
        }
        else
        {
            adc3_channel   = ADC3_IN0_AUX_PWR_I_SNS;
            config.Channel = ADC_CHANNEL_0;
        }

        HAL_ADC_ConfigChannel(&hadc3, &config);
    }
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
