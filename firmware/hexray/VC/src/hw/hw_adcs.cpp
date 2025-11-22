#include "main.h"

// static uint16_t adc1_raw_adc_values[6];
// static uint16_t adc2_raw_adc_values[5];
// static float    adc1_adc_voltages[6];
// static float    adc2_adc_voltages[5];

staitc vc::hw::AdcChip<6, 6> adc1();
static vc::hw::AdcChip<5, 5> adc2();
