#pragma once

#include <stdbool.h>

/**
 * Check if the start switch is turned on
 * @return A boolean indicating whether the start switch is turned on
 */
bool Io_Switches_StartSwitchIsTurnedOn(void);

/**
 * Check if the traction control switch is turned on
 * @return A boolean indicating whether the traction control switch is turned on
 */
bool Io_Switches_TractionControlSwitchIsTurnedOn(void);

/**
 * Check if the torque vectoring switch is turned on
 * @return A boolean indicating whether the torque vectoring switch is turned on
 */
bool Io_Switches_TorqueVectoringSwitchIsTurnedOn(void);
