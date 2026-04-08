#pragma once

#include <cmath>

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"
#include "torque_vectoring/shared_datatypes/low_speed_blend.hpp"

namespace app::tv::estimation
{
class TireModel
{
public:
    enum class WheelSide
    {
        Left,
        Right
    };

    enum class WheelAxle
    {
        Front,
        Rear
    };

    struct StateInputs
    {
        float wheel_angular_velocity_radps = 0.0f;
        float vehicle_velocity_x_mps       = 0.0f;
        float vehicle_velocity_y_mps       = 0.0f;
        float yaw_rate_radps               = 0.0f;
        float steering_angle_rad           = 0.0f;
        float normal_load_N                = 0.0f;
    };

    struct Outputs
    {
        float slip_ratio           = 0.0f;
        float slip_angle_rad       = 0.0f;
        float longitudinal_force_N = 0.0f;
        float lateral_force_N      = 0.0f;
    };

    constexpr TireModel(const WheelSide wheel_side, const WheelAxle wheel_axle)
        : fit_pure_fx_(FIT_PURE_FX_12_PSI),
          fit_pure_fy_(FIT_PURE_FY_12_PSI),
          fit_comb_fx_(FIT_COMB_FX_12_PSI),
          fit_comb_fy_(FIT_COMB_FY_12_PSI),
          wheel_side_(wheel_side),
          wheel_axle_(wheel_axle)
    {
    }

    [[nodiscard]] Outputs estimate(const StateInputs& inputs) const;
    template <typename T>
    [[nodiscard]] T computeCombinedFx_N(
        float normal_load_N, float slip_angle_rad, float low_speed_blend, const T& slip_ratio) const;
    template <typename T>
    [[nodiscard]] T computeCombinedFy_N(
        float normal_load_N, float slip_angle_rad, float low_speed_blend, const T& slip_ratio) const;
    [[nodiscard]] float slipRatioToWheelAngularVelocity(const float slip_ratio, const float wheel_vel_x_mps) const;
    [[nodiscard]] float slipRatioToWheelAngularVelocity(
        const float slip_ratio, const datatypes::datatypes::VehicleState& vehicle_state) const;

private:
    struct WheelVelocities
    {
        float x_mps = 0.0f;
        float y_mps = 0.0f;
    };

    struct TireFitPureParamFy
    {
        float cy_1;
        float dy_1;
        float dy_2;
        float dy_3;
        float ey_1;
        float ey_2;
        float ey_3;
        float ey_4;
        float ky_1;
        float ky_2;
        float ky_3;
        float hy_1;
        float hy_2;
        float hy_3;
        float vy_1;
        float vy_2;
        float vy_3;
        float vy_4;
    };

    struct TireFitPureParamFx
    {
        float cx_1;
        float dx_1;
        float dx_2;
        float dx_3;
        float ex_1;
        float ex_2;
        float ex_3;
        float ex_4;
        float kx_1;
        float kx_2;
        float kx_3;
        float hx_1;
        float hx_2;
        float vx_1;
        float vx_2;
    };

    struct TireFitCombParamFx
    {
        float rBx1;
        float rBx2;
        float rBx3;
        float rCx1;
        float rEx1;
        float rEx2;
        float rHx1;
    };

    struct TireFitCombParamFy
    {
        float rBy1;
        float rBy2;
        float rBy3;
        float rBy4;
        float rCy1;
        float rEy1;
        float rEy2;
        float rHy1;
        float rHy2;
        float rVy1;
        float rVy2;
        float rVy3;
        float rVy4;
        float rVy5;
        float rVy6;
    };

    template <typename T>
    struct PureFxMagicFormulaCoefficients
    {
        T s_hx    = T(0.0f);
        T kappa_x = T(0.0f);
        T b_x     = T(0.0f);
        T c_x     = T(0.0f);
        T d_x     = T(0.0f);
        T e_x     = T(0.0f);
        T s_vx    = T(0.0f);
    };

    template <typename T>
    struct CombinedFxMagicFormulaCoefficients
    {
        T s_hxa   = T(0.0f);
        T alpha_s = T(0.0f);
        T b_xa    = T(0.0f);
        T c_xa    = T(0.0f);
        T e_xa    = T(0.0f);
        T g_xao   = T(0.0f); // letter o, not zero
        T g_xa    = T(0.0f);
    };

    struct PureFyMagicFormulaCoefficients
    {
        float s_hy    = 0.0f;
        float alpha_y = 0.0f;
        float b_y     = 0.0f;
        float c_y     = 0.0f;
        float d_y     = 0.0f;
        float e_y     = 0.0f;
        float s_vy    = 0.0f;
    };

