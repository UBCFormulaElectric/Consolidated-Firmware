#include "torque_allocator.hpp"

#include <cmath>
#include <limits>

#include <Eigen/Dense>
#include <dual.hpp>
#include <gradient.hpp>

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"
using namespace app::tv::shared_datatypes;
using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::allocator
{
namespace
{
    // ---- Optimizer tuning ----
    constexpr float W_FX = 2.0f / 3.0f;
    constexpr float W_MZ = 1.0f / 3.0f;

    constexpr int                    MAX_ITER          = 8;
    [[maybe_unused]] constexpr float SLIP_CLAMP        = 0.3f;
    constexpr float                  NORMAL_MATRIX_EPS = 1e-6f;
    constexpr float                  STEP_TOLERANCE    = 1e-5f;
    constexpr float                  COST_TOLERANCE    = 1e-6f;

    using Vec2f    = Eigen::Matrix<float, 2, 1>;
    using Vec4f    = Eigen::Matrix<float, 4, 1>;
    using Mat24f   = Eigen::Matrix<float, 2, 4>;
    using Mat44f   = Eigen::Matrix<float, 4, 4>;
    using DualVec2 = Eigen::Matrix<autodiff::dual, 2, 1>;
    using DualVec4 = Eigen::Matrix<autodiff::dual, 4, 1>;
} // namespace

[[nodiscard]] wheel_set<float>
    optimize(const VehicleState &state, const float ax_setpoint, const float omegadot_setpoint)
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

    static constexpr float SQRT_W_FX = std::sqrt(W_FX);
    static constexpr float SQRT_W_MZ = std::sqrt(W_MZ);

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
    const auto residualVector = [&](const DualVec4 &kappa) -> DualVec2
    {
        const auto [fz_fl, fz_fr, fz_rl, fz_rr]             = state.est_Fz_N();
        const auto [alpha_fl, alpha_fr, alpha_rl, alpha_rr] = state.alphas();

        const wheel_set<Pair<autodiff::dual>> predicted_f{
            {
                estimation::tire_model.computeCombinedFx_N(fz_fl, alpha_fl, kappa(0)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_fl, alpha_fl, kappa(0)),
            },
            {
                estimation::tire_model.computeCombinedFx_N(fz_fr, alpha_fr, kappa(1)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_fr, alpha_fr, kappa(1)),
            },
            {
                estimation::tire_model.computeCombinedFx_N(fz_rl, alpha_rl, kappa(2)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_rl, alpha_rl, kappa(2)),
            },
            {
                estimation::tire_model.computeCombinedFx_N(fz_rr, alpha_rr, kappa(3)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_rr, alpha_rr, kappa(3)),
            },
        };
        const autodiff::dual sum_fx       = predicted_f.fl.x + predicted_f.fr.x + predicted_f.rl.x + predicted_f.rr.x;
        const autodiff::dual predicted_mz = state.est_Mz_N(predicted_f);
        return DualVec2{
            SQRT_W_FX * (sum_fx - CAR_MASS_AT_CG_KG * ax_setpoint),
            SQRT_W_MZ * (predicted_mz - CAR_YAW_MOMENT_INERTIA_KGM2 * omegadot_setpoint),
        };
    };

    Vec4f opt_slip; // output variable
    // seed opt_slip
    const auto [kappa_fl, kappa_fr, kappa_rl, kappa_rr] = state.kappas();
    opt_slip << kappa_fl, kappa_fr, kappa_rl, kappa_rr;

    float previous_cost = std::numeric_limits<float>::infinity();

    for (uint32_t iter = 0; iter < MAX_ITER; ++iter)
    {
        DualVec4 kappa{
            autodiff::dual(opt_slip(0)),
            autodiff::dual(opt_slip(1)),
            autodiff::dual(opt_slip(2)),
            autodiff::dual(opt_slip(3)),
        };
        // evaluate and calculate jacobian at kappa
        DualVec2 residual_at_kappa;
        Mat24f   jacobian_residual_at_kappa;
        autodiff::jacobian(
            residualVector, autodiff::wrt(kappa), autodiff::at(kappa), residual_at_kappa, jacobian_residual_at_kappa);

        // Gauss-Newton solves:
        //   (J^T J) * delta = -J^T r
        // where r is the residual vector and J is dr/dkappa at the current trial slip.
        //
        // J comes directly from autodiff::jacobian(...), so we do not hand-derive per-wheel slopes
        // or yaw-moment sensitivities. The optimizer stays readable: define residuals first, then
        // let autodiff provide the linearization used by Gauss-Newton.
        Mat44f normal_matrix = jacobian_residual_at_kappa.transpose() * jacobian_residual_at_kappa;
        normal_matrix.diagonal().array() += NORMAL_MATRIX_EPS;

        const Vec2f residuals_at_kappa_primal{
            static_cast<float>(autodiff::val(residual_at_kappa(0))),
            static_cast<float>(autodiff::val(residual_at_kappa(1))),
        };
        const Vec4f               rhs = -jacobian_residual_at_kappa.transpose() * residuals_at_kappa_primal;
        const Eigen::LDLT<Mat44f> ldlt(normal_matrix);

        if (ldlt.info() != Eigen::Success)
            break;

        const Vec4f delta = ldlt.solve(rhs);
        if (!delta.allFinite())
            break;

        opt_slip += delta;
        // I would recommend not doing this
        // consider the following: a large step is required which leaves the clamp space
        // this would cause the loop to never converge as the step size at each iteration would be sufficiently large to
        // continue
        // for (int i = 0; i < 4; ++i)
        //     opt_slip(i) = std::clamp(opt_slip(i), -SLIP_CLAMP, SLIP_CLAMP);

        // Least-squares cost:
        //   J = 0.5 * r^T r
        // This is used only for convergence monitoring; the actual update is driven by J^T J and J^T r above.
        const float cost = 0.5f * residuals_at_kappa_primal.squaredNorm();
        if (delta.norm() < STEP_TOLERANCE || std::fabs(previous_cost - cost) < COST_TOLERANCE)
            break;

        previous_cost = cost;
    }

    return {
        .fl = opt_slip(0),
        .fr = opt_slip(1),
        .rl = opt_slip(2),
        .rr = opt_slip(3),
    };
}
} // namespace app::tv::controllers::allocator
