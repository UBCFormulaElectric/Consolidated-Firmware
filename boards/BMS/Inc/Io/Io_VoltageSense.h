#pragma once

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define R_ERROR_COMPENSATION (1.0f)

// Isolated amplifier gain
#define AMPLIFIER_GAIN (1.0f)

// Voltage divider for the TS+ voltage sensing
#define TS_VOLTAGE_DIV (20e+3f / (6 * 1e+6f + 30e3f + 20e3f))

/**
 * Convert the given ADC voltage to tractive system voltage
 * @param adc_voltage The ADC voltage to convert
 * @return The tractive system voltage in V
 */
float Io_VoltageSense_GetTractiveSystemVoltage(void);
