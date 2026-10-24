#pragma once

#include <cstdint>

constexpr uint32_t TS_OVERCURRENT_DEBOUNCE_DURATION_MS = 100U;
constexpr float CELLS_IN_PARALLEL                   = 4U;
// Constants taken from cell datasheet: https://www.molicel.com/wp-content/uploads/INR18650P26A-V2-80087.pdf
constexpr float  MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS = -35.0f; 
constexpr float  MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS    = 6.0f;
constexpr float  MAX_TS_DISCHARGE_CURRENT_AMPS = MAX_TS_DISCHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL;
constexpr float  MAX_TS_CHARGE_CURRENT_AMPS    = MAX_TS_CHARGE_CURRENT_PER_CELL_AMPS * CELLS_IN_PARALLEL;

namespace app::ts
{

// Returns the voltage of the tractive system
float getVoltage();

// Returns the current of the tractive system, choosing between low and high precision sensors 
float getCurrent();

// Broadcasts the state of the tractive system (voltage, current, power, voltage/sensor diagnostics) 
// and overcurrent warning over CAN
void broadcast();

} // namespace app::ts