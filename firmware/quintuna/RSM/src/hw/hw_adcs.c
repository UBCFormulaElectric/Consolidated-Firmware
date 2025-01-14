#include "hw_adcs.h"
#include <assert.h>
#include "main.h"

static uint16_t raw_adc_values[10];
static float    adc_voltages[10];
const AdcChip   adc1 = { .adc_voltages    = adc_voltages,
                         .channel_count   = 10,
                         .hadc            = &hadc1,
                         .htim            = &htim3,
                         .is_differential = false,
                         .raw_adc_values  = raw_adc_values };

const AdcChannel acc_fan_i_sns        = { &adc_voltages[0] };
const AdcChannel rad_fan_i_sns        = { &adc_voltages[1] };
const AdcChannel coolantpressure1_3v3 = { &adc_voltages[2] };
const AdcChannel coolantpressure2_3v3 = { &adc_voltages[3] };
const AdcChannel lc3_out              = { &adc_voltages[4] };
const AdcChannel susp_travel_rr_3v3   = { &adc_voltages[5] };
const AdcChannel susp_travel_rl_3v3   = { &adc_voltages[6] };
const AdcChannel coolanttemp2_3v3     = { &adc_voltages[7] };
const AdcChannel coolanttemp1_3v3     = { &adc_voltages[8] };
const AdcChannel regen_3v3_lc3_out    = { &adc_voltages[9] };
const AdcChannel bps_r_3v3            = { &adc_voltages[10] };

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    assert(hadc == adc1.hadc);
    hw_adcchip_updateCallback(&adc1);
}