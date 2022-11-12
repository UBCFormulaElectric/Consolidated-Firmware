#pragma once

#include <stdbool.h>

/**
 * @return Percentage of press of accelerator pedal
 */
float Io_AcceleratorPedals_GetPapps(void);

/**
 * Gets the percentage of the accelerator pedal
 * @return The reported percentage travel of the secondary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetSapps(void);

/**
 * Checks whether the pedal voltage is faulty
 * @param pedal_voltage The pedal voltage in question
 * @return Whether or not it is faulty
 */
bool Io_AcceleratorPedals_PedalVoltageAlarm(const float pedal_voltage);
