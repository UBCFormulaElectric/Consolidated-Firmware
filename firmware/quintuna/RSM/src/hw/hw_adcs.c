#include "hw_adcs.h"
#include "main.h"
#include <assert.h>

static uint16_t raw_adc_values[4];
static float    adc_voltages[4];
const AdcChip   adc1 = { .adc_voltages    = adc_voltages,
                         .channel_count   = 4,
                         .hadc            = &hadc1,
                         .htim            = &htim2,
                         .is_differential = false,
                         .raw_adc_values  = raw_adc_values };

const AdcChannel lc3_out            = { .voltage = &adc_voltages[0] };
const AdcChannel susp_travel_rl_3v3 = { .voltage = &adc_voltages[1] };
const AdcChannel susp_travel_rr_3v3 = { .voltage = &adc_voltages[2] };
const AdcChannel bps_3v3            = { .voltage = &adc_voltages[3] };

void hw_adcs_chipsInit()
{
    hw_adcchip_init(&adc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    assert(hadc == adc1.hadc);
    hw_adcchip_updateCallback(&adc1);
}
