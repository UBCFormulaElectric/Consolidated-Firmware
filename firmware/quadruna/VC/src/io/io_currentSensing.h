#pragma once
#include <stdbool.h>

/**
 * Gets current from battery (BAT_I_SNS pin)
 * @return current from battery
 */
float io_currentSensing_getBatteryCurrent();

/**
 * Check if there is a fault at the battery (BAT_I_SNS_!FLT pin)
 * @return true if a battery fault is detected
 */
bool io_currentSensing_hasBatteryFault();

/**
 * Gets current from battery (ACC_I_SNS pin)
 * @return current from accumulator
 */
float io_currentSensing_getAccumulatorCurrent();

/**
 * Check if there is a fault at the accumulator (ACC_I_SNS_!FLT pin)
 * @return true if a accumulator fault is detected
 */
bool io_currentSensing_hasAccumulatorFault();
