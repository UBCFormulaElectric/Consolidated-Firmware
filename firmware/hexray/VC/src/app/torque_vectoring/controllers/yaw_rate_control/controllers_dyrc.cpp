#include "controllers_dyrc.hpp"
#include "torque_vectoring/controllers/controllers_config.hpp"
#include "torque_vectoring/datatypes/datatypes_vd_constants.hpp"

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::controllers::dyrc
{

// Configuration
static float ku = DYRC_ku; // understeer gradient

// Control Scheme
static PID pid(PID_DYRC_config);

// purely for debugging purposes
static float yaw_moment_Nm = 0.0f;
static float r_ref_rad     = 0.0f;

[[nodiscard]] inline float computeRefYawRate(const float steer_ang_rad, const float body_velx_mps)
{
    r_ref_rad = (body_velx_mps * steer_ang_rad) / (WHEELBASE_m * (1.0f + ku * body_velx_mps * body_velx_mps));
    return r_ref_rad;
}

[[nodiscard]] inline float
    computeYawMoment(const float r_actual_rad, const float steer_ang_rad, const float body_velx_mps)
{
    yaw_moment_Nm = pid.compute(computeRefYawRate(steer_ang_rad, body_velx_mps), r_actual_rad, 0.0f);
    return yaw_moment_Nm;
}

[[nodiscard]] float getYawMoment()
{
    return yaw_moment_Nm;
}

[[nodiscard]] float getRefYawRate()
{
    return r_ref_rad;
}
} // namespace app::tv::controllers::dyrc