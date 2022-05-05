#pragma once

#include <stdbool.h>
#include <stm32f3xx.h>

/**
 * Convert the given raw ADC value measured by the given ADC handle into voltage
 * @param hadc ADC handle
 * @param is_differential True if the ADC channel has a differential input
 * @param raw_adc_value Raw ADC value
 * @return The voltage converted from the given raw ADC value
 */
float Io_SharedAdc_ConvertRawAdcValueToVoltage(ADC_HandleTypeDef *hadc, bool is_differential, uint16_t raw_adc_value);
