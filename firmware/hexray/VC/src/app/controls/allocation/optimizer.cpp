#include "controls/allocation/optimizer.hpp"

#include <cmath>
#include <limits>
#include <utility>

#include <Eigen/Dense>
#include <dual.hpp>
#include <gradient.hpp>

#include "controls/params.hpp"
#include "controls/estimation/tire_model.hpp"
#include "controls/controls_debug.h"

using namespace app::controls;
using namespace app::controls::params;

namespace app::controls::allocation
{
namespace
{
    // ---- Optimizer tuning ----
    constexpr float W_FX = 2.5f;
    constexpr float W_MZ = 5.0f;
    constexpr float W_R  = 100.0f; // tuned with the yaw Kp on the driver_sim autocross lap (was 500)

    constexpr int MAX_ITER = 20;
    // Static bound on each wheel's slip target. The tire's Fx peak moves with slip angle (~0.09 at alpha = 0,
    // ~0.24 at alpha = 0.1), so this is a compromise: too low caps cornering force, too high lets straight-line
    // targets go past the peak (unstable wheel-speed loop, second cost basin, cycle-to-cycle jumps).
    constexpr float KAPPA_LIMIT       = 0.4f;
    constexpr float NORMAL_MATRIX_EPS = 1e-6f; // Levenberg-Marquardt damping floor
    constexpr float STEP_TOLERANCE    = 1e-5f;
    constexpr float COST_TOLERANCE    = 1e-6f;

    constexpr float LM_LAMBDA_INIT = 1e-3f;
    // When the force request is infeasible (e.g. full throttle), the Gauss-Newton step overshoots the flat tire peak.
    // Damping must be able to grow far past ||J^float J|| (up to ~1e7 here) to reach a short gradient step; otherwise
    // every try in an iteration is rejected and the solve stops short of the peak.
    constexpr float LM_LAMBDA_UP   = 10.0f;
    constexpr float LM_LAMBDA_DOWN = 0.3f;
    constexpr int   LM_MAX_TRIES   = 10;

    using Vec6     = Eigen::Matrix<float, 6, 1>;
    using Vec4     = Eigen::Matrix<float, 4, 1>;
    using Mat64    = Eigen::Matrix<float, 6, 4>;
    using Mat44    = Eigen::Matrix<float, 4, 4>;
    using DualVec6 = Eigen::Matrix<DecimalDual<float>, 6, 1>;
    using DualVec4 = Eigen::Matrix<DecimalDual<float>, 4, 1>;
} // namespace

[[nodiscard]] wheel_set<float>
    optimize(const VehicleState &state, const float ax_setpoint_mps2, const float mz_setpoint_Nm)
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

    static const float SQRT_W_FX      = std::sqrt(W_FX);
    static const float SQRT_W_MZ      = std::sqrt(W_MZ);
    static const float SQRT_W_R       = std::sqrt(W_R);
    static Vec4        previous_kappa = Vec4::Zero(); // warm start for the next cycle

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
    //   r = [sqrt(W_FX) * (Fx_i - des_fx_i), sqrt(W_MZ) * (Mz - des_Mz)]^float
    //
    // Why this form:
    // - The allocator is naturally a least-squares problem: track desired wheel forces while also
    //   matching a desired yaw moment.
    // - Gauss-Newton is a good fit because it works directly on residuals and only needs the
    //   residual Jacobian J = dr/dkappa, which autodiff can compute for us cleanly.
    //
    // The solve uses the textbook normal equations:
    //   (J^float J) delta = -J^float r
    // where:
    //   r = residual vector evaluated at the current trial slip
    //   J = dr/dkappa evaluated at the current trial slip
    //
    // We keep all vectors/matrices fixed-size (4 decision variables, 2 residuals) so the optimizer
    // stays allocation-free and predictable on embedded targets.
    const wheel_set<float> fz_N      = state.estFz_N();
    const wheel_set<float> alpha_rad = state.alphas();
    alpha_rad.copyTo(tv_debug_data.veh_state.alpha_rad);
    fz_N.copyTo(tv_debug_data.veh_state.fz_N);

