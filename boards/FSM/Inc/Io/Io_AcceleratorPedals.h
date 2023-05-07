#pragma once

#include <stdbool.h>

/**
 * @return Percentage of press of accelerator pedal
 */
float Io_AcceleratorPedals_GetPapps(void);
/**
 * @return whether or not the primary accelerator pedal sensor is open or short
 * circuit
 */
bool Io_AcceleratorPedals_PappsOCSC(void);

/**
 * Gets the percentage of the accelerator pedal
 * @return The reported percentage travel of the secondary accelerator pedal
 * sensor
 */
float Io_AcceleratorPedals_GetSapps(void);
/**
 * @return whether or not the secondary accelerator pedal sensor is open or
 * short circuit
 */
bool Io_AcceleratorPedals_SappsOCSC(void);