    template <typename T>
    struct CombinedFyMagicFormulaCoefficients
    {
        T s_hyk   = T(0.0f);
        T kappa_s = T(0.0f);
        T b_yk    = T(0.0f);
        T c_yk    = T(0.0f);
        T e_yk    = T(0.0f);
        T d_vyk   = T(0.0f);
        T s_vyk   = T(0.0f);
        T g_yko   = T(0.0f);
        T g_yk    = T(0.0f);
    };


    /*
    Current model uses the 12_PSI fitted workbook row as a fixed-pressure operating assumption.
    If pressure becomes a runtime input later, add pressure interpolation or a refit against the raw tire data.
    */
    static constexpr TireFitPureParamFx FIT_PURE_FX_12_PSI = {
        0.008656294312f,
        366.1925838f,
        -57.2826791f,
        8.208916842f,
        1.134744942f,
        0.04728092792f,
        -0.02191230313f,
        -0.01799018414f,
        62.46585932f,
        1.390484252f,
        -0.3909753489f,
        0.0005429909716f,
        -0.0007019708842f,
        0.01286770906f,
        0.004135955934f,
    };

    static constexpr TireFitPureParamFy FIT_PURE_FY_12_PSI = {
        0.90721342f,
        -3.824114982f,
        -0.4280518359f,
        15.56383821f,
        1.218066219f,
        0.3097318297f,
        -0.02930740966f,
        0.2068388689f,
        -54.60640233f,
        2.122730258f,
        1.671281037f,
        -0.004171875264f,
        -0.00360720284f,
        -0.0787609381f,
        -0.1057253032f,
        -0.02043816084f,
        0.83972231f,
        1.494231303f,
    };

    static constexpr TireFitCombParamFx FIT_COMB_FX_12_PSI =
    {
        .rBx1 = 13.046f,
        .rBx2 = 9.718f,
        .rBx3 = 0.0f,
        .rCx1 = 0.9995f,
        .rEx1 = -0.4403f,
        .rEx2 = -0.4663f,
        .rHx1 = -0.0001f,
    };

    static constexpr TireFitCombParamFy FIT_COMB_FY_12_PSI =
    {
        .rBy1 = 10.622f,
        .rBy2 = 7.82f,
        .rBy3 = 0.00204f,
        .rBy4 = 0.0f,
        .rCy1 = 1.0587f,
        .rEy1 = 0.3148f,
        .rEy2 = 0.00487f,
        .rHy1 = 0.00947f,
        .rHy2 = 0.00975f,
        .rVy1 = 0.05187f,
        .rVy2 = 0.00049f,
        .rVy3 = 0.0f,
        .rVy4 = 94.63f,
        .rVy5 = 1.8914f,
        .rVy6 = 23.8f,
    };

    static constexpr float NOMINAL_FZ_N  = 750.0f;
    //-------------------------------------------------------------------- Class Helpers ----------------------------------------------------------------------//

