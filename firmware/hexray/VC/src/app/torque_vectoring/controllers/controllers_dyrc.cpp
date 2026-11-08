#include "controllers_dyrc.hpp"
#include "torque_vectoring/controllers/controllers_config.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/tv_debug.h"

#include <algorithm>
#include <cmath>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::dyrc
{

// Configuration
static float ku = DYRC_ku; // understeer gradient

// Control Scheme
static PID pid(PID_DYRC_config);

[[nodiscard]] float computeRefYawRate(const float steer_ang_rad, const float body_velx_mps)
{
    const float r_ref = (body_velx_mps * steer_ang_rad) / (WHEELBASE_m * (1.0f + ku * body_velx_mps * body_velx_mps));
    // Friction limit: a steady turn at yaw rate r needs a_y = v * r (speed floored so the limit stays finite)
    const float r_max = DYRC_MU_LIMIT * GRAVITY / std::max(std::abs(body_velx_mps), 1.0f);
    return std::clamp(r_ref, -r_max, r_max);
}

[[nodiscard]] float computeYawMoment(const float r_actual_rad, const float steer_ang_rad, const float body_velx_mps)
{
    const float r_ref_rad     = computeRefYawRate(steer_ang_rad, body_velx_mps);
    const float yaw_moment_Nm = pid.compute(r_ref_rad, r_actual_rad, 0.0f);
    tv_debug_data.yrc         = { .r_ref = r_ref_rad, .r_err = r_ref_rad - r_actual_rad, .mz_des_Nm = yaw_moment_Nm };
    return yaw_moment_Nm;
}
} // namespace app::tv::controllers::dyrc
