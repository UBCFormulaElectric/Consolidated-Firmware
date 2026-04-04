#pragma once

#define TS_OVERCURRENT_DEBOUNCE_DURATION_MS (100U)
#define CELLS_IN_PARALLEL (4U)
// Taken from our cell's datasheet (Molicel INR-18650-P30B)
#define MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS (-30.0f)
#define MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS (9.0f)
#define MAX_TS_DISCHARGE_CURRENT_AMPS (MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)
#define MAX_TS_CHARGE_CURRENT_AMPS (MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL)

namespace app::ts
{
/**
 * Init the app tractive system module.
 */
void init();

/**
 * Get the TS voltage
 * @return The voltage in (V)
 */
float getVoltage();

/**
 * Get the TS current by choosing between the low and high resolutions
 * @return The current in A
 */
float getCurrent();

/**
 * Broadcast state of the tractive system and overcurrent warning over CAN.
 */
void broadcast();
} // namespace app::ts
