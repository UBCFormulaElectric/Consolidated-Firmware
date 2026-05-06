#pragma once

#include <cstdint>

namespace app::tv::datatypes::torque_limits
{
inline constexpr float MAX_TORQUE_REQUEST_Nm     = 20.5f;  // Safety limit (actual max is 21 Nm)
inline constexpr float NOMINAL_TORQUE_REQUEST_Nm = 9.8f;   // Nominal continuous torque
inline constexpr float MAX_REGEN_TORQUE_Nm       = -15.0f; // Maximum regenerative braking torque
inline constexpr float NO_TORQUE_Nm              = 0.0f;

/**
 * Motor torque command conversion.
 *
 * DD5-14-10-POW motors accept percentage of nominal torque, not absolute torque.
 * 100% = 9.8 Nm nominal torque.
 */
[[nodiscard]] inline constexpr int16_t TORQUE_REQUEST(const float torque)
{
    return static_cast<int16_t>((torque / NOMINAL_TORQUE_REQUEST_Nm) * 1000.0f);
}

[[nodiscard]] inline constexpr float REGEN_PEDAL_MAP(const float apps)
{
    // TODO: implement actual pedal-to-regen mapping
    return apps;
}
} // namespace app::tv::datatypes::torque_limits
