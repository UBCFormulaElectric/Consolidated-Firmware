#pragma once

/**
 * Broadcast state of the accumulator diagnostics over CAN.
 */
void app_diagnosticsMode_broadcast(void);

/**
 * Set all cell voltages and segment temperatures to negative values
 */
void app_diagnosticsMode_invalidateValues(void);