#include "hw_adcs.h"
#include "main.h"

static uint16_t adc1_raw_adc_values[11];
static float    adc1_adc_voltages[11];

static AdcChip adc1 = { .hadc            = &hadc1,
                        .htim            = &htim3,
                        .raw_adc_values  = adc1_raw_adc_values,
                        .adc_voltages    = adc1_adc_voltages,
                        .channel_count   = 11,
                        .is_differential = false };

const AdcChannel r_rad_fan_i_sns   = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel pump_rl_pwr_i_sns = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel bms_i_sns         = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel inv_f_pwr_i_sns   = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel inv_r_pwr_i_sns   = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel dam_i_sns         = { .voltage = &adc1_adc_voltages[6] };
const AdcChannel front_i_sns       = { .voltage = &adc1_adc_voltages[7] };
const AdcChannel pump_f_pwr_i_sns  = { .voltage = &adc1_adc_voltages[8] };
const AdcChannel pump_rr_pwr_i_sns = { .voltage = &adc1_adc_voltages[9] };
const AdcChannel rsm_i_sns         = { .voltage = &adc1_adc_voltages[10] };
const AdcChannel l_rad_fan_i_sns   = { .voltage = &adc1_adc_voltages[11] };

void hw_adcs_chipsInit()
{
    hw_adcchip_init(&adc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
}