    const auto residualVector = [&](const DualVec4 &kappa_dual) -> DualVec6
    {
        // Mirrored for the right-side tires inside tireForces_N. Unmirrored, the optimizer would "cancel" the fit's
        // offsets with a real left/right Fx split and the car would yaw in a straight line.
        const wheel_set<Pair<DecimalDual<float>>> predicted_forces_N = estimation::tireForces_N(
            fz_N, alpha_rad,
            wheel_set<DecimalDual<float>>{ kappa_dual(0), kappa_dual(1), kappa_dual(2), kappa_dual(3) });
        const DecimalDual<float> sum_fx_kN = predicted_forces_N.fl.x / 1000 + predicted_forces_N.fr.x / 1000 +
                                             predicted_forces_N.rl.x / 1000 + predicted_forces_N.rr.x / 1000;
        const DecimalDual<float> predicted_mz_Nm = state.estMz_Nm(predicted_forces_N);

        return DualVec6{
            SQRT_W_FX * (sum_fx_kN - CAR_MASS_AT_CG_kg * ax_setpoint_mps2 / 1000),
            // Express both tracked quantities in kilo-units so W_FX and W_MZ have comparable meaning.
            SQRT_W_MZ * (predicted_mz_Nm / 1000 - mz_setpoint_Nm / 1000),
            SQRT_W_R * kappa_dual[0],
            SQRT_W_R * kappa_dual[1],
            SQRT_W_R * kappa_dual[2],
            SQRT_W_R * kappa_dual[3],
        };
    };

