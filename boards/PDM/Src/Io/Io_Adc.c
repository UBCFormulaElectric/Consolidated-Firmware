#include "Io_Adc.h"

#include <assert.h>

#include "Io_Hal.h"
#include "Io_SharedAdc.h"

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  adc_voltages[ADC1_CHANNEL_4] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_4]);
  adc_voltages[ADC1_CHANNEL_5] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_5]);
  adc_voltages[ADC1_CHANNEL_6] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_6]);
  adc_voltages[ADC1_CHANNEL_7] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_7]);
  adc_voltages[ADC1_CHANNEL_8] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_8]);
  adc_voltages[ADC1_CHANNEL_9] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_9]);
  adc_voltages[ADC1_CHANNEL_10] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_10]);
  adc_voltages[ADC1_CHANNEL_11] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_11]);
  adc_voltages[ADC1_CHANNEL_12] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_12]);
  adc_voltages[ADC1_CHANNEL_14] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_14]);
  adc_voltages[ADC1_CHANNEL_15] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
      hadc, false, raw_adc_values[ADC1_CHANNEL_15]);
}

uint16_t *Io_Adc_GetRawAdcValues(void) { return raw_adc_values; }

float Io_Adc_GetChannelVoltage(AdcChannel channel) {
  assert(channel < NUM_ADC_CHANNELS);
  return adc_voltages[channel];
}
