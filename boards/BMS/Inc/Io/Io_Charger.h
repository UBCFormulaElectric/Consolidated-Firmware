#pragma once

#include <stdbool.h>

/**
 * Enable the charger
 */
void Io_Charger_Enable(void);

/**
 * Disable the charger
 */
void Io_Charger_Disable(void);

/**
 * Check if the charger is connected
 * @return true if the charger is connected, else false
 */
bool Io_Charger_IsConnected(void);
