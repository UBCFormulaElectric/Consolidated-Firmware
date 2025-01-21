#include "hw_adcs.h"

#include "main.h"

static uint16_t adc1_raw_adc_values[9];
static float    adc1_adc_voltages[9];
static AdcChip  adc1 = { .hadc            = &hadc1,
                         .htim            = &htim3,
                         .raw_adc_values  = adc1_raw_adc_values,
                         .adc_voltages    = adc1_adc_voltages,
                         .channel_count   = 9,
                         .is_differential = false };

static uint16_t adc3_raw_adc_values[2];
static float    adc3_adc_voltages[2];
static AdcChip  adc3 = { .hadc            = &hadc3,
                         .htim            = &htim3,
                         .raw_adc_values  = adc3_raw_adc_values,
                         .adc_voltages    = adc3_adc_voltages,
                         .channel_count   = 2,
                         .is_differential = false };

const AdcChannel acc_24v_sns     = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel lv_pwr_i_sns    = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel acc_i_sns       = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel boost_22v_sns   = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel inv_r_pwr_i_sns = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel inv_l_pwr_i_sns = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel bat_i_sns       = { .voltage = &adc1_adc_voltages[6] };
const AdcChannel shdn_pwr_i_sns  = { .voltage = &adc1_adc_voltages[7] };
const AdcChannel vbat_sns        = { .voltage = &adc1_adc_voltages[8] };
const AdcChannel aux_pwr_i_sns   = { .voltage = &adc3_adc_voltages[0] };
const AdcChannel pump_pwr_i_sns  = { .voltage = &adc3_adc_voltages[1] };

void hw_adcs_chipsInit()
{
    hw_adcchip_init(&adc1);
    hw_adcchip_init(&adc3);
}

static uint8_t ADC3_PTR = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
    else if (hadc == adc3.hadc)
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
        adc3_raw_adc_values[ADC3_PTR] = (uint16_t)HAL_ADC_GetValue(&hadc3);
        ADC_ChannelConfTypeDef config = {
            .Channel      = ADC_CHANNEL_0,
            .Rank         = ADC_REGULAR_RANK_1,
            .SamplingTime = ADC3_SAMPLETIME_2CYCLES_5,
            .SingleDiff   = ADC_SINGLE_ENDED,
            .OffsetNumber = ADC_OFFSET_NONE,
            .Offset       = 0,
            .OffsetSign   = ADC3_OFFSET_SIGN_NEGATIVE,
        };
        if (ADC3_PTR == 0)
            config.Channel = ADC_CHANNEL_1;
        else
            config.Channel = ADC_CHANNEL_0;
        ADC3_PTR = 1 - ADC3_PTR;
        HAL_ADC_ConfigChannel(&hadc3, &config);

        hw_adcchip_updateCallback(&adc3);
    }
}
