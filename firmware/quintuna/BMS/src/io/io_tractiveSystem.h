#pragma once

#include <stdbool.h>

/**
 * Initialize the tractive system driver.
 * @param ts_config Config struct.
 */
void io_tractiveSystem_init(void);

/**
 * Convert the given ADC voltage to tractive system voltage
 * @return The tractive system voltage in V
 */
float io_tractiveSystem_getVoltage(void);

/**
 * Convert the given ADC voltage to high resolution main current
 * @note This correspond to output 1 of DHAB S/124 (+/- 75A):
 * @return Main current from high resolution sensor.
 */
float io_tractiveSystem_getCurrentHighResolution(void);

/**
 * Convert the given ADC voltage to low resolution main current
 * @note This corresponds to output 2 of DHAB S/124 (+/- 500A):
 * @return Main current from low resolution sensor.
 */
float io_tractiveSystem_getCurrentLowResolution(void);

/**
 * Get the state of the diagnostic output pin on the voltage sensor.
 * @return Status of gpio pin for voltage sensor diagnostics.
 */
bool io_tractiveSystem_getVoltageSnsDiagState(void);

/**
 * Get the state of the "open circuit"/"short circuit" output pin on the current sensor.
 * @return Status of gpio pin for current sensor ocsc.
 */
bool io_tractiveSystem_getCurrentSnsDiagState(void);
