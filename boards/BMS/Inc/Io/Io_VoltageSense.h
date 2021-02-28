#pragma once

/**
 * Convert the given ADC voltage to tractive system voltage
 * @param adc_voltage The ADC voltage to convert
 * @return The tractive system voltage in V
 */
float Io_VoltageSense_GetTractiveSystemVoltage(float adc_voltage);
