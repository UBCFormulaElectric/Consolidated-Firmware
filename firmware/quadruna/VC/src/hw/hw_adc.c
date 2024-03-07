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
        /* On the STM32H733xx MCU, the ADC3 peripheral uses the BDMA peripheral to handle DMA transfers (not the regular
        DMA peripheral). Unfortunately, the BDMA can only transfer data to/from specific sections of memory (not regular
        RAM, where `raw_adc_values` will be placed. This means we can't use DMA for the 2 ADC3 channels, without editing
        the linker script. However, STM32 does not support multi-channel conversions per ADC peripheral without using
        DMA.

        So the only way I could figure out to get ADC3 conversions working reliably for both IN0 and IN1 was to:
        1. Configure ADC3 to read from a single channel, in interrupt mode.
        2. Start ADC conversions at 1kHz via TIM3 (same as ADC1).
        3. After a conversion finishes, read the value and then reconfigure ADC3 to read from the other channel (done in
        this interrupt).

        Highly suspicious, but works. :)
         */
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
