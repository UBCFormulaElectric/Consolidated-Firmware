#pragma once

#include <stdint.h>

/**
 * Get the buffer for DMA controller to write raw ADC1 values into
 * @return Pointer to the buffer
 */
uint16_t *Io_Adc_GetRawAdc1Values(void);
/**
 * Get the buffer for DMA controller to write raw ADC2 values into
 * @return Pointer to the buffer
 */
uint16_t *Io_Adc_GetRawAdc2Values(void);

/**
 * Get the voltage measured at ADC1 channel 3
 * @return The voltage measured at ADC1 channel 3, in volts
 */
float Io_Adc_GetAdc1Channel3Voltage(void);

/**
 * Get the voltage measured at ADC2 channel 1
 * @return The voltage measured at ADC2 channel 1, in volts
 */
float Io_Adc_GetAdc2Channel1Voltage(void);

/**
 * Get the voltage measured at ADC2 channel 3
 * @return The voltage measured at ADC2 channel 3, in volts
 */
float Io_Adc_GetAdc2Channel3Voltage(void);
