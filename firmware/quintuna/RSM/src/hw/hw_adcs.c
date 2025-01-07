#include "hw_adcs.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "main.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];
const AdcChip   adc1 = { &hadc1 };

const Adc acc_fan_i_sns        = { &adc_voltages[0] };
const Adc rad_fan_i_sns        = { &adc_voltages[1] };
const Adc coolantpressure1_3v3 = { &adc_voltages[2] };
const Adc coolantpressure2_3v3 = { &adc_voltages[3] };
const Adc lc3_out              = { &adc_voltages[4] };
const Adc susp_travel_rr_3v3   = { &adc_voltages[5] };
const Adc susp_travel_rl_3v3   = { &adc_voltages[6] };
const Adc coolanttemp2_3v3     = { &adc_voltages[7] };
const Adc coolanttemp1_3v3     = { &adc_voltages[8] };
const Adc regen_3v3_lc3_out    = { &adc_voltages[9] };

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    for (AdcChannel ch = (AdcChannel)0U; ch < NUM_ADC_CHANNELS; ch++)
    {
        adc_voltages[ch] = hw_adcConversions_rawAdcValueToVoltage(hadc, false, raw_adc_values[ch]);
    }
}

uint16_t *hw_adc_getRawValuesBuffer(void)
{
    return raw_adc_values;
}

float hw_adc_getVoltage(AdcChannel channel)
{
    assert(channel < NUM_ADC_CHANNELS);
    return adc_voltages[channel];
}
