#pragma once

#include <stdbool.h>

/**
 * Check if the IR- is closed
 * @return true if IR- is closed. Else, false.
 */
bool io_irs_isNegativeClosed(void);

/**
 * Close IR+
 */
void io_irs_closePositive(void);

/**
 * Open IR+
 */
void io_irs_openPositive(void);

/**
 * Check if the IR+ is closed
 * @return true if IR+ is closed. Else, false.
 */
bool io_irs_isPositiveClosed(void);

/**
 * Close precharge relay
 */
void io_irs_closePrecharge(void);

/**
 * Open precharge relay
 */
void io_irs_openPrecharge(void);

/**
 * Check if the precharge relay is closed
 * @return true if precharge relay is closed. Else, false.
 */
bool io_irs_isPrechargeClosed(void);
