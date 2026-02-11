#include "controllers_dyrc.hpp"
#include "datatypes_vd_constants.hpp"

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::controllers::dyrc
{
[[nodiscard]] inline float DirectYawRateControl::computeRefYawRate(float steer_ang_rad, float body_velx_mps) const
{
    ref_yaw_rate_rad = (body_velx_mps * steer_ang_rad) / (WHEELBASE_m * (1.0f + ku * body_velx_mps * body_velx_mps));
    return ref_yaw_rate_rad;
}

[[nodiscard]] inline float
    DirectYawRateControl::computeYawMoment(float r_actual_rad, float steer_ang_rad, float body_velx_mps) const
{
    float r_ref_rad = computeRefYawRate(steer_ang_rad, body_velx_mps);
    yaw_moment_Nm = pid.compute(r_ref_rad, r_actual_rad, 0.0f);
    return yaw_moment_Nm;
}

[[nodiscard]] float getYawMoment() const
{
    return yaw_moment_Nm;
}

[[nodiscard]] float getRefYawRate() const
{
    return ref_yaw_rate_rad;
}
} // namespace app::tv::controllers::dyrc