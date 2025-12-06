#include "hw_adcs.hpp"
#include "main.h"

static uint16_t adc1_raw_adc_values[6];
static float    adc1_adc_voltages[6];
static AdcChip  adc1 = { .hadc            = &hadc1,
                         .htim            = &htim2,
                         .raw_adc_values  = adc1_raw_adc_values,
                         .adc_voltages    = adc1_adc_voltages,
                         .channel_count   = 6,
                         .is_differential = false };

const AdcChannel susp_fl   = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel susp_fr   = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel apps2     = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel bps_f     = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel str_angle = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel apps1     = { .voltage = &adc1_adc_voltages[5] };

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
}

void hw_adcs_chipsInit(void)
{
    hw_adcchip_init(&adc1);
}
