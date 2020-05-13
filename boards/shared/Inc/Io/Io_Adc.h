#pragma once

#include <stdint.h>

uint16_t *Io_Adc_GetRawAdcReadings(void);
float     Io_Adc_GetChannel2Voltage(void);
float     Io_Adc_GetChannel7Voltage(void);
float     Io_Adc_GetChannel8Voltage(void);
float     Io_Adc_GetChannel9Voltage(void);
