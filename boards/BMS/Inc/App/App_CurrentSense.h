#pragma once

#include "App_ErrorCode.h"

/**
 * Convert the given ADC voltage to main current 1
 * @param adc_voltage The ADC voltage to convert
 * @param main_current_1 This will be set to main current 1, in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if main_current_1 is NULL
 */
ExitCode App_CurrentSense_ConvertToMainCurrent1(
    float  adc_voltage,
    float *main_current_1);

/**
 * Convert the given ADC voltage to main current 2
 * @param adc_voltage The ADC voltage to convert
 * @param main_current_2 This will be set to main current 2, in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if main_current_2 is NULL
 */
ExitCode App_CurrentSense_ConvertToMainCurrent2(
    float  adc_voltage,
    float *main_current_2);

/**
 * Convert the given ADC voltage to AIR loop current
 * @param adc_voltage The ADC voltage to convert
 * @param air_loop_current This will be set to AIR loop current, in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if air_loop_current is NULL
 */
ExitCode App_CurrentSense_ConvertToAirLoopCurrent(
    float  adc_voltage,
    float *air_loop_current);
