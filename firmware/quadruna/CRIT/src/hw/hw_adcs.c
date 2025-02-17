#include "hw_adcs.h"
#include "main.h"

static uint16_t adc1_raw_adc_values[1];
static float    adc1_adc_voltages[1];
static AdcChip  adc1 = { .hadc            = &hadc1,
                         .htim            = &htim3,
                         .raw_adc_values  = adc1_raw_adc_values,
                         .adc_voltages    = adc1_adc_voltages,
                         .channel_count   = 1,
                         .is_differential = false };

const AdcChannel regen = { .voltage = &adc1_adc_voltages[0] };

void hw_adcs_chipsInit(void)
{
    hw_adcchip_init(&adc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
    {
        hw_adcchip_updateCallback(&adc1);
    }
}
