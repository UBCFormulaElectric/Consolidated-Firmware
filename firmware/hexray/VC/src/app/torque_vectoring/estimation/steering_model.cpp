#include "torque_vectoring/estimation/steering_model.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

#include "torque_vectoring/datatypes/datatypes_vd_constants.hpp"
#include "util_utils.hpp"

namespace
{
// Linear interpolation of inner wheel with respect to steering angle
// Note: 0.09803348769 Degrees of error at full lock
inline float inner_wheel_ang_rad(float steer_ang_rad)
{
    return (0.2651718671f * steer_ang_rad);
}

// Linear interpolation of outer wheel with respect to steering angle
// Note: 0.3468418335 Degrees of error at full lock
inline float outer_wheel_ang_rad(float steer_ang_rad)
{
    return (0.274579971f * steer_ang_rad);
}
} // namespace

using namespace app::tv::datatypes;

namespace app::tv::estimators::steering
{
WheelSteerAngles wheel_steer_angles(const float steer_ang_rad)
{
    const float steer_ang_clamped_rad =
        std::clamp(steer_ang_rad, -vd_constants::STEER_WHEEL_RANGE_rad, vd_constants::STEER_WHEEL_RANGE_rad);
    /**
     * This model is based off of a table that maps steering wheel angle to wheel angle.
     * Given the steering wheel angle, we scale it into an index, and then linearly interpolate
     * across its upper and lower elements to determine the actual wheel angle
     *
     * Suspension Spreadsheet:
     * https://docs.google.com/spreadsheets/d/1gB3h8JgjsrMDLsJRusXe3zQUWp_cwrUp/edit?gid=2114943012#gid=2114943012
     */

    WheelSteerAngles wheel_angles{};

    // TODO: Verify axis and steering angle sensor values
    // Positive steer_ang_rad = right turn
    if (steer_ang_clamped_rad >= 0.0f)
    {
        wheel_angles.fr_rad = inner_wheel_ang_rad(steer_ang_clamped_rad);
        wheel_angles.fl_rad = outer_wheel_ang_rad(steer_ang_clamped_rad);
    }
    else
    {
        wheel_angles.fr_rad = outer_wheel_ang_rad(steer_ang_clamped_rad);
        wheel_angles.fl_rad = inner_wheel_ang_rad(steer_ang_clamped_rad);
    }

    return wheel_angles;
}
} // namespace app::tv::estimators::steering
