#pragma once

/**
 * Get the VBAT voltage, in volts
 * @return The VBAT voltage, in volts
 */
float Io_VoltageSense_GetVbatVoltage(void);

/**
 * Get the 24V AUX voltage, in volts
 * @return The 24V AUX voltage, in volts
 */
float Io_VoltageSense_Get22vAuxVoltage(void);

/**
 * Get the 24V ACC voltage, in volts
 * @return The 24V ACC voltage, in volts
 */
float Io_VoltageSense_Get24vAccVoltage(void);
