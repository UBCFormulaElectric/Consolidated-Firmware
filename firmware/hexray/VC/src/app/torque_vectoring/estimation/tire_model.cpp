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
    [[nodiscard]] float safeTemplateDenominator(const float value)
    {
        if (std::fabs(value) >= SMALL_EPSILON)
        {
            return value;
        }

        return value < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    [[nodiscard]] double safeTemplateDenominator(const double value)
    {
        const double small_epsilon = static_cast<double>(SMALL_EPSILON);

        if (std::fabs(value) >= small_epsilon)
        {
            return value;
        }

        return value < 0.0 ? -small_epsilon : small_epsilon;
    }
    [[nodiscard]] autodiff::dual safeTemplateDenominator(const autodiff::dual &value)
    {
        const double primal = autodiff::val(value);
        if (std::fabs(static_cast<float>(primal)) >= SMALL_EPSILON)
            return value;
        return primal < 0.0 ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    [[nodiscard]] float safeSignedDenominator(const float value)
    {
        if (std::fabs(value) >= SMALL_EPSILON)
            return value;
        return value < 0.0f ? -SMALL_EPSILON : SMALL_EPSILON;
    }
    [[nodiscard]] constexpr float sign(const float value)
    {
        if (value > 0.0f)
            return 1.0f;
        if (value < 0.0f)
            return -1.0f;
        return 0.0f;
    }
    [[nodiscard]] constexpr double sign(const double value)
    {
        if (value > 0.0)
            return 1.0;
        if (value < 0.0)
            return -1.0;
        return 0.0;
    }
    [[nodiscard]] autodiff::dual sign(const autodiff::dual &value)
    {
        if (value > 0.0f)
            return 1.0f;
        if (value < 0.0f)
            return -1.0f;
        return 0.0f;
    }
    template <DecimalOrDual T> [[nodiscard]] T clampLessEqualOne(const T &value)
    {
        using std::abs;

        return T(0.5f) * (value + T(1.0f) - abs(value - T(1.0f)));
    }
} // namespace

template <DecimalOrDual T>
[[nodiscard]] T TireModel::computeCombinedFx_N(const float fz_N, const float alpha_rad, const T &kappa) const
{
    const T    pure_fx_0    = computePureFx_N<T>(fz_N, kappa);
    const auto coefficients = combinedFxMagicFormulaCoefficients<T>(fz_N, alpha_rad, kappa);

    // Combined-slip longitudinal force: F_x = G_xa * F_x0.
    return coefficients.g_xa * pure_fx_0;
}
template float TireModel::computeCombinedFx_N(float normal_load_N, float slip_angle_rad, const float &slip_ratio) const;
template double
    TireModel::computeCombinedFx_N(float normal_load_N, float slip_angle_rad, const double &slip_ratio) const;
template autodiff::dual TireModel::computeCombinedFx_N<autodiff::dual>(
    float                 normal_load_N,
    float                 slip_angle_rad,
    const autodiff::dual &slip_ratio) const;

template <DecimalOrDual T>
[[nodiscard]] T TireModel::computeCombinedFy_N(const float fz_N, const float alpha_rad, const T &kappa) const
{
    const float pure_fy_0    = computePureFy_N(fz_N, alpha_rad);
    const auto  coefficients = combinedFyMagicFormulaCoefficients(fz_N, alpha_rad, kappa);

    // Combined-slip lateral force: F_y = G_yk * F_y0 + S_vyk.
    return coefficients.g_yk * T(pure_fy_0) + coefficients.s_vyk;
}
template float
    TireModel::computeCombinedFy_N<float>(float normal_load_N, float slip_angle_rad, const float &slip_ratio) const;
template double
    TireModel::computeCombinedFy_N<double>(float normal_load_N, float slip_angle_rad, const double &slip_ratio) const;
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

//------------------------------------------ Pacejka MF 6.2 ----------------------------------//

template <DecimalOrDual T>
TireModel::PureFxMagicFormulaCoefficients<T>
    TireModel::pureFxMagicFormulaCoefficients(const float normal_load_N, const T &slip_ratio) const
{
    // Reduced MF 6.2 assumptions: gamma = 0, dpi = 0, and zeta_0..zeta_4 = 1.
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
    // Reduced MF 6.2 assumptions: gamma = 0, dpi = 0, and the K_ygamma / S_Hyy / S_Vyy branch collapses to zero.
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

    // Pure-slip lateral force: F_y0.
    const float b_y_alpha_y = coefficients.b_y * coefficients.alpha_y;

    return coefficients.d_y * std::sin(
                                  coefficients.c_y *
                                  std::atan(b_y_alpha_y - coefficients.e_y * (b_y_alpha_y - std::atan(b_y_alpha_y)))) +
           coefficients.s_vy;
}

constexpr float TireModel::combinedFx_SHxa() const
{
    // MF 6.2 combined-slip horizontal shift S_Hxalpha.
    return fit_comb_fx_.rHx1;
}

constexpr float TireModel::combinedFx_Alpha_s(const float alpha) const
{
    // The torque-vectoring stack provides slip angle with the opposite sign to the ISO-W/Pacejka convention.
    // Convert once at the tire-model boundary so the public API stays unchanged and Fy is not mirrored.
    return pacejkaSlipAngle(alpha) + combinedFx_SHxa();
}

constexpr float TireModel::combinedFx_Cxa() const
{
    // MF 6.2 combined-slip shape factor C_xalpha.
    return fit_comb_fx_.rCx1;
}

float TireModel::combinedFx_Exa(const float normalized_load_delta) const
{
    // MF 6.2 combined-slip curvature E_xalpha with the reduced gamma = 0 form and E <= 1 clamp.
    return std::fmin(fit_comb_fx_.rEx1 + fit_comb_fx_.rEx2 * normalized_load_delta, 1.0f);
}

template <DecimalOrDual T> T TireModel::combinedFx_Bxa(const T &kappa) const
{
    using std::atan;
    using std::cos;

    // MF 6.2 combined-slip stiffness B_xalpha with gamma = 0.
    return T(fit_comb_fx_.rBx1) * cos(atan(T(fit_comb_fx_.rBx2) * kappa)) * T(scaling_factors_.LXAL);
}

template <DecimalOrDual T> T TireModel::combinedFx_Gxao(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const
{
    using std::atan;
    using std::cos;

    // MF 6.2 reference combined-slip reduction G_xalpha0.
    const T u   = coefficients.b_xa * coefficients.s_hxa;
    const T phi = u - coefficients.e_xa * (u - atan(u));
    return cos(coefficients.c_xa * atan(phi));
}

template <DecimalOrDual T> T TireModel::combinedFx_Gxa(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const
{
    using std::atan;
    using std::cos;

    // MF 6.2 combined-slip reduction G_xalpha with G_xalpha0 protected by a signed epsilon denominator.
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
    // MF 6.2 combined-slip horizontal shift S_Hykappa.
    return fit_comb_fy_.rHy1 + fit_comb_fy_.rHy2 * normalized_load_delta;
}

template <DecimalOrDual T> T TireModel::combinedFy_Kappa_s(const float normalized_load_delta, const T &slip_ratio) const
{
    // MF 6.2 shifted slip ratio kappa_s = kappa + S_Hykappa.
    return slip_ratio + T(combinedFy_SHyk(normalized_load_delta));
}

constexpr float TireModel::combinedFy_Cyk() const
{
    // MF 6.2 combined-slip shape factor C_ykappa.
    return fit_comb_fy_.rCy1;
}

float TireModel::combinedFy_Eyk(const float normalized_load_delta) const
{
    // MF 6.2 combined-slip curvature E_ykappa with the reduced gamma = 0 form and E <= 1 clamp.
    return std::fmin(fit_comb_fy_.rEy1 + fit_comb_fy_.rEy2 * normalized_load_delta, 1.0f);
}

float TireModel::combinedFy_Byk(const float slip_angle_rad) const
{
    // MF 6.2 combined-slip stiffness B_ykappa with gamma = 0.
    const float pacejka_alpha_rad = pacejkaSlipAngle(slip_angle_rad);

    return fit_comb_fy_.rBy1 * std::cos(std::atan(fit_comb_fy_.rBy2 * (pacejka_alpha_rad - fit_comb_fy_.rBy3))) *
           scaling_factors_.LYKA;
}

float TireModel::combinedFy_Dvyk(
    const float normal_load_N,
    const float normalized_load_delta,
    const float slip_angle_rad) const
{
    // MF 6.2 combined-slip lateral offset amplitude D_vyk with gamma = 0 and zeta_2 = 1.
    const float pacejka_alpha_rad = pacejkaSlipAngle(slip_angle_rad);

    return pureFy_mu(normalized_load_delta) * normal_load_N *
           (fit_comb_fy_.rVy1 + fit_comb_fy_.rVy2 * normalized_load_delta) *
           std::cos(std::atan(fit_comb_fy_.rVy4 * pacejka_alpha_rad));
}

template <DecimalOrDual T> T TireModel::combinedFy_Svyk(const T &d_vyk, const T &slip_ratio) const
{
    using std::sin, std::atan;
    // MF 6.2 combined-slip lateral offset S_vykappa.
    return d_vyk * sin(T(fit_comb_fy_.rVy5) * atan(T(fit_comb_fy_.rVy6) * slip_ratio)) * T(scaling_factors_.LVYKA);
}

template <DecimalOrDual T> T TireModel::combinedFy_Gyko(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const
{
    using std::cos, std::atan;

    // MF 6.2 reference combined-slip reduction G_ykappa0.
    const T u   = coefficients.b_yk * coefficients.s_hyk;
    const T phi = u - coefficients.e_yk * (u - atan(u));
    return cos(coefficients.c_yk * atan(phi));
}

template <DecimalOrDual T> T TireModel::combinedFy_Gyk(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const
{
    using std::cos, std::atan;

    // MF 6.2 combined-slip reduction G_ykappa with G_ykappa0 protected by a signed epsilon denominator.
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

//-------------------------------------------------------------------- MF 6.2 Pure Coefficients
//----------------------------------------------------------------------//

float TireModel::referenceNormalLoad_N() const
{
    return FZ0 * scaling_factors_.LFZ0;
}

constexpr float TireModel::pacejkaSlipAngle(const float slip_angle_rad)
{
    return -slip_angle_rad;
}

float TireModel::normalizedLoadDelta(const float normal_load_N) const
{
    const float reference_normal_load_N = referenceNormalLoad_N();
    return (normal_load_N - reference_normal_load_N) / safeSignedDenominator(reference_normal_load_N);
}

constexpr float TireModel::pureFx_Sh(const float normalized_load_delta) const
{
    // MF 6.2 pure-slip horizontal shift S_Hx.
    return (fit_pure_fx_.p_Hx1 + fit_pure_fx_.p_Hx2 * normalized_load_delta) * scaling_factors_.LHX;
}

template <DecimalOrDual T> T TireModel::pureFx_Kappa(const float normalized_load_delta, const T &slip_ratio) const
{
    // MF 6.2 shifted slip ratio kappa_x = kappa + S_Hx.
    return slip_ratio + T(pureFx_Sh(normalized_load_delta));
}

constexpr float TireModel::pureFx_C() const
{
    // MF 6.2 pure-slip shape factor C_x.
    return fit_pure_fx_.p_Cx1 * scaling_factors_.LCX;
}

constexpr float TireModel::pureFx_mu(const float normalized_load_delta) const
{
    // MF 6.2 pure-slip friction mu_x with gamma = 0 and dpi = 0, so p_Dx3 is inactive here.
    return (fit_pure_fx_.p_Dx1 + fit_pure_fx_.p_Dx2 * normalized_load_delta) * scaling_factors_.LMUX;
}

constexpr float TireModel::pureFx_D(const float normal_load_N, const float normalized_load_delta) const
{
    // MF 6.2 pure-slip peak factor D_x = mu_x * F_z.
    return pureFx_mu(normalized_load_delta) * normal_load_N;
}

template <DecimalOrDual T> T TireModel::pureFx_E(const float normalized_load_delta, const T &kappa_x) const
{
    // MF 6.2 pure-slip curvature E_x with the reduced gamma = 0, dpi = 0 form and E <= 1 clamp.
    const float normalized_load_delta_squared = normalized_load_delta * normalized_load_delta;
    const T     e_x                           = (T(fit_pure_fx_.p_Ex1 + fit_pure_fx_.p_Ex2 * normalized_load_delta +
                                                   fit_pure_fx_.p_Ex3 * normalized_load_delta_squared) *
                   (T(1.0f) - T(fit_pure_fx_.p_Ex4) * sign(kappa_x))) *
                  T(scaling_factors_.LEX);

    return clampLessEqualOne(e_x);
}

float TireModel::pureFx_K(const float normal_load_N, const float normalized_load_delta) const
{
    // MF 6.2 pure-slip longitudinal stiffness K_xkappa with dpi = 0.
    return normal_load_N * (fit_pure_fx_.p_Kx1 + fit_pure_fx_.p_Kx2 * normalized_load_delta) *
           std::exp(fit_pure_fx_.p_Kx3 * normalized_load_delta) * scaling_factors_.LKX;
}

float TireModel::pureFx_B(const float slip_stiffness, const float shape_factor, const float peak_factor)
{
    // MF 6.2 pure-slip stiffness factor B_x = K_xkappa / (C_x * D_x + epsilon_x).
    return slip_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFx_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // MF 6.2 pure-slip vertical shift S_Vx. With dpi = 0, p_Vx pressure terms remain inactive.
    return normal_load_N * (fit_pure_fx_.p_Vx1 + fit_pure_fx_.p_Vx2 * normalized_load_delta) * scaling_factors_.LVX *
           scaling_factors_.LMUX;
}

constexpr float TireModel::pureFy_Sh(const float normalized_load_delta) const
{
    // MF 6.2 pure-slip horizontal shift S_Hy. With gamma = 0, S_Hyy collapses to zero so only S_Hy0 remains.
    return (fit_pure_fy_.p_Hy1 + fit_pure_fy_.p_Hy2 * normalized_load_delta) * scaling_factors_.LHY;
}

constexpr float TireModel::pureFy_Alpha(const float normalized_load_delta, const float slip_angle_rad) const
{
    // Convert the external slip-angle convention into Pacejka's ISO-W sign before evaluating Fy.
    return pacejkaSlipAngle(slip_angle_rad) + pureFy_Sh(normalized_load_delta);
}

constexpr float TireModel::pureFy_C() const
{
    // MF 6.2 pure-slip shape factor C_y.
    return fit_pure_fy_.p_Cy1 * scaling_factors_.LCY;
}

constexpr float TireModel::pureFy_mu(const float normalized_load_delta) const
{
    // MF 6.2 pure-slip friction mu_y with gamma = 0 and dpi = 0, so p_Dy3 is inactive here.
    return (fit_pure_fy_.p_Dy1 + fit_pure_fy_.p_Dy2 * normalized_load_delta) * scaling_factors_.LMUY;
}

constexpr float TireModel::pureFy_D(const float normal_load_N, const float normalized_load_delta) const
{
    // MF 6.2 pure-slip peak factor D_y = mu_y * F_z.
    return pureFy_mu(normalized_load_delta) * normal_load_N;
}

float TireModel::pureFy_E(const float normalized_load_delta, const float alpha_y) const
{
    // MF 6.2 pure-slip curvature E_y with gamma = 0 and E <= 1 clamp.
    const float e_y = (fit_pure_fy_.p_Ey1 + fit_pure_fy_.p_Ey2 * normalized_load_delta) *
                      (1.0f - fit_pure_fy_.p_Ey3 * sign(alpha_y)) * scaling_factors_.LEY;

    return std::fmin(e_y, 1.0f);
}

float TireModel::pureFy_K(const float normal_load_N) const
{
    using std::atan;
    using std::sin;

    // MF 6.2 pure-slip cornering stiffness K_yalpha. gamma- and pressure-only terms are inactive at gamma = 0, dpi = 0.
    const float reference_normal_load_N = referenceNormalLoad_N();
    const float denominator             = safeSignedDenominator(fit_pure_fy_.p_Ky2 * reference_normal_load_N);

    return fit_pure_fy_.p_Ky1 * reference_normal_load_N * sin(fit_pure_fy_.p_Ky4 * atan(normal_load_N / denominator)) *
           scaling_factors_.LKY;
}

float TireModel::pureFy_B(const float cornering_stiffness, const float shape_factor, const float peak_factor)
{
    // MF 6.2 pure-slip stiffness factor B_y = K_yalpha / (C_y * D_y + epsilon_y).
    return cornering_stiffness / safeSignedDenominator(shape_factor * peak_factor);
}

constexpr float TireModel::pureFy_Sv(const float normal_load_N, const float normalized_load_delta) const
{
    // MF 6.2 pure-slip vertical shift S_Vy. With gamma = 0, S_Vyy collapses to zero so only S_Vy0 remains.
    return normal_load_N * (fit_pure_fy_.p_Vy1 + fit_pure_fy_.p_Vy2 * normalized_load_delta) * scaling_factors_.LVY *
           scaling_factors_.LMUY;
}
} // namespace app::tv::estimation
