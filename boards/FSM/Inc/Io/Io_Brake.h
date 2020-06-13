#pragma once

#include <stdbool.h>

/**
 * Check if the pressure from MSP-300-2K5-P-3-N-1 exceeds the brake pressure
 * threshold. If the pressure exceeds threshold, the brake is said to be
 * actuated.
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool Io_Brake_IsActuated(void);
