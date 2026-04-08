#pragma once

#include <algorithm>

namespace app::tv::shared_datatypes
{

// Below this speed, tire-force-based optimization is not meaningful because the wheel-speed/slip
// calculation becomes numerically fragile and the tire model can predict unrealistically large forces.
static constexpr float SPEED_MIN_MPS = 0.5f;

// Above this speed, the low-speed safeguard is fully inactive and the nominal model is used.
static constexpr float SPEED_MAX_MPS = 3.0f;

// Returns 0.0 at or below SPEED_MIN_MPS, 1.0 at or above SPEED_MAX_MPS,
// and linearly blends between them in the transition region.
[[nodiscard]] inline float velocityBlend(const float vehicle_speed_mps)
{
    if (vehicle_speed_mps <= SPEED_MIN_MPS)
        return 0.0f;

    if (vehicle_speed_mps >= SPEED_MAX_MPS)
        return 1.0f;

    return std::clamp(
        (vehicle_speed_mps - SPEED_MIN_MPS) / (SPEED_MAX_MPS - SPEED_MIN_MPS),
        0.0f,
        1.0f);
}

} // namespace app::tv::shared_datatypes
