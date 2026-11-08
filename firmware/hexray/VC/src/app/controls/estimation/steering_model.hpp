#pragma once

#include "controls/datatypes/wheel_set.hpp"

namespace app::controls::estimation::steering
{
/**
 * @brief Line fit mapping steering wheel angles to wheel angles
 *
 * @param steering_wheel_angle_rad: steering wheel angle in radians
 *
 * @return road wheel angles in radians, rear wheels always 0 here
 */
[[nodiscard]] app::controls::wheel_set<float> wheel_steer_angles(const float steering_wheel_angle_rad);
} // namespace app::controls::estimation::steering
