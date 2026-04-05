#include "dynamics_estimation.hpp"

#include <cmath>

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
[[nodiscard]] float safeLongitudinalVelocity(const float v_x_mps)
{
    if (std::fabs(v_x_mps) >= SMALL_EPSILON)
    {
        return v_x_mps;
    }

    return v_x_mps < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
}
} // namespace

float vehicleDynamics::estimateBodySlip(float v_x_mps, float v_y_mps) const
{
    return std::atan2(v_y_mps, safeLongitudinalVelocity(v_x_mps));
}

float vehicleDynamics::estimateYawMoment_Nm(
    const app::tv::datatypes::datatypes::wheel_set& longitudinal_forces_N,
    const app::tv::datatypes::datatypes::wheel_set& lateral_forces_N,
    const float steering_angle_rad) const
{
    const float cos_delta = std::cos(steering_angle_rad);
    const float sin_delta = std::sin(steering_angle_rad);
    const float half_track_m = TRACK_WIDTH_m * 0.5f;

    const float fl_fx_body = (cos_delta * longitudinal_forces_N.fl) - (sin_delta * lateral_forces_N.fl);
    const float fl_fy_body = (sin_delta * longitudinal_forces_N.fl) + (cos_delta * lateral_forces_N.fl);
    const float fr_fx_body = (cos_delta * longitudinal_forces_N.fr) - (sin_delta * lateral_forces_N.fr);
    const float fr_fy_body = (sin_delta * longitudinal_forces_N.fr) + (cos_delta * lateral_forces_N.fr);

    const float rl_fx_body = longitudinal_forces_N.rl;
    const float rl_fy_body = lateral_forces_N.rl;
    const float rr_fx_body = longitudinal_forces_N.rr;
    const float rr_fy_body = lateral_forces_N.rr;

    const float fl_moment = (DIST_FRONT_AXLE_CG_m * fl_fy_body) - (half_track_m * fl_fx_body);
    const float fr_moment = (DIST_FRONT_AXLE_CG_m * fr_fy_body) + (half_track_m * fr_fx_body);
    const float rl_moment = (-DIST_REAR_AXLE_CG_m * rl_fy_body) - (half_track_m * rl_fx_body);
    const float rr_moment = (-DIST_REAR_AXLE_CG_m * rr_fy_body) + (half_track_m * rr_fx_body);

    return fl_moment + fr_moment + rl_moment + rr_moment;
}

app::tv::datatypes::datatypes::wheel_set vehicleDynamics::estimateNormalForce_N(
    const float a_x_MPS2, const float a_y_MPS2, const float v_x_mps) const
{
    const float long_load_tf = LONG_ACCEL_TERM_VERTICAL_FORCE(a_x_MPS2);
    const float lat_load_tf  = LAT_ACCEL_TERM_VERTICAL_FORCE(a_y_MPS2);
    const float down_force_n = estimateDownForce_N(v_x_mps);
    const float front_cop    = dynamicCopFront(a_x_MPS2);
    const float right_cop    = dynamicCopRight(a_y_MPS2);

    const float front_down_force_n = down_force_n * front_cop;
    const float rear_down_force_n  = down_force_n * (1.0f - front_cop);

    return {
        .fl = std::fmax(
            0.0f,
            STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) - lat_load_tf +
                (front_down_force_n * (1.0f - right_cop))),
        .fr = std::fmax(
            0.0f,
            STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) + lat_load_tf + (front_down_force_n * right_cop)),
        .rl = std::fmax(
            0.0f,
            STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) - lat_load_tf +
                (rear_down_force_n * (1.0f - right_cop))),
        .rr = std::fmax(
            0.0f,
            STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) + lat_load_tf + (rear_down_force_n * right_cop)),
    };
}

float vehicleDynamics::dynamicCopFront(const float long_accel_mps2)
{
    (void)long_accel_mps2;
    return 1.0f - COP_REAR;
}

float vehicleDynamics::dynamicCopRight(const float lat_accel_mps2)
{
    (void)lat_accel_mps2;
    return COP_RIGHT;
}

float vehicleDynamics::estimateDrag_N(const float v_x_mps) const
{
    return 0.5f * AIR_DENSITY_KGPM3 * FRONTAL_AREA_M2 * DRAG_COEFF * v_x_mps * v_x_mps;
}

float vehicleDynamics::estimateDownForce_N(const float v_x_mps) const
{
    return 0.5f * AIR_DENSITY_KGPM3 * FRONTAL_AREA_M2 * LIFT_COEFF * v_x_mps * v_x_mps;
}
} // namespace app::tv::estimation