    const auto costAt = [&](const Vec4 &kappa) -> float
    {
        const DualVec4 kappa_dual{
            DecimalDual<float>(kappa(0)),
            DecimalDual<float>(kappa(1)),
            DecimalDual<float>(kappa(2)),
            DecimalDual<float>(kappa(3)),
        };
        const DualVec6 residual = residualVector(kappa_dual);
        float          cost     = 0.0f;
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
    const auto solveFrom = [&](const Vec4 &start, uint32_t &iterations) -> std::pair<Vec4, float>
    {
        Vec4 current_kappa = start;
        for (int i = 0; i < 4; ++i)
            current_kappa(i) = std::clamp(current_kappa(i), -KAPPA_LIMIT, KAPPA_LIMIT);
        float    current_cost = costAt(current_kappa);
        Vec4     best_kappa   = current_kappa;
        float    best_cost    = current_cost;
        float    lambda       = LM_LAMBDA_INIT;
        uint32_t iter;
        for (iter = 0; iter < MAX_ITER; ++iter)
        {
            DualVec4 kappa_dual{
                DecimalDual<float>(current_kappa(0)),
                DecimalDual<float>(current_kappa(1)),
                DecimalDual<float>(current_kappa(2)),
                DecimalDual<float>(current_kappa(3)),
            };
            // evaluate and calculate jacobian at kappa_dual
            DualVec6 residual_at_kappa;
            Mat64    jacobian_residual_at_kappa;
            autodiff::jacobian(
                residualVector, autodiff::wrt(kappa_dual), autodiff::at(kappa_dual), residual_at_kappa,
                jacobian_residual_at_kappa);
            const Vec6 residuals_at_kappa_primal{
                autodiff::val(residual_at_kappa(0)), autodiff::val(residual_at_kappa(1)),
                autodiff::val(residual_at_kappa(2)), autodiff::val(residual_at_kappa(3)),
                autodiff::val(residual_at_kappa(4)), autodiff::val(residual_at_kappa(5)),
            };
            const Mat44 normal_matrix = jacobian_residual_at_kappa.transpose() * jacobian_residual_at_kappa;
            const Vec4  rhs           = -jacobian_residual_at_kappa.transpose() * residuals_at_kappa_primal;

            const float cost_before    = current_cost;
            bool        accepted_step  = false;
            Vec4        accepted_delta = Vec4::Zero();

            // Increase damping until the nonlinear cost decreases. Larger lambda moves the update away from the
            // poorly conditioned Gauss-Newton direction and toward a shorter gradient-descent step.
            for (int attempt = 0; attempt < LM_MAX_TRIES; ++attempt)
            {
                Mat44 augmented_normal_matrix = normal_matrix;
                augmented_normal_matrix.diagonal().array() += lambda;
                const Eigen::LDLT<Mat44> ldlt(augmented_normal_matrix);
                if (ldlt.info() != Eigen::Success)
                {
                    lambda *= LM_LAMBDA_UP;
                    continue;
                }

                const Vec4 delta = ldlt.solve(rhs);
                if (!delta.allFinite())
                {
                    lambda *= LM_LAMBDA_UP;
                    continue;
                }

                Vec4 trial_kappa = current_kappa + delta;
                for (int i = 0; i < 4; ++i)
                    trial_kappa(i) = std::clamp(trial_kappa(i), -KAPPA_LIMIT, KAPPA_LIMIT);

                const float trial_cost = costAt(trial_kappa);
                if (trial_cost < current_cost)
                {
                    accepted_delta = trial_kappa - current_kappa;
                    current_kappa  = trial_kappa;
                    current_cost   = trial_cost;
                    lambda         = std::max(lambda * LM_LAMBDA_DOWN, NORMAL_MATRIX_EPS);
                    accepted_step  = true;
                    break;
                }

                lambda *= LM_LAMBDA_UP;
            }

            if (current_cost < best_cost)
            {
                best_cost  = current_cost;
                best_kappa = current_kappa;
            }

            if (!accepted_step || accepted_delta.norm() < STEP_TOLERANCE ||
                (cost_before - current_cost) < COST_TOLERANCE)
                break;
        }
        iterations = iter;
        return { best_kappa, best_cost };
    };

    uint32_t cold_iterations = 0, warm_iterations = 0;
    const auto [cold_kappa, cold_cost] = solveFrom(Vec4::Zero(), cold_iterations);
    const auto [warm_kappa, warm_cost] = solveFrom(previous_kappa, warm_iterations);
    const bool use_warm                = warm_cost <= cold_cost + COST_TOLERANCE;
    const Vec4 best_kappa              = use_warm ? warm_kappa : cold_kappa;
    previous_kappa                     = best_kappa;

    // Residuals at the chosen slips, reported in the debug struct.
    const DualVec6 residual_at_solution = residualVector({
        DecimalDual<float>(best_kappa(0)),
        DecimalDual<float>(best_kappa(1)),
        DecimalDual<float>(best_kappa(2)),
        DecimalDual<float>(best_kappa(3)),
    });
    const float    r_fx                 = autodiff::val(residual_at_solution(0));
    const float    r_mz                 = autodiff::val(residual_at_solution(1));
    tv_debug_data.optimizer             = {
                    .kappas         = { best_kappa(0), best_kappa(1), best_kappa(2), best_kappa(3) },
                    .r_fx           = r_fx,
                    .r_mz           = r_mz,
                    .fx_setpoint_N  = CAR_MASS_AT_CG_kg * ax_setpoint_mps2,
                    .mz_setpoint_Nm = mz_setpoint_Nm,
                    .fx_pred_N      = (r_fx / SQRT_W_FX + CAR_MASS_AT_CG_kg * ax_setpoint_mps2 / 1000) * 1000,
                    .mz_pred_Nm     = (r_mz / SQRT_W_MZ + mz_setpoint_Nm / 1000) * 1000,
    };

    return {
        .fl = best_kappa(0),
        .fr = best_kappa(1),
        .rl = best_kappa(2),
        .rr = best_kappa(3),
    };
}

} // namespace app::controls::allocation
