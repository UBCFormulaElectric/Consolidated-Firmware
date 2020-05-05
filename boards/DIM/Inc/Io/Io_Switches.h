#pragma once

#include <stdbool.h>

/**
 * Check if the start switch is turned on
 * @return true if the start switch is turned on, else false
 */
bool Io_Switches_StartSwitchIsTurnedOn(void);

/**
 * Check if the traction control switch is turned on
 * @return true if the traction control switch is turned on, else false
 */
bool Io_Switches_TractionControlSwitchIsTurnedOn(void);

/**
 * Check if the torque vectoring switch is turned on
 * @return true if the torque vectoring switch is turned on, else false
 */
bool Io_Switches_TorqueVectoringSwitchIsTurnedOn(void);
