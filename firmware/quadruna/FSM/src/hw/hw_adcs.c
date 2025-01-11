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

const AdcChannel lc2            = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel apps2          = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel bps_f          = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel susp_travel_fl = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel susp_travel_fr = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel steering_angle = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel apps1          = { .voltage = &adc1_adc_voltages[6] };
const AdcChannel lc1            = { .voltage = &adc1_adc_voltages[7] };
const AdcChannel bps_b          = { .voltage = &adc1_adc_voltages[8] };

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
}

void hw_adcs_chipsInit(void)
{
    hw_adcchip_init(&adc1);
}
