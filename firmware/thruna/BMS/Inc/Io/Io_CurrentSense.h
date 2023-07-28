#pragma once

#include "App_SharedExitCode.h"

/**
 * Convert the given ADC voltage to low-resolution main current
 * @note This correspond to output 1 of HSNBV-D06 (+/- 50A):
 * @param adc_voltage The ADC voltage to convert
 * @return high_res_main_current
 */
float Io_CurrentSense_GetHighResolutionMainCurrent(void);

/**
 * Convert the given ADC voltage to high resolution main current
 * @note This corresponds to output 2 of HSNBV-D06 (+/- 300A):
 * @param adc_voltage The ADC voltage to convert
 * @return low_res_main_current
 */
float Io_CurrentSense_GetLowResolutionMainCurrent(void);

/**
 * Convert the given ADC voltage to AIR loop current
 * @param adc_voltage The ADC voltage to convert
 * @param air_loop_current This will be set to AIR loop current, in amps
 * @return air_loop_current This is the current flowing through the shutdown loop to power the AIRs
 */
float Io_CurrentSense_GetAirLoopCurrent(void);
