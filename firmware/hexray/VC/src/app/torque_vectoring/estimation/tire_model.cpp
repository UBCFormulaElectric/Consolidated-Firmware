#include "tire_model.hpp"

#include <cmath>
#include <dual.hpp>

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
[[nodiscard]] float safeMagnitude(const float value)
{
    return std::fmax(std::fabs(value), SMALL_EPSILON);
}
} // namespace

[[nodiscard]] TireModel::Outputs TireModel::estimate(const StateInputs& inputs)
{
    const auto wheel_velocities =
        wheelVelocities(inputs.vehicle_velocity_x_mps, inputs.vehicle_velocity_y_mps, inputs.yaw_rate_radps);
    wheel_vel_x_mps_ = wheel_velocities.x_mps;
    wheel_vel_y_mps_ = wheel_velocities.y_mps;
    estimateSlipAngle(inputs.steering_angle_rad);
    estimateSlipRatio(inputs.wheel_angular_velocity_radps);
    computePureLongitudinalForce(inputs.normal_load_N);
    computePureLateralForce(inputs.normal_load_N);
    return tire_outputs_;
}

[[nodiscard]] float TireModel::slipRatioToWheelAngularVelocity(const float slip_ratio, const float wheel_vel_x_mps) const
{
    const float wheel_surface_speed_mps = wheel_vel_x_mps * (1.0f + slip_ratio);
    return wheel_surface_speed_mps / safeMagnitude(WHEEL_RADIUS_M);
}

[[nodiscard]] float TireModel::slipRatioToWheelAngularVelocity(
    const float slip_ratio, const datatypes::datatypes::VehicleState& vehicle_state) const
{
    const auto wheel_velocities =
        wheelVelocities(vehicle_state.v_x_mps, vehicle_state.v_y_mps, vehicle_state.yaw_rate_radps);
    return slipRatioToWheelAngularVelocity(slip_ratio, wheel_velocities.x_mps);
}

[[nodiscard]] float TireModel::wheelLongOffset_m() const
{
    return wheel_axle_ == WheelAxle::Front ? DIST_FRONT_AXLE_CG_m : -DIST_REAR_AXLE_CG_m;
}

[[nodiscard]] float TireModel::wheelLatOffset_m() const
{
    return wheel_side_ == WheelSide::Left ? TRACK_WIDTH_m * 0.5f : -TRACK_WIDTH_m * 0.5f;
}

[[nodiscard]] float TireModel::wheelSteeringAngle_rad(const float steering_angle_rad) const
{
    return wheel_axle_ == WheelAxle::Front ? steering_angle_rad : 0.0f;
}

[[nodiscard]] TireModel::WheelVelocities TireModel::wheelVelocities(
    const float vehicle_velocity_x_mps, const float vehicle_velocity_y_mps, const float yaw_rate_radps) const
{
    // Rigid-body planar kinematics in the body frame:
    // v_wheel = v_cg + omega_z x r_wheel, where r_wheel = [x_offset, y_offset, 0].
    return {
        .x_mps = vehicle_velocity_x_mps - (yaw_rate_radps * wheelLatOffset_m()),
        .y_mps = vehicle_velocity_y_mps + (yaw_rate_radps * wheelLongOffset_m()),
    };
}

void TireModel::estimateSlipAngle(float steering_angle_rad)
{
    tire_outputs_.slip_angle_rad = std::atan2(wheel_vel_y_mps_, wheel_vel_x_mps_) - wheelSteeringAngle_rad(steering_angle_rad);
}

void TireModel::estimateSlipRatio(const float wheel_angular_velocity_radps)
{
    const float wheel_surface_speed_mps   = wheel_angular_velocity_radps * WHEEL_RADIUS_M;
    const float wheel_speed_magnitude_mps = std::hypot(wheel_vel_x_mps_, wheel_vel_y_mps_);
    const float effective_wheel_speed_mps = wheel_speed_magnitude_mps * std::cos(tire_outputs_.slip_angle_rad);

    tire_outputs_.slip_ratio =
        (wheel_surface_speed_mps - effective_wheel_speed_mps) / safeMagnitude(effective_wheel_speed_mps);
}

//-------------------------------------------------------------------- Pacejka 5.2 ----------------------------------------------------------------------//


