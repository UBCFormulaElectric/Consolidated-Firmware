#pragma once

#include <stdbool.h>

/**
 * Check if the AIR- is closed
 * @return true if AIR- is closed. Else, false.
 */
bool io_airs_isNegativeClosed(void);

/**
 * Close AIR+
 */
void io_airs_closePositive(void);

/**
 * Open AIR+
 */
void io_airs_openPositive(void);

/**
 * Check if the AIR+ is closed
 * @return true if AIR+ is closed. Else, false.
 */
bool io_airs_isPositiveClosed(void);

/**
 * Close precharge relay
 */
void io_airs_closePrecharge(void);

/**
 * Open precharge relay
 */
void io_airs_openPrecharge(void);

/**
 * Check if the precharge relay is closed
 * @return true if precharge relay is closed. Else, false.
 */
bool io_airs_isPrechargeClosed(void);
