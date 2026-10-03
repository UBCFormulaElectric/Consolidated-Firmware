#pragma once

#include "torque_vectoring/shared_datatypes/wheel_set.hpp"

namespace app::tv::estimators::steering
{
/**
 * @brief Line fit mapping steering wheel angles to wheel angles
 *
 * @param steer_ang_rad: steering wheel angle in readiants
 *
 * @return WheelSteerAngles influenced by steering wheel input, rear wheels always 0 here
 */
template <Decimal T> [[nodiscard]] app::tv::shared_datatypes::wheel_set<T> wheel_steer_angles(const T steer_ang_rad);
} // namespace app::tv::estimators::steering
