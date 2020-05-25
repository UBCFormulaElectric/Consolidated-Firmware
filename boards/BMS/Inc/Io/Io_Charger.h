#pragma once

#include <stdbool.h>

/**
 * Enable the PON pin of the BRUSA NLG513 charger
 */
void Io_Charger_Enable(void);

/**
 * Disable the PON pin of the BRUSA NLG513 charger
 */
void Io_Charger_Disable(void);

/**
 * Check if the BRUSA NLG513 charger is connected
 * @return true if the BRUSA NLG513 charger is connected, else false
 */
bool Io_Charger_IsConnected(void);
