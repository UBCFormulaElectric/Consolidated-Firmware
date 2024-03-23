#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio bat_fault_gpio;
    const Gpio acc_fault_gpio;
    const AdcChannel bat_current_adc;
    const AdcChannel acc_current_adc;
} CurrentSensingConfig;
#else
EMPTY_STRUCT(CurrentSensingConfig);
#endif

/*
 * Initialize config for curent sensing pins
 * @param current_sensing_config config with shutdown gpio pins
 */
void io_currentSensing_init(const CurrentSensingConfig *current_sensing_config);

/**
 * Gets current from battery (BAT_I_SNS pin)
 * @return current from battery
 */
float io_currentSensing_batteryCurrent();

/**
 * Check if there is a fault at the battery (BAT_I_SNS_!FLT pin)
 * @return true if a battery fault is detected
 */
bool io_currentSensing_hasBatteryFault();

/**
 * Gets current from battery (ACC_I_SNS pin)
 * @return current from accumulator
 */
float io_currentSensing_accumulatorCurrent();

/**
 * Check if there is a fault at the accumulator (ACC_I_SNS_!FLT pin)
 * @return true if a accumulator fault is detected
 */
bool io_currentSensing_hasAccumulatorFault();
