#pragma once

struct TractiveSystem;

/**
 * Get the TS voltage
 * @return The TS voltage in (V)
 */
float ts_getVoltage(void);

/**
 * Broadcast periodic TS data at 100Hz
 */
void ts_tick100Hz(void);
