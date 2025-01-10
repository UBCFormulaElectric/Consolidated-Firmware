#include "hw_adcs.h"
#include "main.h"

static uint16_t adc1_raw_adc_values[10];
static float    adc1_adc_voltages[10];

AdcChip adc1 = { .hadc            = &hadc1,
                 .htim            = &htim3,
                 .raw_adc_values  = adc1_raw_adc_values,
                 .adc_voltages    = adc1_adc_voltages,
                 .channel_count   = 10,
                 .is_differential = false };

const AdcChannel lc4                = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel susp_travel_rl     = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel coolant_temp_1     = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel coolant_temp_2     = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel susp_travel_rr     = { .voltage = &adc1_adc_voltages[4] };
const AdcChannel lc3                = { .voltage = &adc1_adc_voltages[5] };
const AdcChannel coolant_pressure_2 = { .voltage = &adc1_adc_voltages[6] };
const AdcChannel coolant_pressure_1 = { .voltage = &adc1_adc_voltages[7] };
const AdcChannel rad_fan_i_sns      = { .voltage = &adc1_adc_voltages[8] };
const AdcChannel acc_fan_i_sns      = { .voltage = &adc1_adc_voltages[9] };

void hw_adcs_chipsInit() {
    hw_adcchip_init(&adc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
}