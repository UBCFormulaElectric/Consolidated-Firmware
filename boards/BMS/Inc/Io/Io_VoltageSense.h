#pragma once

#include "App_ErrorCode.h"

/**
 * Convert the given ADC voltage to tractive system voltage
 * @param adc_voltage The ADC voltage to convert
 * @param tractive_system_voltage This will be set to the tractive system
 *                                voltage, in volts
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if tractive_system_voltage is NULL
 */
ExitCode Io_VoltageSense_GetTractiveSystemVoltage(
    float  adc_voltage,
    float *tractive_system_voltage);
