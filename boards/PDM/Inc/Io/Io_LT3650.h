#pragma once

#include <stdbool.h>

/**
 * Check if the LT3650 has a fault
 * @return true if the LT3650 has a fault, else false
 */
bool Io_LT3650_HasFault(void);
