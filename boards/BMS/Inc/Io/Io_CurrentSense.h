#pragma once

#include "App_SharedExitCode.h"

/**
 * Convert the given ADC voltage to low-resolution main current
 * @note This correspond to output 1 of HSNBV-D06 (+/- 50A):
 * @param adc_voltage The ADC voltage to convert
 * @param low_res_main_current This will be set to low-resolution main current,
 *                             in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if low_res_main_current is NULL
 */
ExitCode Io_CurrentSense_ConvertToLowResolutionMainCurrent(float adc_voltage, float *low_res_main_current);

/**
 * Convert the given ADC voltage to high resolution main current
 * @note This correspond to output 2 of HSNBV-D06 (+/- 300A):
 * @param adc_voltage The ADC voltage to convert
 * @param high_res_main_current This will be set to main current, in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if high_res_main_current is NULL
 */
ExitCode Io_CurrentSense_ConvertToHighResolutionMainCurrent(float adc_voltage, float *high_res_main_current);

/**
 * Convert the given ADC voltage to AIR loop current
 * @param adc_voltage The ADC voltage to convert
 * @param air_loop_current This will be set to AIR loop current, in amps
 * @return EXIT_CODE_OUT_OF_RANGE if adc_voltage is negative
 *         EXIT_CODE_INVALID_ARGS if air_loop_current is NULL
 */
ExitCode Io_CurrentSense_ConvertToAirLoopCurrent(float adc_voltage, float *air_loop_current);
