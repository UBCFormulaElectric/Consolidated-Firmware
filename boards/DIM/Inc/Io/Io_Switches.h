#pragma once

#include <stdbool.h>

/**
 * Check if the start switch is turned on
 * @return true if the start switch is turned on, else false
 */
bool Io_Switches_StartSwitchIsTurnedOn(void);

/**
 * Check if the aux switch is turned on
 * @return true if the aux switch is turned on, else false
 */
bool Io_Switches_AuxSwitchIsTurnedOn(void);