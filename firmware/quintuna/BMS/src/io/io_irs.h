#pragma once

#include <stdbool.h>

/**
 * Check if the IR- is closed
 * @return true if IR- is closed. Else, false.
 */
bool io_irs_isNegativeClosed(void);

/**
 * Sets the IR- relay to the desired state
 */
void io_irs_setPositive(bool closed);

/**
 * Check if the IR+ is closed
 * @return true if IR+ is closed. Else, false.
 */
bool io_irs_isPositiveClosed(void);

/**
 * Set the precharge relay to the desired state
 * @param closed true to close the precharge relay, false to open it.
 */
void io_irs_setPrecharge(bool closed);

/**
 * Check if the precharge relay is closed
 * @return true if precharge relay is closed. Else, false.
 */
bool io_irs_isPrechargeClosed(void);
