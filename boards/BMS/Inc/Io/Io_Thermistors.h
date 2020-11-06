#pragma once

#include "App_SharedExitCode.h"

/**
 * Get thermistor resistances from the cell monitoring chips.
 * @return A pointer to a 2D array containing thermistor resistances (ohms).
 */
float *Io_Thermistors_GetThermistorResistancesOhms(void);
