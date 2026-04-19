#include <cmath>
#include <array>

#include "app/torque_vectoring/datatypes/datatypes_vd_constants.hpp"
#include "util_units.hpp"
#include "util_utils.hpp"

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::estimators::steering
{
static constexpr std::size_t LUT_LEN = 16;

static constexpr std::array<float, LUT_LEN> inner_wheel_ang_rad = {{
    0.000000000f, 0.027750735f, 0.055501470f, 0.083077672f, 0.110828408f, 0.138404610f, 0.166155345f, 0.193906080f,
    0.221656815f, 0.249756616f, 0.277681884f, 0.305956218f, 0.334230552f, 0.362679419f, 0.391477351f, 0.395840674f
}};

static constexpr std::array<float, LUT_LEN> outer_wheel_ang_rad = {{
    0.000000000f, 0.027750735f, 0.055676003f, 0.083775804f, 0.111875605f, 0.140324472f, 0.168773339f, 0.197571271f,
    0.226718270f, 0.256039801f, 0.285884931f, 0.315904595f, 0.346622389f, 0.377689250f, 0.409454243f, 0.414166631f
}};

// We include rear wheels for convenience, in reality they are always 0
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
inline WheelSteerAngles wheel_steer_angles(float steer_ang_rad)
{
    const float steer_ang_mag_rad = CLAMP(std::abs(steer_ang_rad), 0.0f, STEER_WHEEL_RANGE_rad);

    const float       scaled_index = (static_cast<float>(LUT_LEN - 1) * steer_ang_mag_rad) / STEER_WHEEL_RANGE_rad;
    const std::size_t lower_index  = std::min(LUT_LEN - 2, static_cast<std::size_t>(scaled_index));

    const float t = scaled_index - static_cast<float>(lower_index);

    // Perform linear interpolation to get wheel angles.
    const float inner_angle_rad = std::lerp(inner_wheel_ang_rad[lower_index], inner_wheel_ang_rad[lower_index + 1], t);
    const float outer_angle_rad = std::lerp(outer_wheel_ang_rad[lower_index], outer_wheel_ang_rad[lower_index + 1], t);

    WheelSteerAngles wheel_angles{};

    // TODO: Verify axis and steering angle sensor values
    // Positive steer_ang_rad = right turn
    if (steer_ang_rad >= 0.0f)
    {
        wheel_angles.fl_rad = inner_angle_rad;
        wheel_angles.fr_rad = outer_angle_rad;
    }
    else
    {
        wheel_angles.fl_rad = -outer_angle_rad;
        wheel_angles.fr_rad = -inner_angle_rad;
    }

    return wheel_angles;
}
} // namespace app::tv::estimators::steering