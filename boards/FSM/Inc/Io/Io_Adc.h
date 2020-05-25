#pragma once

#include <stdint.h>

/**
 * Get the buffer for DMA controller to write raw ADC values into
 * @return Pointer to the buffer
 */
uint16_t *Io_Adc_GetRawAdcValues(void);

/**
 * Get the voltage measured at ADC channel 1
 * @return The voltage measured at ADC channel 1, in volts
 */
float Io_Adc_GetChannel1Voltage(void);
