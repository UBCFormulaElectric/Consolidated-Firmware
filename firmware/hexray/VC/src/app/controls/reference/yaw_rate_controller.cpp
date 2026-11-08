#include "controls/reference/yaw_rate_controller.hpp"
#include "controls/params.hpp"
#include "controls/controls_debug.h"

#include <algorithm>
#include <cmath>

using namespace app::controls::params;

namespace app::controls::reference::yaw_rate
{

// Configuration
static constexpr app::PID::Config PID_YAW_RATE_config{
    .Kp               = 9000.0f, // tuned on the driver_sim autocross lap (with W_R = 20 in the allocator)
    .Ki               = 0.0f,
    .Kd               = 0.0f,
    .Kb               = 0.0f,
    .Kff              = 0.0f,
    .smoothing_coeff  = 0.0f,
    .out_max          = 0.0f,
    .out_min          = 0.0f,
    .max_integral     = 0.0f,
    .min_integral     = 0.0f,
    .clamp_output     = false,
    .clamp_integral   = false,
    .back_calculation = false,
    .feed_forward     = false,
    .sample_time      = 0.05f // 50 ms sample time (20 Hz)
};

static constexpr float UNDERSTEER_GRADIENT_S2PM2 = -0.0e-3f; // understeer gradient, s^2/m^2
// The reference yaw rate is limited to what the tires can hold in a steady turn: |r_ref| <= mu * g / v. Without it,
// large steer at speed asks for a yaw moment the tires cannot give. About the tire model's steady limit (1.6-1.7 g).
// downforce * mu / (mass * vx)
// ay = vx * r
// r = ay / vx
// r = (downforce * mu / (mass)) / vx
static constexpr float MU_LIMIT = 1.6f;

static float ku = UNDERSTEER_GRADIENT_S2PM2; // understeer gradient, s^2/m^2

static PID pid(PID_YAW_RATE_config);

[[nodiscard]] float computeRefYawRate(const float wheel_angle_rad, const float v_x_mps)
{
    const float r_ref_radps = (v_x_mps * wheel_angle_rad) / (WHEELBASE_m * (1.0f + ku * v_x_mps * v_x_mps));
    // Friction limit: a steady turn at yaw rate r needs a_y = v * r (speed floored so the limit stays finite)
    const float r_max_radps = MU_LIMIT * GRAVITY / std::max(std::abs(v_x_mps), 1.0f);
    return std::clamp(r_ref_radps, -r_max_radps, r_max_radps);
}

[[nodiscard]] float computeYawMoment(const float yaw_rate_radps, const float wheel_angle_rad, const float v_x_mps)
{
    const float r_ref_radps   = computeRefYawRate(wheel_angle_rad, v_x_mps);
    const float yaw_moment_Nm = pid.compute(r_ref_radps, yaw_rate_radps, 0.0f);
    tv_debug_data.yrc         = { .r_ref_radps = r_ref_radps,
                                  .r_err_radps = r_ref_radps - yaw_rate_radps,
                                  .mz_des_Nm   = yaw_moment_Nm };
    return yaw_moment_Nm;
}
} // namespace app::controls::reference::yaw_rate
