#pragma once

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
