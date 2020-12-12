#pragma once

#include <stdbool.h>

/**
 * Check if the AIR- is closed
 * @return true if AIR- is closed. Else, false
 */
bool Io_Airs_IsAirNegativeClosed(void);

/**
 * Check if AIR+ is closed
 * @return true if AIR+ is closed. Else, false
 */
bool Io_Airs_IsAirPositiveClosed(void);

/**
 * Close AIR+
 */
void Io_Airs_CloseAirPositive(void);

/**
 * Open AIR+
 */
void Io_Airs_OpenAirPositive(void);