    [[nodiscard]] static constexpr float safeSignedDenominator(const float value);
    [[nodiscard]] static constexpr float signum(const float value);
    [[nodiscard]] static constexpr float normalizedLoadDelta(const float normal_load_N);
    // Reduced-model assumptions for combined slip in this pass:
    // gamma* = 0, lambda_xa = 1, lambda_yk = 1, lambda_vyk = 1, zeta_2 = 1.
    // Pressure dependence is captured by the fixed 12_PSI fitted parameter row.
    //-------------------------------------------------------------------- Pure Pacejka 5.2 Helpers ----------------------------------------------------------------------//
    [[nodiscard]] constexpr float pureFx_Sh(const float normalized_load_delta) const;
    template <typename T>
    [[nodiscard]] T pureFx_Kappa(const float normalized_load_delta, const T& slip_ratio) const;
    [[nodiscard]] constexpr float pureFx_C() const;
    [[nodiscard]] constexpr float pureFx_mu(const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFx_D(const float normal_load_N, const float normalized_load_delta) const;
    template <typename T>
    [[nodiscard]] T pureFx_E(const float normalized_load_delta, const T& kappa_x) const;
    [[nodiscard]] constexpr float pureFx_K(const float normal_load_N, const float normalized_load_delta) const;
    [[nodiscard]] static constexpr float pureFx_B(
        const float slip_stiffness, const float shape_factor, const float peak_factor);
    [[nodiscard]] constexpr float pureFx_Sv(const float normal_load_N, const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFy_Sh(const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFy_Alpha(const float normalized_load_delta, const float slip_angle_rad) const;
    [[nodiscard]] constexpr float pureFy_C() const;
    [[nodiscard]] constexpr float pureFy_mu(const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFy_D(const float normal_load_N, const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFy_E(const float normalized_load_delta, const float alpha_y) const;
    [[nodiscard]] constexpr float pureFy_K(const float normal_load_N) const;
    [[nodiscard]] static constexpr float pureFy_B(
        const float cornering_stiffness, const float shape_factor, const float peak_factor);
    [[nodiscard]] constexpr float pureFy_Sv(const float normal_load_N, const float normalized_load_delta) const;
    template <typename T>
    [[nodiscard]] PureFxMagicFormulaCoefficients<T> pureFxMagicFormulaCoefficients(
        float normal_load_N, const T& slip_ratio) const;
    [[nodiscard]] PureFyMagicFormulaCoefficients pureFyMagicFormulaCoefficients(
        float normal_load_N, float slip_angle_rad) const;
    //-------------------------------------------------------------------- Combined Pacejka 5.2 Helpers ----------------------------------------------------------------------//
    [[nodiscard]] constexpr float combinedFx_SHxa() const;
    [[nodiscard]] constexpr float combinedFx_Alpha_s(float slip_angle_rad) const;
    [[nodiscard]] constexpr float combinedFx_Cxa() const;
    [[nodiscard]] float combinedFx_Exa(float normalized_load_delta) const;
    template <typename T>
    [[nodiscard]] T combinedFx_Bxa(const T& slip_ratio) const;
    template <typename T>
    [[nodiscard]] T combinedFx_Gxao(const CombinedFxMagicFormulaCoefficients<T>& coefficients) const;
    template <typename T>
    [[nodiscard]] T combinedFx_Gxa(const CombinedFxMagicFormulaCoefficients<T>& coefficients) const;
    template <typename T>
    [[nodiscard]] CombinedFxMagicFormulaCoefficients<T> combinedFxMagicFormulaCoefficients(
        float normal_load_N, float slip_angle_rad, const T& slip_ratio) const;
    [[nodiscard]] float combinedFy_SHyk(float normalized_load_delta) const;
    template <typename T>
    [[nodiscard]] T combinedFy_Kappa_s(float normalized_load_delta, const T& slip_ratio) const;
    [[nodiscard]] constexpr float combinedFy_Cyk() const;
    [[nodiscard]] float combinedFy_Eyk(float normalized_load_delta) const;
    [[nodiscard]] float combinedFy_Byk(float slip_angle_rad) const;
    [[nodiscard]] float combinedFy_Dvyk(float normal_load_N, float normalized_load_delta, float slip_angle_rad) const;
    template <typename T>
    [[nodiscard]] T combinedFy_Svyk(const T& d_vyk, const T& slip_ratio) const;
    template <typename T>
    [[nodiscard]] T combinedFy_Gyko(const CombinedFyMagicFormulaCoefficients<T>& coefficients) const;
    template <typename T>
    [[nodiscard]] T combinedFy_Gyk(const CombinedFyMagicFormulaCoefficients<T>& coefficients) const;
    template <typename T>
    [[nodiscard]] CombinedFyMagicFormulaCoefficients<T> combinedFyMagicFormulaCoefficients(
        float normal_load_N, float slip_angle_rad, const T& slip_ratio) const;

    [[nodiscard]] float wheelLongOffset_m() const;
    [[nodiscard]] float wheelLatOffset_m() const;
    [[nodiscard]] float wheelSteeringAngle_rad(const float steering_angle_rad) const;
    [[nodiscard]] WheelVelocities wheelVelocities(
        float vehicle_velocity_x_mps, float vehicle_velocity_y_mps, float yaw_rate_radps) const;
    [[nodiscard]] float estimateSlipAngle(
        float wheel_vel_x_mps, float wheel_vel_y_mps, float steering_angle_rad) const;
    [[nodiscard]] float estimateSlipRatio(
        float wheel_vel_x_mps, float wheel_vel_y_mps, float slip_angle_rad, float wheel_angular_velocity_radps) const;
    template <typename T>
    [[nodiscard]] T computePureFx_N(float normal_load_N, const T& slip_ratio) const;
    [[nodiscard]] float computePureFy_N(float normal_load_N, float slip_angle_rad) const;

    const TireFitPureParamFx fit_pure_fx_;
    const TireFitPureParamFy fit_pure_fy_;
    const TireFitCombParamFx fit_comb_fx_;
    const TireFitCombParamFy fit_comb_fy_;
    const WheelSide wheel_side_;
    const WheelAxle wheel_axle_;
};
} // namespace app::tv::estimation
