#include "torque_allocator.hpp"

#include <cmath>
#include <limits>
#include <utility>

#include <Eigen/Dense>
#include <dual.hpp>
#include <gradient.hpp>

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"
#include "torque_vectoring/tv_debug.h"

using namespace app::tv::shared_datatypes;
using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::allocator
{
namespace
{
    // ---- Optimizer tuning ----
    constexpr float W_FX = 0.5f;
    constexpr float W_MZ = 0.5f;
    constexpr float W_R  = 0.01f;

    constexpr int MAX_ITER = 20;
    // Static bound on each wheel's slip target. The tire's Fx peak moves with slip angle (~0.09 at alpha = 0,
    // ~0.24 at alpha = 0.1), so this is a compromise: too low caps cornering force, too high lets straight-line
    // targets go past the peak (unstable wheel-speed loop, second cost basin, cycle-to-cycle jumps).
    constexpr float SLIP_CLAMP = 0.4f;
    // The combined-slip Fx fit (rCx1 sits on its fit bound at 1.4) flips sign beyond |alpha| ~ 0.22 rad, which is
    // outside the measured data. Saturate the slip angle fed to the fit at the edge of its valid region.
    constexpr float FIT_MAX_ALPHA_RAD = 0.2f;
    constexpr float NORMAL_MATRIX_EPS = 1e-6f; // Levenberg-Marquardt damping floor
    constexpr float STEP_TOLERANCE    = 1e-5f;
    constexpr float COST_TOLERANCE    = 1e-6f;

    constexpr float LM_LAMBDA_INIT = 1e-3f;
    // When the force request is infeasible (e.g. full throttle), the Gauss-Newton step overshoots the flat tire peak.
    // Damping must be able to grow far past ||J^T J|| (up to ~1e7 here) to reach a short gradient step; otherwise
    // every try in an iteration is rejected and the solve stops short of the peak.
    constexpr float LM_LAMBDA_UP   = 10.0f;
    constexpr float LM_LAMBDA_DOWN = 0.3f;
    constexpr int   LM_MAX_TRIES   = 10;

    template <Decimal T> using Vec6     = Eigen::Matrix<T, 6, 1>;
    template <Decimal T> using Vec4     = Eigen::Matrix<T, 4, 1>;
    template <Decimal T> using Mat64    = Eigen::Matrix<T, 6, 4>;
    template <Decimal T> using Mat44    = Eigen::Matrix<T, 4, 4>;
    template <Decimal T> using DualVec6 = Eigen::Matrix<DecimalDual<T>, 6, 1>;
    template <Decimal T> using DualVec4 = Eigen::Matrix<DecimalDual<T>, 4, 1>;
} // namespace

template <Decimal T>
[[nodiscard]] wheel_set<T> optimize(const VehicleState<T> &state, const T ax_setpoint, const T omegadot_setpoint)
{
    // Low-speed safeguard:
    // torque_vectoring.cpp computes a single force-availability blend from vehicle speed and passes it
    // into the allocator. Keeping that policy decision outside the optimizer makes the heuristic explicit
    // at the orchestration layer while the optimizer itself only consumes the already-decided scaling.

    // Below a very small blend threshold, there is no meaningful traction allocation problem to solve,
    // so return zero requested slip immediately.
    // if (low_speed_blend < 0.05f)
    // {
    //     return { .fl = 0.0f, .fr = 0.0f, .rl = 0.0f, .rr = 0.0f };
    // }

    static const T SQRT_W_FX     = std::sqrt(W_FX);
    static const T SQRT_W_MZ     = std::sqrt(W_MZ);
    static const T SQRT_W_R      = std::sqrt(W_R);
    static Vec4<T> previous_slip = Vec4<T>::Zero(); // warm start for the next cycle

    // const wheel_set<float> blended_des_f_x{
    //     .fl = low_speed_blend * des_f_x.fl,
    //     .fr = low_speed_blend * des_f_x.fr,
    //     .rl = low_speed_blend * des_f_x.rl,
    //     .rr = low_speed_blend * des_f_x.rr,
    // };
    // const float blended_des_m_z = low_speed_blend * des_M_z;

    // Reference material used to shape this implementation:
    // - Video walkthrough: https://www.youtube.com/watch?v=C6DCtQjKkdY
    // - Wikipedia summary: https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm
    //
    // Residual vector for Gauss-Newton:
    //   r = [sqrt(W_FX) * (Fx_i - des_fx_i), sqrt(W_MZ) * (Mz - des_Mz)]^T
    //
    // Why this form:
    // - The allocator is naturally a least-squares problem: track desired wheel forces while also
    //   matching a desired yaw moment.
    // - Gauss-Newton is a good fit because it works directly on residuals and only needs the
    //   residual Jacobian J = dr/dkappa, which autodiff can compute for us cleanly.
    //
    // The solve uses the textbook normal equations:
    //   (J^T J) delta = -J^T r
    // where:
    //   r = residual vector evaluated at the current trial slip
    //   J = dr/dkappa evaluated at the current trial slip
    //
    // We keep all vectors/matrices fixed-size (4 decision variables, 2 residuals) so the optimizer
    // stays allocation-free and predictable on embedded targets.
    const auto [fz_fl, fz_fr, fz_rl, fz_rr]             = state.est_Fz_N();
    const auto [alpha_fl, alpha_fr, alpha_rl, alpha_rr] = state.alphas();
    tv_debug_data.veh_state.alpha_rad[0] = alpha_fl, tv_debug_data.veh_state.alpha_rad[1] = alpha_fr;
    tv_debug_data.veh_state.alpha_rad[2] = alpha_rl, tv_debug_data.veh_state.alpha_rad[3] = alpha_rr;
    tv_debug_data.veh_state.fz_N[0] = fz_fl, tv_debug_data.veh_state.fz_N[1] = fz_fr;
    tv_debug_data.veh_state.fz_N[2] = fz_rl, tv_debug_data.veh_state.fz_N[3] = fz_rr;
    // The fit describes one physical tire, and the right-side tires are its mirror image. Using the fit unmirrored on
    // both sides puts its ply-steer/conicity offsets (Fy ~ -20 N at alpha = 0) in the same direction on all four
    // tires, which predicts a yaw moment the car does not have; the optimizer then "cancels" it with a real
    // left/right Fx split and the car yaws in a straight line. Mirroring makes the offsets splay symmetrically.
    const auto fitAlpha = [](const float alpha, const bool right_side)
    { return std::clamp(right_side ? -alpha : alpha, -FIT_MAX_ALPHA_RAD, FIT_MAX_ALPHA_RAD); };
    const auto tireForces = [&](const float fz, const float alpha, const DecimalDual<T> &k, const bool right_side)
    {
        const float fit_alpha = fitAlpha(alpha, right_side);
        const float fy_sign   = right_side ? -1.0f : 1.0f;
        return Pair<DecimalDual<T>>{
            estimation::tire_model.computeCombinedFx_N<DecimalDual<T>>(fz, fit_alpha, k),
            static_cast<T>(fy_sign) * estimation::tire_model.computeCombinedFy_N<DecimalDual<T>>(fz, fit_alpha, k),
        };
    };

    const auto residualVector = [&](const DualVec4<T> &kappa) -> DualVec6<T>
    {
        const wheel_set<Pair<DecimalDual<T>>> predicted_f{
            tireForces(fz_fl, alpha_fl, kappa(0), false),
            tireForces(fz_fr, alpha_fr, kappa(1), true),
            tireForces(fz_rl, alpha_rl, kappa(2), false),
            tireForces(fz_rr, alpha_rr, kappa(3), true),
        };
        const DecimalDual<T> sum_fx_over_1k =
            predicted_f.fl.x / 1000 + predicted_f.fr.x / 1000 + predicted_f.rl.x / 1000 + predicted_f.rr.x / 1000;
        const DecimalDual<T> predicted_mz = state.est_Mz_N(predicted_f);

        return DualVec6<T>{
            SQRT_W_FX * (sum_fx_over_1k - static_cast<T>(CAR_MASS_AT_CG_KG) * ax_setpoint / 1000),
            // Express both tracked quantities in kilo-units so W_FX and W_MZ have comparable meaning.
            SQRT_W_MZ * (predicted_mz / 1000 - omegadot_setpoint / 1000),
            SQRT_W_R * kappa[0],
            SQRT_W_R * kappa[1],
            SQRT_W_R * kappa[2],
            SQRT_W_R * kappa[3],
        };
    };

    const auto costAt = [&](const Vec4<T> &slip) -> float
    {
        const DualVec4<T> kappa{
            DecimalDual<T>(slip(0)),
            DecimalDual<T>(slip(1)),
            DecimalDual<T>(slip(2)),
            DecimalDual<T>(slip(3)),
        };
        const DualVec6<T> residual = residualVector(kappa);
        float             cost     = 0.0f;
        for (int i = 0; i < residual.size(); ++i)
        {
            const float value = static_cast<float>(autodiff::val(residual(i)));
            cost += value * value;
        }
        return cost;
    };

    // Solve from two starting points and keep the cheaper result:
    //  - the previous cycle's solution (warm start): gives cycle-to-cycle continuity. With saturated / infeasible
    //    requests the cost has several local minima, and a cold start alone can land in a different one from one
    //    cycle to the next, which shows up as targets chattering between two values;
    //  - zero slip (cold start): escapes a basin the warm start may have been dragged into (e.g. by a start-up
    //    transient), which on its own used to trap every later cycle. It also bounds the effect of the warm state
    //    carrying over between Simulink runs.
    // On a tie the warm solution wins, so equal-cost minima do not alternate.
    const auto solveFrom = [&](const Vec4<T> &start, uint32_t &iterations) -> std::pair<Vec4<T>, float>
    {
        Vec4<T> opt_slip = start;
        for (int i = 0; i < 4; ++i)
            opt_slip(i) = std::clamp(opt_slip(i), -static_cast<T>(SLIP_CLAMP), static_cast<T>(SLIP_CLAMP));
        float    current_cost = costAt(opt_slip);
        Vec4<T>  best_slip    = opt_slip;
        float    best_cost    = current_cost;
        T        lambda       = static_cast<T>(LM_LAMBDA_INIT);
        uint32_t iter;
        for (iter = 0; iter < MAX_ITER; ++iter)
        {
            DualVec4<T> kappa{
                DecimalDual<T>(opt_slip(0)),
                DecimalDual<T>(opt_slip(1)),
                DecimalDual<T>(opt_slip(2)),
                DecimalDual<T>(opt_slip(3)),
            };
            // evaluate and calculate jacobian at kappa
            DualVec6<T> residual_at_kappa;
            Mat64<T>    jacobian_residual_at_kappa;
            autodiff::jacobian(
                residualVector, autodiff::wrt(kappa), autodiff::at(kappa), residual_at_kappa,
                jacobian_residual_at_kappa);
            const Vec6<T> residuals_at_kappa_primal{
                autodiff::val(residual_at_kappa(0)), autodiff::val(residual_at_kappa(1)),
                autodiff::val(residual_at_kappa(2)), autodiff::val(residual_at_kappa(3)),
                autodiff::val(residual_at_kappa(4)), autodiff::val(residual_at_kappa(5)),
            };
            const Mat44<T> normal_matrix = jacobian_residual_at_kappa.transpose() * jacobian_residual_at_kappa;
            const Vec4<T>  rhs           = -jacobian_residual_at_kappa.transpose() * residuals_at_kappa_primal;

            const float cost_before    = current_cost;
            bool        accepted_step  = false;
            Vec4<T>     accepted_delta = Vec4<T>::Zero();

            // Increase damping until the nonlinear cost decreases. Larger lambda moves the update away from the
            // poorly conditioned Gauss-Newton direction and toward a shorter gradient-descent step.
            for (int attempt = 0; attempt < LM_MAX_TRIES; ++attempt)
            {
                Mat44<T> augmented_normal_matrix = normal_matrix;
                augmented_normal_matrix.diagonal().array() += lambda;
                const Eigen::LDLT<Mat44<T>> ldlt(augmented_normal_matrix);
                if (ldlt.info() != Eigen::Success)
                {
                    lambda *= static_cast<T>(LM_LAMBDA_UP);
                    continue;
                }

                const Vec4<T> delta = ldlt.solve(rhs);
                if (!delta.allFinite())
                {
                    lambda *= static_cast<T>(LM_LAMBDA_UP);
                    continue;
                }

                Vec4<T> trial_slip = opt_slip + delta;
                for (int i = 0; i < 4; ++i)
                    trial_slip(i) = std::clamp(trial_slip(i), -static_cast<T>(SLIP_CLAMP), static_cast<T>(SLIP_CLAMP));

                const float trial_cost = costAt(trial_slip);
                if (trial_cost < current_cost)
                {
                    accepted_delta = trial_slip - opt_slip;
                    opt_slip       = trial_slip;
                    current_cost   = trial_cost;
                    lambda = std::max(lambda * static_cast<T>(LM_LAMBDA_DOWN), static_cast<T>(NORMAL_MATRIX_EPS));
                    accepted_step = true;
                    break;
                }

                lambda *= static_cast<T>(LM_LAMBDA_UP);
            }

            if (current_cost < best_cost)
            {
                best_cost = current_cost;
                best_slip = opt_slip;
            }

            if (!accepted_step || accepted_delta.norm() < STEP_TOLERANCE ||
                (cost_before - current_cost) < COST_TOLERANCE)
                break;
        }
        iterations = iter;
        return { best_slip, best_cost };
    };

    uint32_t cold_iterations = 0, warm_iterations = 0;
    const auto [cold_slip, cold_cost] = solveFrom(Vec4<T>::Zero(), cold_iterations);
    const auto [warm_slip, warm_cost] = solveFrom(previous_slip, warm_iterations);
    const bool    use_warm            = warm_cost <= cold_cost + COST_TOLERANCE;
    const Vec4<T> best_slip           = use_warm ? warm_slip : cold_slip;
    previous_slip                     = best_slip;

    // Residuals at the chosen slips, reported in the debug struct.
    const DualVec6<T> residual_at_solution = residualVector({
        DecimalDual<T>(best_slip(0)),
        DecimalDual<T>(best_slip(1)),
        DecimalDual<T>(best_slip(2)),
        DecimalDual<T>(best_slip(3)),
    });
    const double      r_fx                 = autodiff::val(residual_at_solution(0));
    const double      r_mz                 = autodiff::val(residual_at_solution(1));
    tv_debug_data.optimizer                = {
                       .kappas     = { best_slip(0), best_slip(1), best_slip(2), best_slip(3) },
                       .r_ax       = r_fx,
                       .r_Mz       = r_mz,
                       .fx_des_N   = CAR_MASS_AT_CG_KG * ax_setpoint,
                       .mz_des_Nm  = omegadot_setpoint,
                       .fx_pred_N  = (r_fx / SQRT_W_FX + CAR_MASS_AT_CG_KG * ax_setpoint / 1000) * 1000,
                       .mz_pred_Nm = (r_mz / SQRT_W_MZ + omegadot_setpoint / 1000) * 1000,
    };

    return {
        .fl = best_slip(0),
        .fr = best_slip(1),
        .rl = best_slip(2),
        .rr = best_slip(3),
    };
}

template wheel_set<float>  optimize(const VehicleState<float> &state, float ax_setpoint, float omegadot_setpoint);
template wheel_set<double> optimize(const VehicleState<double> &state, double ax_setpoint, double omegadot_setpoint);
} // namespace app::tv::controllers::allocator
