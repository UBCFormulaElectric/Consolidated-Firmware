#include "hw_adcs.h"
#include "main.h"

static uint16_t adc1_raw_adc_values[7];
static float    adc1_adc_voltages[7];
static uint16_t adc3_raw_adc_values[1];
static float    adc3_adc_voltages[1];

static const AdcChip adc1 = { .hadc            = &hadc1,
                              .htim            = &htim3,
                              .raw_adc_values  = adc1_raw_adc_values,
                              .adc_voltages    = adc1_adc_voltages,
                              .channel_count   = 7,
                              .is_differential = false };

static const AdcChip adc3 = { .hadc            = &hadc3,
                              .htim            = &htim3,
                              .raw_adc_values  = adc3_raw_adc_values,
                              .adc_voltages    = adc3_adc_voltages,
                              .channel_count   = 1,
                              .is_differential = false };

const AdcChannel fan_isns     = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel ts_vsense_p  = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel ts_vsense_n  = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel shdn_isns    = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel emeter_tsns  = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel ts_isns_400a = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel ts_isns_50a  = { .voltage = &adc1_adc_voltages[6] };

const AdcChannel aux_tsns = { .voltage = &adc3_adc_voltages[0] };

void hw_adcs_chipsInit(void)
{
    hw_adcchip_init(&adc1);
    hw_adcchip_init(&adc3);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
    {
        hw_adcchip_updateCallback(&adc1);
    }
    else if (hadc == adc3.hadc)
    {
        hw_adcchip_updateCallback(&adc3);
    }
}
