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

    // note that these only exist for float, double, dual
    template <DecimalOrDual T> [[nodiscard]] T computeCombinedFx_N(float fz_N, float alpha_rad, const T &kappa) const;
    template <DecimalOrDual T> [[nodiscard]] T computeCombinedFy_N(float fz_N, float alpha_rad, const T &kappa) const;

    struct TireFitPureParamFy
    {
        float p_Cy1;
        float p_Dy1;
        float p_Dy2;
        float p_Dy3;
        float p_Ey1;
        float p_Ey2;
        float p_Ey3;
        float p_Ey4;
        float p_Ey5;
        float p_Ky1;
        float p_Ky2;
        float p_Ky3;
        float p_Ky4;
        float p_Ky5;
        float p_Ky6;
        float p_Ky7;
        float p_Hy1;
        float p_Hy2;
        float p_Vy1;
        float p_Vy2;
        float p_Vy3;
        float p_Vy4;
        float p_py1;
        float p_py2;
        float p_py3;
        float p_py4;
        float p_py5;
    };

    struct TireFitPureParamFx
    {
        float p_Cx1;
        float p_Dx1;
        float p_Dx2;
        float p_Dx3;
        float p_Ex1;
        float p_Ex2;
        float p_Ex3;
        float p_Ex4;
        float p_Kx1;
        float p_Kx2;
        float p_Kx3;
        float p_Hx1;
        float p_Hx2;
        float p_Vx1;
        float p_Vx2;
        float p_px1;
        float p_px2;
        float p_px3;
        float p_px4;
    };

    struct TireScalingFactors
    {
        float LFZ0;
        float LCX;
        float LMUX;
        float LEX;
        float LKX;
        float LHX;
        float LVX;
        float LGAX;
        float LXAL;
        float LCY;
        float LMUY;
        float LEY;
        float LKY;
        float LKYG;
        float LHY;
        float LVY;
        float LGAY;
        float LYKA;
        float LVYKA;
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
        const TireFitCombParamFy &fit_comb_fy,
        const TireScalingFactors &scaling_factors)
      : fit_pure_fx_(fit_pure_fx),
        fit_pure_fy_(fit_pure_fy),
        fit_comb_fx_(fit_comb_fx),
        fit_comb_fy_(fit_comb_fy),
        scaling_factors_(scaling_factors)
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

    static constexpr float FZ0 = 890.0f;
    static constexpr float R0  = 0.230885999f;
    static constexpr float P0  = 82737.12f;
    //-------------------------------------------------------------------- Class Helpers
    //----------------------------------------------------------------------//
    [[nodiscard]] float normalizedLoadDelta(float normal_load_N) const;
    [[nodiscard]] float referenceNormalLoad_N() const;
    // Reduced MF 6.2 assumptions in this implementation:
    // gamma = 0, dpi = 0, zeta_0..zeta_4 = 1, and the scaling factors below come from the fixed Hoosier fit row.
    //-------------------------------------------------------------------- Pure Pacejka MF 6.2 Helpers
    //----------------------------------------------------------------------//
    [[nodiscard]] static constexpr float       pacejkaSlipAngle(float slip_angle_rad);
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
    [[nodiscard]] float                        pureFy_E(float normalized_load_delta, float alpha_y) const;
    [[nodiscard]] float                        pureFy_K(float normal_load_N) const;
    [[nodiscard]] static float    pureFy_B(float cornering_stiffness, float shape_factor, float peak_factor);
    [[nodiscard]] constexpr float pureFy_Sv(float normal_load_N, float normalized_load_delta) const;
    template <DecimalOrDual T>
    [[nodiscard]] PureFxMagicFormulaCoefficients<T>
        pureFxMagicFormulaCoefficients(float normal_load_N, const T &slip_ratio) const;
    [[nodiscard]] PureFyMagicFormulaCoefficients pureFyMagicFormulaCoefficients(float fz, float alpha) const;
    //-------------------------------------------------------------------- Combined Pacejka MF 6.2 Helpers
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
    const TireScalingFactors &scaling_factors_;
};

class HoosierTireModel : public TireModel
{
    /*
    Current model uses the 12_PSI fitted workbook row as a fixed-pressure operating assumption.
    If pressure becomes a runtime input later, add pressure interpolation or a refit against the raw tire data.
    */
    static constexpr TireFitPureParamFx HOOSIER_FIT_PURE_FX_12_PSI = {
        .p_Cx1 = 1.4779238369371919f,
        .p_Dx1 = 2.181045396988853f,
        .p_Dx2 = 0.07001659216024461f,
        .p_Dx3 = 4.097157624686553f,
        .p_Ex1 = 0.24999999999999986f,
        .p_Ex2 = -0.4935998299083556f,
        .p_Ex3 = 0.062308987383295614f,
        .p_Ex4 = 0.49999999999999994f,
        .p_Kx1 = 44.40869215364958f,
        .p_Kx2 = -0.11467697610242364f,
        .p_Kx3 = 0.005123300455879327f,
        .p_Hx1 = 0.00028611316090635194f,
        .p_Hx2 = -0.001728319335508632f,
        .p_Vx1 = 0.004999999999999999f,
        .p_Vx2 = 0.00477148979863604f,
        .p_px1 = 5.985592437722123e-20f,
        .p_px2 = -1.4999999999999998f,
        .p_px3 = -0.45857497477578263f,
        .p_px4 = -0.43069292241848617f,
    };

