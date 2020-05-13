#pragma once

#include <stdint.h>
#include "Io_SharedFilters.h"

#define ADC_FILTER_SIZE 4U

uint16_t *Io_Adc_GetRawAdcReadings(void);
float     Io_Adc_GetChannel2Voltage(void);
float     Io_Adc_GetChannel7Voltage(void);
float     Io_Adc_GetChannel8Voltage(void);
float     Io_Adc_GetChannel9Voltage(void);
