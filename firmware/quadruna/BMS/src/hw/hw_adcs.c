#include "hw_adcs.h"
#include "main.h"

static uint16_t      adc1_raw_adc_values[5];
static float         adc1_adc_voltages[5];
static const AdcChip adc1 = { .hadc            = &hadc1,
                              .htim            = &htim3,
                              .raw_adc_values  = adc1_raw_adc_values,
                              .adc_voltages    = adc1_adc_voltages,
                              .channel_count   = 5,
                              .is_differential = false };

const AdcChannel aux_tsns     = { .voltage = &adc1_adc_voltages[0] };
const AdcChannel ts_isns_75a  = { .voltage = &adc1_adc_voltages[1] };
const AdcChannel ts_isns_400a = { .voltage = &adc1_adc_voltages[2] };
const AdcChannel ts_vsense_p  = { .voltage = &adc1_adc_voltages[3] };
const AdcChannel ts_vsense_n  = { .voltage = &adc1_adc_voltages[4] };

void hw_adcs_chipsInit(void) {
    hw_adcchip_init(&adc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc == adc1.hadc)
        hw_adcchip_updateCallback(&adc1);
}
