#include "hw_adcs.c"

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

const AdcChannel bms_i_sns         = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel front_i_sns       = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel dam_i_sns         = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel rsm_i_sns         = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel pump_rr_pwr_i_sns = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel pump_f_pwr_i_sns  = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel pump_rl_pwr_i_sns = { .voltage = &adc1_adc_voltages[8] };
const AdcChannel inv_r_pwr_i_sns   = { .voltage = &adc3_adc_voltages[0] };
const AdcChannel inv_l_pwr_i_sns   = { .voltage = &adc3_adc_voltages[1] };

void hw_adcs_chipsInit()
{
    hw_adcchip_init(&adc1);
    hw_adcchip_init(&adc3);
}

