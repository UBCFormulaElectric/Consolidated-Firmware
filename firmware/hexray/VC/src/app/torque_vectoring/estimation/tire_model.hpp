#pragma once
#include "torque_vectoring/shared_datatypes/decimal_dual.hpp"

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

    // note that these only exist for float, dual
    template <DecimalOrDual T> [[nodiscard]] T computeCombinedFx_N(float fz_N, float alpha_rad, const T &kappa) const;
    template <DecimalOrDual T> [[nodiscard]] T computeCombinedFy_N(float fz_N, float alpha_rad, const T &kappa) const;

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

    TireModel() = delete;

  protected:
    constexpr TireModel(
        const TireFitPureParamFx &fit_pure_fx,
        const TireFitPureParamFy &fit_pure_fy,
        const TireFitCombParamFx &fit_comb_fx,
        const TireFitCombParamFy &fit_comb_fy)
      : fit_pure_fx_(fit_pure_fx), fit_pure_fy_(fit_pure_fy), fit_comb_fx_(fit_comb_fx), fit_comb_fy_(fit_comb_fy)
    {
    }

  private:
    template <DecimalOrDual T> struct PureFxMagicFormulaCoefficients
    {
        T s_hx    = T(0.0f);
        T kappa_x = T(0.0f);
        T b_x     = T(0.0f);
        T c_x     = T(0.0f);
        T d_x     = T(0.0f);
        T e_x     = T(0.0f);
        T s_vx    = T(0.0f);
    };

    template <DecimalOrDual T> struct CombinedFxMagicFormulaCoefficients
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

    template <DecimalOrDual T> struct CombinedFyMagicFormulaCoefficients
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

    static constexpr float NOMINAL_FZ_N = 750.0f;
    //-------------------------------------------------------------------- Class Helpers
    //----------------------------------------------------------------------//
    [[nodiscard]] static float normalizedLoadDelta(float normal_load_N);
    // Reduced-model assumptions for combined slip in this pass:
    // gamma* = 0, lambda_xa = 1, lambda_yk = 1, lambda_vyk = 1, zeta_2 = 1.
    // Pressure dependence is captured by the fixed 12_PSI fitted parameter row.
    //-------------------------------------------------------------------- Pure Pacejka 5.2 Helpers
    //----------------------------------------------------------------------//
    [[nodiscard]] constexpr float              pureFx_Sh(float normalized_load_delta) const;
    template <DecimalOrDual T> [[nodiscard]] T pureFx_Kappa(float normalized_load_delta, const T &slip_ratio) const;
    [[nodiscard]] constexpr float              pureFx_C() const;
    [[nodiscard]] constexpr float              pureFx_mu(float normalized_load_delta) const;
    [[nodiscard]] constexpr float              pureFx_D(float normal_load_N, float normalized_load_delta) const;
    template <DecimalOrDual T> [[nodiscard]] T pureFx_E(float normalized_load_delta, const T &kappa_x) const;
    [[nodiscard]] float                        pureFx_K(float normal_load_N, float normalized_load_delta) const;
    [[nodiscard]] static float                 pureFx_B(float slip_stiffness, float shape_factor, float peak_factor);
    [[nodiscard]] constexpr float              pureFx_Sv(float normal_load_N, float normalized_load_delta) const;
    [[nodiscard]] constexpr float              pureFy_Sh(float normalized_load_delta) const;
    [[nodiscard]] constexpr float              pureFy_Alpha(float normalized_load_delta, float slip_angle_rad) const;
    [[nodiscard]] constexpr float              pureFy_C() const;
    [[nodiscard]] constexpr float              pureFy_mu(float normalized_load_delta) const;
    [[nodiscard]] constexpr float              pureFy_D(float normal_load_N, float normalized_load_delta) const;
    [[nodiscard]] constexpr float              pureFy_E(float normalized_load_delta, float alpha_y) const;
    [[nodiscard]] float                        pureFy_K(float normal_load_N) const;
    [[nodiscard]] static float    pureFy_B(float cornering_stiffness, float shape_factor, float peak_factor);
    [[nodiscard]] constexpr float pureFy_Sv(float normal_load_N, float normalized_load_delta) const;
    template <DecimalOrDual T>
    [[nodiscard]] PureFxMagicFormulaCoefficients<T>
        pureFxMagicFormulaCoefficients(float normal_load_N, const T &slip_ratio) const;
    [[nodiscard]] PureFyMagicFormulaCoefficients pureFyMagicFormulaCoefficients(float fz, float alpha) const;
    //-------------------------------------------------------------------- Combined Pacejka 5.2 Helpers
    //----------------------------------------------------------------------//
    [[nodiscard]] constexpr float              combinedFx_SHxa() const;
    [[nodiscard]] constexpr float              combinedFx_Alpha_s(float alpha) const;
    [[nodiscard]] constexpr float              combinedFx_Cxa() const;
    [[nodiscard]] float                        combinedFx_Exa(float normalized_load_delta) const;
    template <DecimalOrDual T> [[nodiscard]] T combinedFx_Bxa(const T &kappa) const;
    template <DecimalOrDual T>
    [[nodiscard]] T combinedFx_Gxao(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const;
    template <DecimalOrDual T>
    [[nodiscard]] T combinedFx_Gxa(const CombinedFxMagicFormulaCoefficients<T> &coefficients) const;
    template <DecimalOrDual T>
    [[nodiscard]] CombinedFxMagicFormulaCoefficients<T>
        combinedFxMagicFormulaCoefficients(float normal_load_N, float slip_angle_rad, const T &slip_ratio) const;
    [[nodiscard]] float combinedFy_SHyk(float normalized_load_delta) const;
    template <DecimalOrDual T>
    [[nodiscard]] T               combinedFy_Kappa_s(float normalized_load_delta, const T &slip_ratio) const;
    [[nodiscard]] constexpr float combinedFy_Cyk() const;
    [[nodiscard]] float           combinedFy_Eyk(float normalized_load_delta) const;
    [[nodiscard]] float           combinedFy_Byk(float slip_angle_rad) const;
    [[nodiscard]] float combinedFy_Dvyk(float normal_load_N, float normalized_load_delta, float slip_angle_rad) const;
    template <DecimalOrDual T> [[nodiscard]] T combinedFy_Svyk(const T &d_vyk, const T &slip_ratio) const;
    template <DecimalOrDual T>
    [[nodiscard]] T combinedFy_Gyko(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const;
    template <DecimalOrDual T>
    [[nodiscard]] T combinedFy_Gyk(const CombinedFyMagicFormulaCoefficients<T> &coefficients) const;
    template <DecimalOrDual T>
    [[nodiscard]] CombinedFyMagicFormulaCoefficients<T>
        combinedFyMagicFormulaCoefficients(float normal_load_N, float slip_angle_rad, const T &slip_ratio) const;

    template <DecimalOrDual T> [[nodiscard]] T computePureFx_N(float fz, const T &kappa) const;
    [[nodiscard]] float                        computePureFy_N(float fz_N, float alpha) const;

    const TireFitPureParamFx &fit_pure_fx_;
    const TireFitPureParamFy &fit_pure_fy_;
    const TireFitCombParamFx &fit_comb_fx_;
    const TireFitCombParamFy &fit_comb_fy_;
};

class HoosierTireModel : public TireModel
{
    /*
    Current model uses the 12_PSI fitted workbook row as a fixed-pressure operating assumption.
    If pressure becomes a runtime input later, add pressure interpolation or a refit against the raw tire data.
    */
    static constexpr TireFitPureParamFx HOOSIER_FIT_PURE_FX_12_PSI = {
        0.008656294312f, 366.1925838f,     -57.2826791f,      8.208916842f,   1.134744942f,
        0.04728092792f,  -0.02191230313f,  -0.01799018414f,   62.46585932f,   1.390484252f,
        -0.3909753489f,  0.0005429909716f, -0.0007019708842f, 0.01286770906f, 0.004135955934f,
    };

    static constexpr TireFitPureParamFy HOOSIER_FIT_PURE_FY_12_PSI = {
        0.90721342f,     -3.824114982f,  -0.4280518359f, 15.56383821f,    1.218066219f, 0.3097318297f,
        -0.02930740966f, 0.2068388689f,  -54.60640233f,  2.122730258f,    1.671281037f, -0.004171875264f,
        -0.00360720284f, -0.0787609381f, -0.1057253032f, -0.02043816084f, 0.83972231f,  1.494231303f,
    };

    static constexpr TireFitCombParamFx HOOSIER_FIT_COMB_FX_12_PSI = {
        .rBx1 = 13.046f,
        .rBx2 = 9.718f,
        .rBx3 = 0.0f,
        .rCx1 = 0.9995f,
        .rEx1 = -0.4403f,
        .rEx2 = -0.4663f,
        .rHx1 = -0.0001f,
    };

    static constexpr TireFitCombParamFy HOOSIER_FIT_COMB_FY_12_PSI = {
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

  public:
    constexpr HoosierTireModel()
      : TireModel(
            HOOSIER_FIT_PURE_FX_12_PSI,
            HOOSIER_FIT_PURE_FY_12_PSI,
            HOOSIER_FIT_COMB_FX_12_PSI,
            HOOSIER_FIT_COMB_FY_12_PSI)
    {
    }
};

inline constexpr HoosierTireModel tire_model{};

} // namespace app::tv::estimation
