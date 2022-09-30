#pragma once

// Isolated amplifier gain
#define AMPLIFIER_GAIN (8.0f)

// Voltage divider for the TS+ voltage sensing
#define TS_VOLTAGE_DIV (1.24e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.24e+3f))

/**
 * Get the tractive system voltage from the adc
 * @return The tractive system voltage in V
 */
float Io_VoltageSense_GetTractiveSystemVoltage(void);
