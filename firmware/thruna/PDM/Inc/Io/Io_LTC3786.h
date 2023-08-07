#pragma once

#include <stdbool.h>

/**
 * Check if the LTC3786 has a fault
 * @return true if the LTC3786 has a fault, else false
 */
bool Io_LTC3786_HasFault(void);
