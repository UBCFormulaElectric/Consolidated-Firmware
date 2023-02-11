#pragma once

#include <stdint.h>
/**
 * Get the buffer for DMA controller to write raw ADC values into
 * @return Pointer to the buffer
 */
uint16_t *Io_Adc_GetRawAdcValues(void);

/**
 * Get the voltage measured at ADC channel 3
 * @return The voltage measured at ADC channel 3, in volts
 */
float Io_Adc_GetAdcChannel3Voltage(void);

/**
 * Get the voltage measured at ADC channel 7
 * @return The voltage measured at ADC channel 7, in volts
 */
float Io_Adc_GetAdcChannel7Voltage(void);

/**
 * Get the voltage measured at ADC channel 8
 * @return The voltage measured at ADC channel 8, in volts
 */
float Io_Adc_GetAdcChannel8Voltage(void);

/**
 * Get the voltage measured at ADC channel 9
 * @return The voltage measured at ADC channel 9, in volts
 */
float Io_Adc_GetAdcChannel9Voltage(void);

/**
 * Get the voltage measured at ADC channel 10
 * @return The voltage measured at ADC channel 10, in volts
 */
float Io_Adc_GetAdcChannel10Voltage(void);

/**
 * Get the voltage measured at ADC channel 11
 * @return The voltage measured at ADC channel 11, in volts
 */
float Io_Adc_GetAdcChannel11Voltage(void);

/**
 * Get the voltage measured at ADC channel 14
 * @return The voltage measured at ADC channel 14, in volts
 */
float Io_Adc_GetAdcChannel14Voltage(void);
