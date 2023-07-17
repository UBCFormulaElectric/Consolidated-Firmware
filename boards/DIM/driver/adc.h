#pragma once

#include <stdint.h>
#include "adcConfig.h"

typedef struct {
    AdcChannel channel;
} Adc;

void adc_init(Adc* adc, AdcChannel channel);
uint16_t* adc_getRawAdcValuesBuffer(void);
float adc_getVoltage(Adc* adc);
