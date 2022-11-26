#pragma once

#include <stdbool.h>

/**
 * @return the voltage of the primary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetPappVoltage(void);
/**
 * @return Percentage of press of accelerator pedal
 */
float Io_AcceleratorPedals_GetPapps(void);
/**
 * @return whether or not the primary accelerator pedal sensor is open or short circuit
 */
bool Io_AcceleratorPedals_PappOCSC(void);

/**
 * @return the voltage of the secondary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetSappVoltage(void);
/**
 * Gets the percentage of the accelerator pedal
 * @return The reported percentage travel of the secondary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetSapps(void);
/**
 * @return whether or not the secondary accelerator pedal sensor is open or short circuit
 */
bool Io_AcceleratorPedals_SappOCSC(void);
