#pragma once

namespace app::tv::estimators::steering
{
// We include rear wheels for convenience, in reality they are always 0
// TODO: Incorporate this with the wheel set data structure in algo step PR?
struct WheelSteerAngles
{
    float rr_rad = 0.0f;
    float rl_rad = 0.0f;
    float fr_rad = 0.0f;
    float fl_rad = 0.0f;
};

/**
 * @brief Linearly interpolate on a LUT for to determine wheel angles
 *
 * @param steer_ang_rad: steering wheel angle in readiants
 *
 * @return WheelSteerAngles influenced by steering wheel input, rear wheels always 0 here
 */
WheelSteerAngles wheel_steer_angles(float steer_ang_rad);
} // namespace app::tv::estimators::steering
