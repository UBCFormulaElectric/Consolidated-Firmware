#include "controls/estimation/steering_model.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

#include "controls/params.hpp"
#include "util_utils.hpp"

namespace
{
// Linear interpolation of inner wheel with respect to steering angle
// Note: 0.09803348769 Degrees of error at full lock
template <Decimal T> inline T inner_wheel_angle_rad(const T steering_wheel_angle_rad)
{
    return (static_cast<T>(0.2651718671) * steering_wheel_angle_rad);
}

// Linear interpolation of outer wheel with respect to steering angle
// Note: 0.3468418335 Degrees of error at full lock
template <Decimal T> inline T outer_wheel_angle_rad(const T steering_wheel_angle_rad)
{
    return (static_cast<T>(0.274579971) * steering_wheel_angle_rad);
}
} // namespace

using namespace app::controls;

namespace app::controls::estimation::steering
{
template <Decimal T> [[nodiscard]] wheel_set<T> wheel_steer_angles(const T steering_wheel_angle_rad)
{
    const T steering_wheel_angle_clamped_rad = std::clamp(
        steering_wheel_angle_rad, -static_cast<T>(params::STEER_WHEEL_RANGE_rad),
        static_cast<T>(params::STEER_WHEEL_RANGE_rad));
    /**
     * This model is based off of a table that maps steering wheel angle to wheel angle.
     * Given the steering wheel angle, we scale it into an index, and then linearly interpolate
     * across its upper and lower elements to determine the actual wheel angle
     *
     * Suspension Spreadsheet:
     * https://docs.google.com/spreadsheets/d/1gB3h8JgjsrMDLsJRusXe3zQUWp_cwrUp/edit?gid=2114943012#gid=2114943012
     */

    app::controls::wheel_set<T> wheel_angle_rad{};

    // ISO 8855 (x forward, y left, z up): positive steering_wheel_angle_rad = left turn, and positive wheel angles turn
    // the wheels left. In a left turn the left wheel is the inner wheel.
    // TODO: Verify the steering angle sensor sign on the car matches this
    if (steering_wheel_angle_clamped_rad >= static_cast<T>(0.0))
    {
        wheel_angle_rad.fl = inner_wheel_angle_rad(steering_wheel_angle_clamped_rad);
        wheel_angle_rad.fr = outer_wheel_angle_rad(steering_wheel_angle_clamped_rad);
    }
    else
    {
        wheel_angle_rad.fl = outer_wheel_angle_rad(steering_wheel_angle_clamped_rad);
        wheel_angle_rad.fr = inner_wheel_angle_rad(steering_wheel_angle_clamped_rad);
    }

    return wheel_angle_rad;
}
template wheel_set<tv_real> wheel_steer_angles(tv_real steering_wheel_angle_rad);
} // namespace app::controls::estimation::steering
