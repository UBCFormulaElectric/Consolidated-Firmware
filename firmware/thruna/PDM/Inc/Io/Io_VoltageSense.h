#pragma once

/**
 * Get the VBAT voltage, in volts
 * @return The VBAT voltage, in volts
 */
float Io_VoltageSense_GetVbatVoltage(void);

/**
 * Get the 22V Boost voltage, in volts
 * @return The 22V Boost voltage, in volts
 */
float Io_VoltageSense_GetBoostVoltage(void);

/**
 * Get the 24V ACC voltage, in volts
 * @return The 24V ACC voltage, in volts
 */
float Io_VoltageSense_GetAccVoltage(void);
