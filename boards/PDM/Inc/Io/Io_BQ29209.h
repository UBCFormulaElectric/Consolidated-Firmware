#pragma once

#include <stdbool.h>

/**
 * Check if the BQ29209 has an overvoltage fault
 * @return true if the BQ29209 has an overvoltage fault, else false
 */
bool Io_BQ29209_IsOverVoltage(void);
