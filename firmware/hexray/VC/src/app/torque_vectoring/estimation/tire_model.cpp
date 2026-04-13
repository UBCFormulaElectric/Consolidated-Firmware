#include "tire_model.hpp"

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "dual.hpp"
#include <cmath>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::estimation
{
// static namespace :)
namespace
{
    // [[nodiscard]] float safeMagnitude(const float value)
    // {
    //     return std::fmax(std::fabs(value), SMALL_EPSILON);
    // }
    [[nodiscard]] float safeTemplateDenominator(const float value)
    {
        if (std::fabs(value) >= SMALL_EPSILON)
        {
            return value;
        }

        return value < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    [[nodiscard]] autodiff::dual safeTemplateDenominator(const autodiff::dual &value)
    {
        const double primal = autodiff::val(value);
        if (std::fabs(static_cast<float>(primal)) >= SMALL_EPSILON)
            return value;
        return primal < 0.0 ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    float safeSignedDenominator(const float value)
    {
        if (std::fabs(value) >= SMALL_EPSILON)
            return value;
        return value < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    constexpr float sign(const float value)
    {
        if (value > 0.0f)
            return 1.0f;
        if (value < 0.0f)
            return -1.0f;
        return 0.0f;
    }
    autodiff::dual sign(const autodiff::dual &value)
    {
        if (value > 0.0f)
            return 1.0f;
        if (value < 0.0f)
            return -1.0f;
        return 0.0f;
    }
} // namespace

template <DecimalOrDual T>
[[nodiscard]] T TireModel::computeCombinedFx_N(const float fz_N, const float alpha_rad, const T &kappa) const
{
    const T    pure_fx_0    = computePureFx_N<T>(fz_N, kappa);
    const auto coefficients = combinedFxMagicFormulaCoefficients<T>(fz_N, alpha_rad, kappa);
    // Low-speed safeguard:
    // Below a small vehicle-speed threshold the tire model can predict unrealistically large
    // forces because the slip calculation becomes ill-conditioned while the fitted Pacejka
    // peak terms remain large. Blend the final combined-slip force output to zero so the model
    // fades out smoothly instead of producing unstable low-speed force demands.
    //
    // Pacejka Page 181 (4.E50): F_x = G_xa * F_x0
    return coefficients.g_xa * pure_fx_0;
}
template float TireModel::computeCombinedFx_N(
    const float  normal_load_N,
    const float  slip_angle_rad,
    const float &slip_ratio) const;
template autodiff::dual TireModel::computeCombinedFx_N<autodiff::dual>(
    float                 normal_load_N,
    float                 slip_angle_rad,
    const autodiff::dual &slip_ratio) const;

template <DecimalOrDual T>
[[nodiscard]] T TireModel::computeCombinedFy_N(const float fz_N, const float alpha_rad, const T &kappa) const
{
    const float pure_fy_0    = computePureFy_N(fz_N, alpha_rad);
    const auto  coefficients = combinedFyMagicFormulaCoefficients(fz_N, alpha_rad, kappa);

    // Apply the same low-speed force-availability blend used for Fx so the optimizer sees a
    // self-consistent pair of tire forces as the vehicle approaches a stop.
    //
    // Pacejka Pages 181-182 (4.E58): F_y = G_yk * F_y0 + S_vyk
    return coefficients.g_yk * T(pure_fy_0) + coefficients.s_vyk;
}
template float TireModel::computeCombinedFy_N<float>(
    const float  normal_load_N,
    const float  slip_angle_rad,
    const float &slip_ratio) const;
template autodiff::dual TireModel::computeCombinedFy_N<autodiff::dual>(
    float                 normal_load_N,
    float                 slip_angle_rad,
    const autodiff::dual &slip_ratio) const;

// [[nodiscard]] float TireModel::slipRatioToWheelAngularVelocity(const float slip_ratio, const float
// wheel_vel_x_mps)
// {
//     const float wheel_surface_speed_mps = wheel_vel_x_mps * (1.0f + slip_ratio);
//     return wheel_surface_speed_mps / safeMagnitude(WHEEL_RADIUS_M);
// }
//
// [[nodiscard]] float TireModel::slipRatioToWheelAngularVelocity(
//     const float                           slip_ratio,
//     const shared_datatypes::VehicleState &vehicle_state) const
// {
//     const auto [x_mps, y_mps] =
//         wheelVelocities(vehicle_state.v_x_mps, vehicle_state.v_y_mps, vehicle_state.yaw_rate_radps);
//     return slipRatioToWheelAngularVelocity(slip_ratio, x_mps);
// }
// [[nodiscard]] float TireModel::estimateSlipRatio(
//     const float wheel_vel_x_mps,
//     const float wheel_vel_y_mps,
//     const float slip_angle_rad,
//     const float wheel_angular_velocity_radps)
// {
//     const float wheel_surface_speed_mps   = wheel_angular_velocity_radps * WHEEL_RADIUS_M;
//     const float wheel_speed_magnitude_mps = std::hypot(wheel_vel_x_mps, wheel_vel_y_mps);
//     const float effective_wheel_speed_mps = wheel_speed_magnitude_mps * std::cos(slip_angle_rad);
//
//     return (wheel_surface_speed_mps - effective_wheel_speed_mps) / safeMagnitude(effective_wheel_speed_mps);
// }

//------------------------------------------ Pacejka 5.2 ----------------------------------//

template <DecimalOrDual T>
TireModel::PureFxMagicFormulaCoefficients<T>
    TireModel::pureFxMagicFormulaCoefficients(const float normal_load_N, const T &slip_ratio) const
{
    // Assumes lambda terms are 1.0, gamma = 0, and pressure effects are captured by the fixed 12_PSI fitted row.
    const float clamped_normal_load_N = std::fmax(normal_load_N, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);
    const float s_hx                  = pureFx_Sh(normalized_load_delta);
    const T     kappa_x               = pureFx_Kappa(normalized_load_delta, slip_ratio);
    const float c_x                   = pureFx_C();
    const float d_x                   = pureFx_D(clamped_normal_load_N, normalized_load_delta);
    const T     e_x                   = pureFx_E(normalized_load_delta, kappa_x);
    const float k_x                   = pureFx_K(clamped_normal_load_N, normalized_load_delta);
    const float b_x                   = pureFx_B(k_x, c_x, d_x);
    const float s_vx                  = pureFx_Sv(clamped_normal_load_N, normalized_load_delta);

    return {
        .s_hx    = T(s_hx),
        .kappa_x = kappa_x,
        .b_x     = T(b_x),
        .c_x     = T(c_x),
        .d_x     = T(d_x),
        .e_x     = e_x,
        .s_vx    = T(s_vx),
    };
}

TireModel::PureFyMagicFormulaCoefficients
    TireModel::pureFyMagicFormulaCoefficients(const float fz, const float alpha) const
{
    // Assumes gamma = 0, lambda terms are 1.0, and pressure effects are captured by the fixed 12_PSI fitted row.
    const float clamped_normal_load_N = std::fmax(fz, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);
    const float s_hy                  = pureFy_Sh(normalized_load_delta);
    const float alpha_y               = pureFy_Alpha(normalized_load_delta, alpha);
    const float c_y                   = pureFy_C();
    const float d_y                   = pureFy_D(clamped_normal_load_N, normalized_load_delta);
    const float e_y                   = pureFy_E(normalized_load_delta, alpha_y);
    const float k_y                   = pureFy_K(clamped_normal_load_N);
    const float b_y                   = pureFy_B(k_y, c_y, d_y);
    const float s_vy                  = pureFy_Sv(clamped_normal_load_N, normalized_load_delta);

    return {
        .s_hy    = s_hy,
        .alpha_y = alpha_y,
        .b_y     = b_y,
        .c_y     = c_y,
        .d_y     = d_y,
        .e_y     = e_y,
        .s_vy    = s_vy,
    };
}

template <DecimalOrDual T> T TireModel::computePureFx_N(const float fz, const T &kappa) const
{
    using std::sin, std::atan;

    const auto coefficients = pureFxMagicFormulaCoefficients<T>(fz, kappa);
    const T    u            = coefficients.b_x * coefficients.kappa_x;
    const T    phi          = u - coefficients.e_x * (u - atan(u));
    return coefficients.d_x * sin(coefficients.c_x * atan(phi)) + coefficients.s_vx;
}

float TireModel::computePureFy_N(const float fz_N, const float alpha) const
{
    const auto coefficients = pureFyMagicFormulaCoefficients(fz_N, alpha);

    // Pacejka Page 180 (4.E19): F_y0
    const float b_y_alpha_y = coefficients.b_y * coefficients.alpha_y;

    return coefficients.d_y * std::sin(
                                  coefficients.c_y *
                                  std::atan(b_y_alpha_y - coefficients.e_y * (b_y_alpha_y - std::atan(b_y_alpha_y)))) +
           coefficients.s_vy;
}

constexpr float TireModel::combinedFx_SHxa() const
{
    // Pacejka Page 181 (4.E57): S_Hxa
    return fit_comb_fx_.rHx1;
}

constexpr float TireModel::combinedFx_Alpha_s(const float alpha) const
{
    // Pacejka Page 181 (4.E53): alpha_s = alpha* + S_Hxa
    return alpha + combinedFx_SHxa();
}

constexpr float TireModel::combinedFx_Cxa() const
{
    // Pacejka Page 181 (4.E55): C_xa
    return fit_comb_fx_.rCx1;
}

float TireModel::combinedFx_Exa(const float normalized_load_delta) const
{
    // Pacejka Page 181 (4.E56): E_xa with reduced-model assumptions and E_xa <= 1.
    return std::fmin(fit_comb_fx_.rEx1 + fit_comb_fx_.rEx2 * normalized_load_delta, 1.0f);
}

template <DecimalOrDual T> T TireModel::combinedFx_Bxa(const T &kappa) const
{
    using std::atan;
    using std::cos;

    // Pacejka Page 181 (4.E54): B_xa with gamma* = 0 and lambda_xa = 1.
    return T(fit_comb_fx_.rBx1) * cos(atan(T(fit_comb_fx_.rBx2) * kappa));
}

template <DecimalOrDual T> T TireModel::combinedFx_Gxao(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const
{
    using std::atan;
    using std::cos;

    // Pacejka Page 181 (4.E52): G_xao
    const T u   = coefficients.b_xa * coefficients.s_hxa;
    const T phi = u - coefficients.e_xa * (u - atan(u));
    return cos(coefficients.c_xa * atan(phi));
}

template <DecimalOrDual T> T TireModel::combinedFx_Gxa(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const
{
    using std::atan;
    using std::cos;

    // Pacejka Page 181 (4.E51): G_xa with G_xao > 0 protected by a signed epsilon denominator.
    const T u         = coefficients.b_xa * coefficients.alpha_s;
    const T phi       = u - coefficients.e_xa * (u - atan(u));
    const T numerator = cos(coefficients.c_xa * atan(phi));

    return numerator / safeTemplateDenominator(coefficients.g_xao);
}

template <DecimalOrDual T>
TireModel::CombinedFxMagicFormulaCoefficients<T> TireModel::combinedFxMagicFormulaCoefficients(
    const float normal_load_N,
    const float slip_angle_rad,
    const T    &slip_ratio) const
{
    const float clamped_normal_load_N = std::fmax(normal_load_N, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);

    CombinedFxMagicFormulaCoefficients<T> coefficients{
        .s_hxa   = T(combinedFx_SHxa()),
        .alpha_s = T(combinedFx_Alpha_s(slip_angle_rad)),
        .b_xa    = combinedFx_Bxa(slip_ratio),
        .c_xa    = T(combinedFx_Cxa()),
        .e_xa    = T(combinedFx_Exa(normalized_load_delta)),
        .g_xao   = T(0.0f),
        .g_xa    = T(0.0f),
    };

    coefficients.g_xao = combinedFx_Gxao(coefficients);
    coefficients.g_xa  = combinedFx_Gxa(coefficients);
    return coefficients;
}

float TireModel::combinedFy_SHyk(const float normalized_load_delta) const
{
    // Pacejka Page 182 (4.E65): S_Hyk
    return fit_comb_fy_.rHy1 + fit_comb_fy_.rHy2 * normalized_load_delta;
}

template <DecimalOrDual T> T TireModel::combinedFy_Kappa_s(const float normalized_load_delta, const T &slip_ratio) const
{
    // Pacejka Page 181 (4.E61): kappa_s = kappa + S_Hyk
    return slip_ratio + T(combinedFy_SHyk(normalized_load_delta));
}

constexpr float TireModel::combinedFy_Cyk() const
{
    // Pacejka Page 182 (4.E63): C_yk
    return fit_comb_fy_.rCy1;
}

float TireModel::combinedFy_Eyk(const float normalized_load_delta) const
{
    // Pacejka Page 182 (4.E64): E_yk with reduced-model assumptions and E_yk <= 1.
    return std::fmin(fit_comb_fy_.rEy1 + fit_comb_fy_.rEy2 * normalized_load_delta, 1.0f);
}

float TireModel::combinedFy_Byk(const float slip_angle_rad) const
{
    // Pacejka Page 182 (4.E62): B_yk with gamma* = 0 and lambda_yk = 1.
    return fit_comb_fy_.rBy1 * std::cos(std::atan(fit_comb_fy_.rBy2 * (slip_angle_rad - fit_comb_fy_.rBy3)));
}

float TireModel::combinedFy_Dvyk(
    const float normal_load_N,
    const float normalized_load_delta,
    const float slip_angle_rad) const
{
    // Pacejka Page 182 (4.E67): D_vyk with gamma* = 0 and zeta_2 = 1.
    return pureFy_mu(normalized_load_delta) * normal_load_N *
           (fit_comb_fy_.rVy1 + fit_comb_fy_.rVy2 * normalized_load_delta) *
           std::cos(std::atan(fit_comb_fy_.rVy4 * slip_angle_rad));
}

template <DecimalOrDual T> T TireModel::combinedFy_Svyk(const T &d_vyk, const T &slip_ratio) const
{
    using std::sin, std::atan;
    // Pacejka Page 182 (4.E66): S_vyk with lambda_vyk = 1.
    return d_vyk * sin(T(fit_comb_fy_.rVy5) * atan(T(fit_comb_fy_.rVy6) * slip_ratio));
}

template <DecimalOrDual T> T TireModel::combinedFy_Gyko(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const
{
    using std::cos, std::atan;
    // Pacejka Page 181 (4.E60): G_yko
    const T u   = coefficients.b_yk * coefficients.s_hyk;
    const T phi = u - coefficients.e_yk * (u - atan(u));
    return cos(coefficients.c_yk * atan(phi));
}

template <DecimalOrDual T> T TireModel::combinedFy_Gyk(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const
{
    using std::cos, std::atan;

    // Pacejka Page 181 (4.E59): G_yk with G_yko > 0 protected by a signed epsilon denominator.
    const T u         = coefficients.b_yk * coefficients.kappa_s;
    const T phi       = u - coefficients.e_yk * (u - atan(u));
    const T numerator = cos(coefficients.c_yk * atan(phi));

    return numerator / safeTemplateDenominator(coefficients.g_yko);
}

template <DecimalOrDual T>
TireModel::CombinedFyMagicFormulaCoefficients<T> TireModel::combinedFyMagicFormulaCoefficients(
    const float normal_load_N,
    const float slip_angle_rad,
    const T    &slip_ratio) const
{
    const float clamped_normal_load_N = std::fmax(normal_load_N, 0.0f);
    const float normalized_load_delta = normalizedLoadDelta(clamped_normal_load_N);

    CombinedFyMagicFormulaCoefficients<T> coefficients{
        .s_hyk   = T(combinedFy_SHyk(normalized_load_delta)),
        .kappa_s = combinedFy_Kappa_s(normalized_load_delta, slip_ratio),
        .b_yk    = T(combinedFy_Byk(slip_angle_rad)),
        .c_yk    = T(combinedFy_Cyk()),
        .e_yk    = T(combinedFy_Eyk(normalized_load_delta)),
        .d_vyk   = T(combinedFy_Dvyk(clamped_normal_load_N, normalized_load_delta, slip_angle_rad)),
        .s_vyk   = T(0.0f),
        .g_yko   = T(0.0f),
        .g_yk    = T(0.0f),
    };

    coefficients.s_vyk = combinedFy_Svyk(coefficients.d_vyk, slip_ratio);
    coefficients.g_yko = combinedFy_Gyko(coefficients);
    coefficients.g_yk  = combinedFy_Gyk(coefficients);
    return coefficients;
}

//-------------------------------------------------------------------- MJ Pure Coefficents 5.2
//----------------------------------------------------------------------//

float TireModel::normalizedLoadDelta(const float normal_load_N)
{
    return (normal_load_N - NOMINAL_FZ_N) / safeSignedDenominator(NOMINAL_FZ_N);
}

constexpr float TireModel::pureFx_Sh(const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E17): S_Hx
    return fit_pure_fx_.hx_1 + fit_pure_fx_.hx_2 * normalized_load_delta;
}

template <DecimalOrDual T> T TireModel::pureFx_Kappa(const float normalized_load_delta, const T &slip_ratio) const
{
    // Pacejka Page 179 (4.E10): kappa_x = kappa + S_Hx
    return slip_ratio + T(pureFx_Sh(normalized_load_delta));
}

constexpr float TireModel::pureFx_C() const
{
    // Pacejka Page 179 (4.E11): C_x
    return fit_pure_fx_.cx_1;
}

constexpr float TireModel::pureFx_mu(const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E13): mu_x with pressure/camber terms reduced to the current assumptions.
    return fit_pure_fx_.dx_1 + fit_pure_fx_.dx_2 * normalized_load_delta;
}

constexpr float TireModel::pureFx_D(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E12): D_x = mu_x * F_z
    return pureFx_mu(normalized_load_delta) * normal_load_N;
}

template <DecimalOrDual T> T TireModel::pureFx_E(const float normalized_load_delta, const T &kappa_x) const
{
    // Pacejka Page 179 (4.E14): E_x
    const float normalized_load_delta_squared = normalized_load_delta * normalized_load_delta;
    const float base_e_x                      = fit_pure_fx_.ex_1 + fit_pure_fx_.ex_2 * normalized_load_delta +
                           fit_pure_fx_.ex_3 * normalized_load_delta_squared;
    return base_e_x * (1.0f - fit_pure_fx_.ex_4 * sign(kappa_x));
}

float TireModel::pureFx_K(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E15): K_xk
    return normal_load_N * (fit_pure_fx_.kx_1 + fit_pure_fx_.kx_2 * normalized_load_delta) *
           std::exp(fit_pure_fx_.kx_3 * normalized_load_delta);
}

float TireModel::pureFx_B(const float slip_stiffness, const float shape_factor, const float peak_factor)
{
    // Pacejka Page 179 (4.E16): B_x = K_xk / (C_x * D_x + epsilon_x)
    return slip_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFx_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 179 (4.E18): S_Vx
    return normal_load_N * (fit_pure_fx_.vx_1 + fit_pure_fx_.vx_2 * normalized_load_delta);
}

constexpr float TireModel::pureFy_Sh(const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E27): S_Hy with gamma terms reduced to zero.
    return fit_pure_fy_.hy_1 + fit_pure_fy_.hy_2 * normalized_load_delta;
}

constexpr float TireModel::pureFy_Alpha(const float normalized_load_delta, const float slip_angle_rad) const
{
    // Pacejka Page 180 (4.E20): alpha_y = alpha* + S_Hy
    return slip_angle_rad + pureFy_Sh(normalized_load_delta);
}

constexpr float TireModel::pureFy_C() const
{
    // Pacejka Page 180 (4.E21): C_y
    return fit_pure_fy_.cy_1;
}

constexpr float TireModel::pureFy_mu(const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E23): mu_y with pressure/camber terms reduced to the current assumptions.
    return fit_pure_fy_.dy_1 + fit_pure_fy_.dy_2 * normalized_load_delta;
}

constexpr float TireModel::pureFy_D(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E22): D_y = mu_y * F_z
    return pureFy_mu(normalized_load_delta) * normal_load_N;
}

constexpr float TireModel::pureFy_E(const float normalized_load_delta, const float alpha_y) const
{
    // Pacejka Page 180 (4.E24): E_y with gamma terms reduced to zero.
    return (fit_pure_fy_.ey_1 + fit_pure_fy_.ey_2 * normalized_load_delta) * (1.0f - fit_pure_fy_.ey_3 * sign(alpha_y));
}

float TireModel::pureFy_K(const float normal_load_N) const
{
    // Pacejka Page 180 (4.E25): K_yalpha with gamma/pressure terms reduced to zero.
    const float denominator = safeSignedDenominator(fit_pure_fy_.ky_2 * NOMINAL_FZ_N);
    return fit_pure_fy_.ky_1 * NOMINAL_FZ_N * std::sin(2.0f * std::atan(normal_load_N / denominator));
}

float TireModel::pureFy_B(const float cornering_stiffness, const float shape_factor, const float peak_factor)
{
    // Pacejka Page 180 (4.E26): B_y = K_yalpha / (C_y * D_y + epsilon_y)
    return cornering_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFy_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // Pacejka Page 180 (4.E29): S_Vy with gamma terms reduced to zero.
    return normal_load_N * (fit_pure_fy_.vy_1 + fit_pure_fy_.vy_2 * normalized_load_delta);
}
} // namespace app::tv::estimation
