#pragma once

namespace io::ts {

/**
 * Initialize the tractive system driver.
 */
void init();

/**
 * Convert the given ADC voltage to tractive system voltage
 * @return The tractive system voltage in V
 */
float getVoltage();

/**
 * Convert the given ADC voltage to high resolution main current
 * @note This correspond to output 1 of DHAB S/124 (+/- 75A):
 * @return Main current from high resolution sensor.
 */
float getCurrentHighResolution();

/**
 * Convert the given ADC voltage to low resolution main current
 * @note This corresponds to output 2 of DHAB S/124 (+/- 500A):
 * @return Main current from low resolution sensor.
 */
float getCurrentLowResolution();

/**
 * Get the state of the diagnostic output pin on the voltage sensor.
 * @return Status of gpio pin for voltage sensor diagnostics.
 */
bool getVoltageSnsDiagState();

/**
 * Get the state of the "open circuit"/"short circuit" output pin on the current sensor.
 * @return Status of gpio pin for current sensor ocsc.
 */
bool getCurrentSnsDiagState();

} // namespace io::ts