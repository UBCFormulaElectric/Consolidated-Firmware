#include "torque_vectoring/estimation/steering_model.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "util_utils.hpp"

namespace
{
// Linear interpolation of inner wheel with respect to steering angle
// Note: 0.09803348769 Degrees of error at full lock
template <Decimal T> inline float inner_wheel_ang_rad(const T steer_ang_rad)
{
    return (static_cast<T>(0.2651718671) * steer_ang_rad);
}

// Linear interpolation of outer wheel with respect to steering angle
// Note: 0.3468418335 Degrees of error at full lock
template <Decimal T> inline float outer_wheel_ang_rad(const T steer_ang_rad)
{
    return (static_cast<T>(0.274579971) * steer_ang_rad);
}
} // namespace

using namespace app::tv::shared_datatypes;

namespace app::tv::estimators::steering
{
template <Decimal T> [[nodiscard]] wheel_set<T> wheel_steer_angles(const T steer_ang_rad)
{
    const T steer_ang_clamped_rad = std::clamp(
        steer_ang_rad, -static_cast<T>(vd_constants::STEER_WHEEL_RANGE_rad),
        static_cast<T>(vd_constants::STEER_WHEEL_RANGE_rad));
    /**
     * This model is based off of a table that maps steering wheel angle to wheel angle.
     * Given the steering wheel angle, we scale it into an index, and then linearly interpolate
     * across its upper and lower elements to determine the actual wheel angle
     *
     * Suspension Spreadsheet:
     * https://docs.google.com/spreadsheets/d/1gB3h8JgjsrMDLsJRusXe3zQUWp_cwrUp/edit?gid=2114943012#gid=2114943012
     */

    app::tv::shared_datatypes::wheel_set<T> wheel_ang_rad{};

    // TODO: Verify axis and steering angle sensor values
    // Positive steer_ang_rad = right turn
    if (steer_ang_clamped_rad >= static_cast<T>(0.0))
    {
        wheel_ang_rad.fr = inner_wheel_ang_rad(steer_ang_clamped_rad);
        wheel_ang_rad.fl = outer_wheel_ang_rad(steer_ang_clamped_rad);
    }
    else
    {
        wheel_ang_rad.fr = outer_wheel_ang_rad(steer_ang_clamped_rad);
        wheel_ang_rad.fl = inner_wheel_ang_rad(steer_ang_clamped_rad);
    }

    return wheel_ang_rad;
}

template wheel_set<float>  wheel_steer_angles(float steer_ang_rad);
template wheel_set<double> wheel_steer_angles(double steer_ang_rad);
} // namespace app::tv::estimators::steering
