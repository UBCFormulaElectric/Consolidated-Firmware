#include "dynamics_estimation.hpp"

#include <cmath>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
    [[nodiscard]] float safe_vx(const float v_x_mps)
    {
        if (std::fabs(v_x_mps) >= SMALL_EPSILON)
        {
            return v_x_mps;
        }

        return v_x_mps < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
    }
} // namespace

float vehicleDynamics::est_beta_rad(const float v_x_mps, const float v_y_mps)
{
    return std::atan2(v_y_mps, safe_vx(v_x_mps));
}

float vehicleDynamics::est_Mz_N(
    const app::tv::datatypes::datatypes::wheel_set<float> &tires_Fx_N,
    const app::tv::datatypes::datatypes::wheel_set<float> &tires_Fy_N,
    const float                                            delta)
{
    constexpr float half_track_m = TRACK_WIDTH_m * 0.5f;

    // rotate front wheels into body frame
    const float cos_delta  = std::cos(delta);
    const float sin_delta  = std::sin(delta);
    const float fl_fx_body = (cos_delta * tires_Fx_N.fl) - (sin_delta * tires_Fy_N.fl);
    const float fl_fy_body = (sin_delta * tires_Fx_N.fl) + (cos_delta * tires_Fy_N.fl);
    const float fr_fx_body = (cos_delta * tires_Fx_N.fr) - (sin_delta * tires_Fy_N.fr);
    const float fr_fy_body = (sin_delta * tires_Fx_N.fr) + (cos_delta * tires_Fy_N.fr);

    // contributions to moment of each tire
    const float fl_moment = (DIST_FRONT_AXLE_CG_m * fl_fy_body) - (half_track_m * fl_fx_body);
    const float fr_moment = (DIST_FRONT_AXLE_CG_m * fr_fy_body) + (half_track_m * fr_fx_body);
    const float rl_moment = (-DIST_REAR_AXLE_CG_m * tires_Fy_N.rl) - (half_track_m * tires_Fx_N.rl);
    const float rr_moment = (-DIST_REAR_AXLE_CG_m * tires_Fy_N.rr) + (half_track_m * tires_Fx_N.rr);

    return fl_moment + fr_moment + rl_moment + rr_moment;
}

app::tv::datatypes::datatypes::wheel_set<float>
    vehicleDynamics::est_Fz_N(const float a_x_MPS2, const float a_y_MPS2, const float v_x_mps)
{
    // load transfer
    const float long_load_tf = LONG_ACCEL_TERM_VERTICAL_FORCE(a_x_MPS2);
    const float lat_load_tf  = LAT_ACCEL_TERM_VERTICAL_FORCE(a_y_MPS2);

    // downforce and cop components
    const float down_force_n = est_downforceFz_N(v_x_mps);
    // Assuming COP is a fraction of the total downforce
    const float front_cop = dynamicCOPFront(a_x_MPS2), rear_cop = 1.0f - front_cop,
                right_cop = dynamicCOPRight(a_y_MPS2), left_cop = 1.0f - right_cop;
    return {
        .fl = std::fmax(
            0.0f,
            STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) - lat_load_tf + down_force_n * front_cop * left_cop),
        .fr = std::fmax(
            0.0f,
            STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) + lat_load_tf + down_force_n * front_cop * right_cop),
        .rl = std::fmax(
            0.0f, STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) - lat_load_tf + down_force_n * rear_cop * left_cop),
        .rr = std::fmax(
            0.0f, STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) + lat_load_tf + down_force_n * rear_cop * right_cop),
    };
}
} // namespace app::tv::estimation
