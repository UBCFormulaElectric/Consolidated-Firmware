#pragma once

#include <cmath>

#include "torque_vectoring/shared_datatypes/constants.hpp"

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

    enum class TirePressure
    {
        PSI_10,
        PSI_12
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

    constexpr TireModel(const TirePressure tire_pressure, const WheelSide wheel_side, const WheelAxle wheel_axle)
        : coeff_fx_(selectFxCoefficients(tire_pressure)),
          coeff_fy_(selectFyCoefficients(tire_pressure)),
          wheel_side_(wheel_side),
          wheel_axle_(wheel_axle)
    {
    }

    [[nodiscard]] Outputs estimate(const StateInputs& inputs);
    [[nodiscard]] float slipRatioToWheelAngularVelocity(const float slip_ratio, const float wheel_vel_x_mps) const;

private:
    struct TireCoefficents_Fy
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

    struct TireCoefficents_Fx
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

    struct PureFxMagicFormulaCoefficients
    {
        float kappa_x = 0.0f;
        float c_x     = 0.0f;
        float d_x     = 0.0f;
        float b_x     = 0.0f;
        float e_x     = 0.0f;
        float s_vx    = 0.0f;
    };

    struct PureFyMagicFormulaCoefficients
    {
        float alpha_y = 0.0f;
        float c_y     = 0.0f;
        float d_y     = 0.0f;
        float b_y     = 0.0f;
        float e_y     = 0.0f;
        float s_vy    = 0.0f;
    };

    /*
    These are discrete pressure snapshots from the coefficient workbook. If pressure becomes a continuous input later,
    replace this with interpolation or a refit against the raw tire data.
    */
    static constexpr TireCoefficents_Fx Fx_10_PSI = {
        0.01284039486f,
        262.5114145f,
        -41.08996459f,
        8.15087112f,
        1.175111664f,
        0.04701116929f,
        -0.009561725595f,
        -0.01302948897f,
        62.43954722f,
        -0.05063343595f,
        -0.3634867697f,
        -0.0003767549902f,
        0.0008216827314f,
        -0.002877423451f,
        -0.01695834766f,
    };

    static constexpr TireCoefficents_Fx Fx_12_PSI = {
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

    static constexpr TireCoefficents_Fy Fy_10_PSI = {
        1.652843597f,
        -2.803116154f,
        -0.2320399036f,
        17.97429428f,
        1.450330291f,
        0.5012143046f,
        -0.02726754434f,
        -0.7645474843f,
        -52.17053158f,
        1.886718468f,
        1.530410198f,
        -0.003839066017f,
        -0.003970614639f,
        -0.1017943816f,
        -0.08574689113f,
        -0.02575321928f,
        0.5309737209f,
        1.493197148f,
    };

    static constexpr TireCoefficents_Fy Fy_12_PSI = {
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

    static constexpr float NOMINAL_FZ_N  = 750.0f;
    static constexpr float WHEEL_RADIUS_M = (WHEEL_DIAMETER_IN * IN_TO_M) * 0.5f;

    [[nodiscard]] static constexpr TireCoefficents_Fx selectFxCoefficients(const TirePressure tire_pressure)
    {
        return tire_pressure == TirePressure::PSI_10 ? Fx_10_PSI : Fx_12_PSI;
    }

    [[nodiscard]] static constexpr TireCoefficents_Fy selectFyCoefficients(const TirePressure tire_pressure)
    {
        return tire_pressure == TirePressure::PSI_10 ? Fy_10_PSI : Fy_12_PSI;
    }

    [[nodiscard]] static constexpr float safeSignedDenominator(const float value);
    [[nodiscard]] static constexpr float signum(const float value);
    [[nodiscard]] static constexpr float normalizedLoadDelta(const float normal_load_N);
    [[nodiscard]] constexpr float pureFx_Sh(const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFx_Kappa(const float normalized_load_delta, const float slip_ratio) const;
    [[nodiscard]] constexpr float pureFx_C() const;
    [[nodiscard]] constexpr float pureFx_mu(const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFx_D(const float normal_load_N, const float normalized_load_delta) const;
    [[nodiscard]] constexpr float pureFx_E(const float normalized_load_delta, const float kappa_x) const;
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
    void pureFxMagicFormulaCoefficients(const float normal_load_N);
    [[nodiscard]] constexpr float pure_Fx() const;
    void pureFyMagicFormulaCoefficients(const float normal_load_N);
    [[nodiscard]] constexpr float pure_Fy() const;

    [[nodiscard]] float wheelLongOffset_m() const;
    [[nodiscard]] float wheelLatOffset_m() const;
    [[nodiscard]] float wheelSteeringAngle_rad(const float steering_angle_rad) const;
    void estimateWheelVelocities(float vehicle_velocity_x_mps, float vehicle_velocity_y_mps, float yaw_rate_radps);
    void estimateSlipAngle(float steering_angle_rad);
    void estimateSlipRatio(const float wheel_angular_velocity_radps);
    void computePureLongitudinalForce(const float normal_load_N);
    void computePureLateralForce(const float normal_load_N);
    void estimateForces(const float normal_load_N);

    const TireCoefficents_Fx coeff_fx_;
    const TireCoefficents_Fy coeff_fy_;
    const WheelSide wheel_side_;
    const WheelAxle wheel_axle_;
    PureFxMagicFormulaCoefficients pure_fx_mj_coefficients_{};
    PureFyMagicFormulaCoefficients pure_fy_mj_coefficients_{};
    float wheel_vel_x_mps_ = 0.0f;
    float wheel_vel_y_mps_ = 0.0f;
    Outputs tire_outputs_{};
};
} // namespace app::tv::estimation
