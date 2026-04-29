#pragma once

namespace app::charger
{
/**
 * Get available charging current (A) computed from EVSE CP duty cycle.
 * Returns 0.0f when the duty cycle is outside supported ranges.
 */
float getAvailableCurrent();
} // namespace app::charger