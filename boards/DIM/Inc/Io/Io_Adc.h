#pragma once

/**
 * Get the buffer for DMA controller to write raw ADC values into
 * @return Pointer to the buffer
 */
uint16_t *Io_Adc_GetRawAdcValues(void);

/**
 * Get the voltage measured at ADC channel 12
 * @return  The voltage measured at ADC channel 12
 */
float Io_Adc_GetChannel12Voltage(void);
