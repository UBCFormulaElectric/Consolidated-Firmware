#pragma once

#include <cstdint>

// =============================================================================
// MOTOR & POWERTRAIN SPECIFICATIONS
// =============================================================================

namespace app::tv::datatypes::torque_limits
{
inline constexpr float    MAX_TORQUE_REQUEST_Nm             = 15.5f;  // Safety limit (actual max is 21 Nm)
inline constexpr float    NOMINAL_TORQUE_REQUEST_Nm         = 9.8f;   // Nominal continuous torque
inline constexpr float    MAX_REGEN_TORQUE_Nm               = -15.0f; // Maximum regenerative braking torque
inline constexpr float    NO_TORQUE_Nm                      = 0.0f;
inline constexpr uint16_t POWER_TO_TORQUE_CONVERSION_FACTOR = 9550; // 60/(2*pi)*1000 for T = P/ω
/**
 * Motor torque command conversion.
 *
 * DD5-14-10-POW motors accept percentage of nominal torque, not absolute torque.
 * 100% = 9.8 Nm nominal torque.
 */
[[nodiscard]] constexpr int16_t TORQUE_REQUEST(const float torque)
{
    return static_cast<int16_t>((torque / NOMINAL_TORQUE_REQUEST_Nm) * 1000.0f);
}

[[nodiscard]] constexpr float REGEN_PEDAL_MAP(const float apps)
{
    // TODO: implement actual pedal-to-regen mapping
    return apps;
}

/**
 * Convert torque and RPM to power (kW)
 */
[[nodiscard]] constexpr float TORQUE_TO_POWER(const float torque, const float rpm)
{
    return torque * (rpm / GEAR_RATIO) / static_cast<float>(POWER_TO_TORQUE_CONVERSION_FACTOR);
}

/**
 * Convert power (kW) and RPM to torque (Nm)
 * Includes safety guard against division by zero
 */
[[nodiscard]] inline float POWER_TO_TORQUE(const float power, const float rpm)
{
    return (power * static_cast<float>(POWER_TO_TORQUE_CONVERSION_FACTOR)) / (std::fmax(rpm, 0.00001f) / GEAR_RATIO);
}
} // namespace app::tv::datatypes::torque_limits