void TireModel::pureFxMagicFormulaCoefficients(const float normal_load_N)
{
    // Assumes lambda terms are 1.0, gamma = 0, and pressure effects are captured by the discrete pressure row.
    const float clamped_normal_load_N = std::fmax(normal_load_N, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);
    const float kappa_x = pureFx_Kappa(normalized_load_delta, tire_outputs_.slip_ratio);
    const float c_x     = pureFx_C();
    const float d_x     = pureFx_D(clamped_normal_load_N, normalized_load_delta);
    const float e_x     = pureFx_E(normalized_load_delta, kappa_x);
    const float k_x     = pureFx_K(clamped_normal_load_N, normalized_load_delta);
    const float b_x     = pureFx_B(k_x, c_x, d_x);
    const float s_vx    = pureFx_Sv(clamped_normal_load_N, normalized_load_delta);

    pure_fx_mj_coefficients_ = {
        .kappa_x = kappa_x,
        .c_x     = c_x,
        .d_x     = d_x,
        .b_x     = b_x,
        .e_x     = e_x,
        .s_vx    = s_vx,
    };
}

void TireModel::pureFyMagicFormulaCoefficients(const float normal_load_N)
{
    // Assumes gamma = 0, lambda terms are 1.0, and pressure effects are captured by the discrete pressure row.
    const float clamped_normal_load_N = std::fmax(normal_load_N, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);
    const float alpha_y = pureFy_Alpha(normalized_load_delta, tire_outputs_.slip_angle_rad);
    const float c_y     = pureFy_C();
    const float d_y     = pureFy_D(clamped_normal_load_N, normalized_load_delta);
    const float e_y     = pureFy_E(normalized_load_delta, alpha_y);
    const float k_y     = pureFy_K(clamped_normal_load_N);
    const float b_y     = pureFy_B(k_y, c_y, d_y);
    const float s_vy    = pureFy_Sv(clamped_normal_load_N, normalized_load_delta);

    pure_fy_mj_coefficients_ = {
        .alpha_y = alpha_y,
        .c_y     = c_y,
        .d_y     = d_y,
        .b_y     = b_y,
        .e_y     = e_y,
        .s_vy    = s_vy,
    };
}

constexpr float TireModel::pure_Fy() const
{
    // Pacejka Page 180 (4.E19): F_y0
    const float b_y_alpha_y = pure_fy_mj_coefficients_.b_y * pure_fy_mj_coefficients_.alpha_y;

    return pure_fy_mj_coefficients_.d_y *
               std::sin(
                   pure_fy_mj_coefficients_.c_y *
                   std::atan(
                       b_y_alpha_y -
                       (pure_fy_mj_coefficients_.e_y *
                        (b_y_alpha_y - std::atan(b_y_alpha_y))))) +
           pure_fy_mj_coefficients_.s_vy;
}

void TireModel::computePureLongitudinalForce(const float normal_load_N)
{
    pureFxMagicFormulaCoefficients(normal_load_N);

    // Use forward-mode autodiff to compute Fx and ∂Fx/∂κ simultaneously.
    // All Pacejka coefficients are fixed at this operating point; kappa_x is the only variable.
    const auto& c = pure_fx_mj_coefficients_;
    autodiff::dual kappa_x = c.kappa_x;

    auto fx_formula = [&](autodiff::dual k) -> autodiff::dual {
        autodiff::dual u   = c.b_x * k;
        autodiff::dual phi = u - c.e_x * (u - atan(u));
        return c.d_x * sin(c.c_x * atan(phi)) + c.s_vx;
    };

    const autodiff::dual fx_result = fx_formula(kappa_x);
    const auto dFx_dKappa = autodiff::derivative(fx_formula, autodiff::wrt(kappa_x), autodiff::at(kappa_x));

    tire_outputs_.longitudinal_force_N = static_cast<float>(autodiff::val(fx_result));
    tire_outputs_.dFx_dKappa           = static_cast<float>(dFx_dKappa);
}

void TireModel::computePureLateralForce(const float normal_load_N)
{
    pureFyMagicFormulaCoefficients(normal_load_N);
    tire_outputs_.lateral_force_N = pure_Fy();
}


constexpr float TireModel::safeSignedDenominator(const float value)
{
    if (std::fabs(value) >= SMALL_EPSILON)
    {
        return value;
    }

    return value < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
}

constexpr float TireModel::signum(const float value)
{
    if (value > 0.0f)
    {
        return 1.0f;
    }

    if (value < 0.0f)
    {
        return -1.0f;
    }

    return 0.0f;
}

//-------------------------------------------------------------------- MJ Pure Coefficents 5.2 ----------------------------------------------------------------------//


constexpr float TireModel::normalizedLoadDelta(const float normal_load_N)
{
    return (normal_load_N - NOMINAL_FZ_N) / safeSignedDenominator(NOMINAL_FZ_N);
}

