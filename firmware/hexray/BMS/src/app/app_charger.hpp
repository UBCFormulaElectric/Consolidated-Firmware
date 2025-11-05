#pragma once

namespace app {

/**
 * Charger utilities
 */
class Charger {
public:
    /**
     * Get available charging current (A) computed from EVSE CP duty cycle.
     * Returns 0.0f when the duty cycle is outside supported ranges.
     */
    static float getAvailableCurrent();
};
} // namespace app