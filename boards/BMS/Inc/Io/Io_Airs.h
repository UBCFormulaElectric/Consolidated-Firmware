#pragma once

#include <stdbool.h>

/**
 * Check if the AIR- is on
 * @return true if AIR- is on. Else, false
 */
bool Io_Airs_IsAirNegativeClosed(void);

/**
 * Check if AIR+ is on
 * @return true if AIR+ is on. Else, false
 */
bool Io_Airs_IsAirPositiveClosed(void);

/**
 * Close AIR+
 */
void Io_Airs_CloseAirPositive(void);

/**
 * Open AIR-
 */
void Io_Airs_OpenAirPositive(void);
