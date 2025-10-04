#include "hw_adcs.h"
#include "hw_adc.h"
#include "main.h"

static uint16_t adc1_raw_adc_values[6];
static uint16_t adc2_raw_adc_values[5];
static float    adc1_adc_voltages[6];
static float    adc2_adc_voltages[5];

static AdcChip adc1 = { .hadc            = &hadc1,
                        .htim            = &htim3,
                        .raw_adc_values  = adc1_raw_adc_values,
                        .adc_voltages    = adc1_adc_voltages,
                        .channel_count   = 6,
                        .is_differential = false };

static AdcChip adc2 = { .hadc            = &hadc2,
                        .htim            = &htim3,
                        .raw_adc_values  = adc2_raw_adc_values,
                        .adc_voltages    = adc2_adc_voltages,
                        .channel_count   = 5,
                        .is_differential = false };

const AdcChannel pump_rl_pwr_v_sns = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel bms_v_sns         = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel dam_v_sns         = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel front_v_sns       = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel pump_f_pwr_v_sns  = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel pump_rr_pwr_v_sns = { .voltage = &adc1_adc_voltages[6] };
const AdcChannel r_rad_fan_v_sns   = { .voltage = &adc2_adc_voltages[1] };
const AdcChannel inv_f_pwr_v_sns   = { .voltage = &adc2_adc_voltages[2] };
const AdcChannel inv_r_pwr_v_sns   = { .voltage = &adc2_adc_voltages[3] };
const AdcChannel rsm_v_sns         = { .voltage = &adc2_adc_voltages[4] };
const AdcChannel l_rad_fan_v_sns   = { .voltage = &adc2_adc_voltages[5] };

void hw_adcs_chipsInit()
{
    hw_adcchip_init(&adc1);
    hw_adcchip_init(&adc2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
    else if (hadc == adc2.hadc)
        hw_adcchip_updateCallback(&adc2);
}