constexpr float TireModel::pureFx_Sh(const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E17): S_Hx
    return coeff_fx_.hx_1 + (coeff_fx_.hx_2 * normalized_load_delta);
}

constexpr float TireModel::pureFx_Kappa(const float normalized_load_delta, const float slip_ratio) const
{
    // Pacejka Page 179 (4.E10): kappa_x = kappa + S_Hx
    return slip_ratio + pureFx_Sh(normalized_load_delta);
}

constexpr float TireModel::pureFx_C() const
{
    // Pacejka Page 179 (4.E11): C_x
    return coeff_fx_.cx_1;
}

constexpr float TireModel::pureFx_mu(const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E13): mu_x with pressure/camber terms reduced to the current assumptions.
    return coeff_fx_.dx_1 + (coeff_fx_.dx_2 * normalized_load_delta);
}

constexpr float TireModel::pureFx_D(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E12): D_x = mu_x * F_z
    return pureFx_mu(normalized_load_delta) * normal_load_N;
}

constexpr float TireModel::pureFx_E(const float normalized_load_delta, const float kappa_x) const
{
    // Pacejka Page 179 (4.E14): E_x
    const float normalized_load_delta_squared = normalized_load_delta * normalized_load_delta;
    return (coeff_fx_.ex_1 + (coeff_fx_.ex_2 * normalized_load_delta) +
            (coeff_fx_.ex_3 * normalized_load_delta_squared)) *
           (1.0f - (coeff_fx_.ex_4 * signum(kappa_x)));
}

constexpr float TireModel::pureFx_K(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E15): K_xk
    return normal_load_N * (coeff_fx_.kx_1 + (coeff_fx_.kx_2 * normalized_load_delta)) *
           std::exp(coeff_fx_.kx_3 * normalized_load_delta);
}

constexpr float TireModel::pureFx_B(
    const float slip_stiffness, const float shape_factor, const float peak_factor)
{
    // Pacejka Page 179 (4.E16): B_x = K_xk / (C_x * D_x + epsilon_x)
    return slip_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFx_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E18): S_Vx
    return normal_load_N * (coeff_fx_.vx_1 + (coeff_fx_.vx_2 * normalized_load_delta));
}

constexpr float TireModel::pureFy_Sh(const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E27): S_Hy with gamma terms reduced to zero.
    return coeff_fy_.hy_1 + (coeff_fy_.hy_2 * normalized_load_delta);
}

constexpr float TireModel::pureFy_Alpha(const float normalized_load_delta, const float slip_angle_rad) const
{
    // Pacejka Page 180 (4.E20): alpha_y = alpha* + S_Hy
    return slip_angle_rad + pureFy_Sh(normalized_load_delta);
}

constexpr float TireModel::pureFy_C() const
{
    // Pacejka Page 180 (4.E21): C_y
    return coeff_fy_.cy_1;
}

constexpr float TireModel::pureFy_mu(const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E23): mu_y with pressure/camber terms reduced to the current assumptions.
    return coeff_fy_.dy_1 + (coeff_fy_.dy_2 * normalized_load_delta);
}

constexpr float TireModel::pureFy_D(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E22): D_y = mu_y * F_z
    return pureFy_mu(normalized_load_delta) * normal_load_N;
}

constexpr float TireModel::pureFy_E(const float normalized_load_delta, const float alpha_y) const
{
    // Pacejka Page 180 (4.E24): E_y with gamma terms reduced to zero.
    return (coeff_fy_.ey_1 + (coeff_fy_.ey_2 * normalized_load_delta)) *
           (1.0f - (coeff_fy_.ey_3 * signum(alpha_y)));
}

constexpr float TireModel::pureFy_K(const float normal_load_N) const
{
    // Pacejka Page 180 (4.E25): K_yalpha with gamma/pressure terms reduced to zero.
    const float denominator = safeSignedDenominator(coeff_fy_.ky_2 * NOMINAL_FZ_N);
    return coeff_fy_.ky_1 * NOMINAL_FZ_N *
           std::sin(2.0f * std::atan(normal_load_N / denominator));
}

constexpr float TireModel::pureFy_B(
    const float cornering_stiffness, const float shape_factor, const float peak_factor)
{
    // Pacejka Page 180 (4.E26): B_y = K_yalpha / (C_y * D_y + epsilon_y)
    return cornering_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFy_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E29): S_Vy with gamma terms reduced to zero.
    return normal_load_N * (coeff_fy_.vy_1 + (coeff_fy_.vy_2 * normalized_load_delta));
}

} // namespace app::tv::estimation