    static constexpr TireFitPureParamFy HOOSIER_FIT_PURE_FY_12_PSI = {
        .p_Cy1 = 1.1204027009158741f,
        .p_Dy1 = 2.5264849720779265f,
        .p_Dy2 = -0.32531957059268557f,
        .p_Dy3 = 8.961207772979192f,
        .p_Ey1 = -0.3535685826026044f,
        .p_Ey2 = 0.49999999999999994f,
        .p_Ey3 = 0.49999999999999994f,
        .p_Ey4 = -4.3346742910317335e-12f,
        .p_Ey5 = -9.999999999996396f,
        .p_Ky1 = -58.87561425961986f,
        .p_Ky2 = 2.6000000000000005f,
        .p_Ky3 = 0.8244703642222122f,
        .p_Ky4 = 1.7352802106825729f,
        .p_Ky5 = 0.24677855458172623f,
        .p_Ky6 = -4.063776528205966f,
        .p_Ky7 = -1.9999999999999838f,
        .p_Hy1 = 0.0018254580526529563f,
        .p_Hy2 = 0.000636192957410061f,
        .p_Vy1 = 0.049999999999999996f,
        .p_Vy2 = -0.0036551964003752914f,
        .p_Vy3 = 0.09833213814504116f,
        .p_Vy4 = 0.046585547800409376f,
        .p_py1 = 0.5693120879649871f,
        .p_py2 = 0.9999999999962573f,
        .p_py3 = -0.15182658503935315f,
        .p_py4 = -1.6963735361954737e-22f,
        .p_py5 = -0.9999999999931753f,
    };

    static constexpr TireFitCombParamFx HOOSIER_FIT_COMB_FX_12_PSI = {
        .rBx1 = 7.480599472060266f,
        .rBx2 = 10.358691627632123f,
        .rBx3 = 29.999999999999996f,
        .rCx1 = 1.399999999995603f,
        .rEx1 = -1.1134570691694545f,
        .rEx2 = -0.5558333477828554f,
        .rHx1 = -9.999999999999997e-07f,
    };

    static constexpr TireFitCombParamFy HOOSIER_FIT_COMB_FY_12_PSI = {
        .rBy1 = 20.05591682759158f,
        .rBy2 = 19.99972340129322f,
        .rBy3 = 9.403590493232056e-07f,
        .rBy4 = -10.995639456729068f,
        .rCy1 = 0.9048173729141128f,
        .rEy1 = -0.7096982445643287f,
        .rEy2 = 0.55848931432607f,
        .rHy1 = 9.999999999999997e-07f,
        .rHy2 = 9.999999999999784e-07f,
        .rVy1 = 0.0009999999999999998f,
        .rVy2 = 0.001999999999999954f,
        .rVy3 = 0.049999999999999996f,
        .rVy4 = -1.9999999999999998f,
        .rVy5 = 2.4544048158091005f,
        .rVy6 = 5.900331527230952f,
    };

    static constexpr TireScalingFactors HOOSIER_SCALING_FACTORS = {
        .LFZ0  = 1.0f,
        .LCX   = 1.0f,
        .LMUX  = 0.65f,
        .LEX   = 1.0f,
        .LKX   = 1.0f,
        .LHX   = 1.0f,
        .LVX   = 1.0f,
        .LGAX  = 1.0f,
        .LXAL  = 1.0f,
        .LCY   = 1.0f,
        .LMUY  = 0.65f,
        .LEY   = 1.0f,
        .LKY   = 1.0f,
        .LKYG  = 1.0f,
        .LHY   = 1.0f,
        .LVY   = 1.0f,
        .LGAY  = 1.0f,
        .LYKA  = 1.0f,
        .LVYKA = 1.0f,
    };

  public:
    constexpr HoosierTireModel()
      : TireModel(
            HOOSIER_FIT_PURE_FX_12_PSI,
            HOOSIER_FIT_PURE_FY_12_PSI,
            HOOSIER_FIT_COMB_FX_12_PSI,
            HOOSIER_FIT_COMB_FY_12_PSI,
            HOOSIER_SCALING_FACTORS)
    {
    }
};

inline constexpr HoosierTireModel tire_model{};

} // namespace app::tv::estimation
