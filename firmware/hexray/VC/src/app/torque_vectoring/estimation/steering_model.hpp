#include <cmath>

#include "app/torque_vectoring/datatypes/datatypes_vd_constants.hpp"
#include "util_units.hpp"

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::estimators::steering
{
// We include rear wheels for convenience, in reality they are always 0
struct WheelSteerAngles
{
    float rr_rad, rl_rad, fr_rad, fl_rad = 0.0f;
};

inline float blendedOutSteerAngleRad(float in_rad, float ack_rad)
{
    return in_rad - ACKERMAN_PERCENTAGE * (in_rad - ack_rad);
}

inline WheelSteerAngles wheel_steer_angles(float steer_ang_rad)
{
    /**
     * Ackerman Steering Angle = Steer Angle / Steering Ratio
     *
     * Note: it does not include tan. Here we are applying
     * tanf so we do not compute tan twice
     */
    float ack_steer_ang_rad = steer_ang_rad / STEER_RATIO;
    float ack_steer_ang_tan = std::tanf(ack_steer_ang_rad);

    // Ideal steering angles
    float fr_rad = std::atan2f(WHEELBASE_m * ack_steer_ang_tan, WHEELBASE_m - 0.5f * TRACK_WIDTH_m * ack_steer_ang_tan);
    float fl_rad = std::atan2f(WHEELBASE_m * ack_steer_ang_tan, WHEELBASE_m + 0.5f * TRACK_WIDTH_m * ack_steer_ang_tan);

    float out_rad, in_rad = 0.0f;

    // TODO: Verify signs/directions
    if (steer_ang_rad > 0)
    {
        fr_rad = blendedOutSteerAngleRad(fl_rad, ack_steer_ang_rad);
    }
    else
    {
        fl_rad = blendedOutSteerAngleRad(fr_rad, ack_steer_ang_rad);
    }

    return WheelSteerAngles{ .fr_rad = fr_rad, .fl_rad = fl_rad };
}
} // namespace app::tv::estimators::steering