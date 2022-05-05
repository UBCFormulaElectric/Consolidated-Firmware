#pragma once

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define R_ERROR_COMPENSATION (1.05f)

// Isolated amplifier gain
#define AMPLIFIER_GAIN (8.0f)

// Voltage divider for the TS+ voltage sensing
#define TS_VOLTAGE_DIV (1.24e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.24e+3f))

/**
 * Convert the given ADC voltage to tractive system voltage
 * @param adc_voltage The ADC voltage to convert
 * @return The tractive system voltage in V
 */
float Io_VoltageSense_GetTractiveSystemVoltage(float adc_voltage);
