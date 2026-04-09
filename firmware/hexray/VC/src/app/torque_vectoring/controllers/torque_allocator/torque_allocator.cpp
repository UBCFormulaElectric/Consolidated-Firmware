#include "torque_allocator.hpp"

#include <algorithm>
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
    constexpr float W_FX              = 2.0f / 3.0f;
    constexpr float W_MZ              = 1.0f / 3.0f;
    constexpr int   MAX_ITER          = 8;
    constexpr float SLIP_CLAMP        = 0.3f;
    constexpr float NORMAL_MATRIX_EPS = 1e-6f;
    constexpr float STEP_TOLERANCE    = 1e-5f;
    constexpr float COST_TOLERANCE    = 1e-6f;

    using Vec4f    = Eigen::Matrix<float, 4, 1>;
    using Vec5f    = Eigen::Matrix<float, 5, 1>;
    using Mat54f   = Eigen::Matrix<float, 5, 4>;
    using Mat44f   = Eigen::Matrix<float, 4, 4>;
    using DualVec4 = Eigen::Matrix<autodiff::dual, 4, 1>;
    using DualVec5 = Eigen::Matrix<autodiff::dual, 5, 1>;

    // template <typename T>
    // [[nodiscard]] T yawMomentFromTireForces(
    //     const shared_datatypes::wheel_set<T> &f_x,
    //     const shared_datatypes::wheel_set<T> &f_y,
    //     const float                           steering_angle_rad)
    // {
    //     constexpr float half_track_m = TRACK_WIDTH_m * 0.5f;
    //
    //     const float cos_delta = std::cos(steering_angle_rad);
    //     const float sin_delta = std::sin(steering_angle_rad);
    //
    //     const T fl_fx_body = (T(cos_delta) * f_x.fl) - (T(sin_delta) * f_y.fl);
    //     const T fl_fy_body = (T(sin_delta) * f_x.fl) + (T(cos_delta) * f_y.fl);
    //     const T fr_fx_body = (T(cos_delta) * f_x.fr) - (T(sin_delta) * f_y.fr);
    //     const T fr_fy_body = (T(sin_delta) * f_x.fr) + (T(cos_delta) * f_y.fr);
    //     const T fl_moment  = (T(DIST_FRONT_AXLE_CG_m) * fl_fy_body) - (T(half_track_m) * fl_fx_body);
    //     const T fr_moment  = (T(DIST_FRONT_AXLE_CG_m) * fr_fy_body) + (T(half_track_m) * fr_fx_body);
    //     const T rl_moment  = (T(-DIST_REAR_AXLE_CG_m) * f_y.rl) - (T(half_track_m) * f_x.rl);
    //     const T rr_moment  = (T(-DIST_REAR_AXLE_CG_m) * f_y.rr) + (T(half_track_m) * f_x.rr);
    //     return fl_moment + fr_moment + rl_moment + rr_moment;
    // }
} // namespace

[[nodiscard]] wheel_set<float> optimize(const VehicleState &state, float ax_setpoint, float omegadot_setpoint)
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

    // const wheel_set<float> blended_des_f_x{
    //     .fl = low_speed_blend * des_f_x.fl,
    //     .fr = low_speed_blend * des_f_x.fr,
    //     .rl = low_speed_blend * des_f_x.rl,
    //     .rr = low_speed_blend * des_f_x.rr,
    // };
    // const float blended_des_m_z = low_speed_blend * des_M_z;

    Vec4f opt_slip; // output variable
    const auto [kappa_fl, kappa_fr, kappa_rl, kappa_rr] = state.kappas();
    opt_slip << kappa_fl, kappa_fr, kappa_rl, kappa_rr;

    // const float sqrt_w_fx = std::sqrt(W_FX);
    // const float sqrt_w_mz = std::sqrt(W_MZ);

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
    // We keep all vectors/matrices fixed-size (4 decision variables, 5 residuals) so the optimizer
    // stays allocation-free and predictable on embedded targets.
    const auto residualVector = [&](const DualVec4 &kappa) -> DualVec5
    {
        const auto [fz_fl, fz_fr, fz_rl, fz_rr] = state.est_Fz_N();
        const auto alphas                       = state.alphas();

        const wheel_set<autodiff::dual> predicted_fx{
            .fl = estimation::tire_models.fl.computeCombinedFx_N<autodiff::dual>(
                fz_fl, current_slip_angles.fl, low_speed_blend, kappa(0)),
            .fr = estimation::tire_models.fr.computeCombinedFx_N<autodiff::dual>(
                fz_fr, current_slip_angles.fr, low_speed_blend, kappa(1)),
            .rl = estimation::tire_models.rl.computeCombinedFx_N<autodiff::dual>(
                fz_rl, current_slip_angles.rl, low_speed_blend, kappa(2)),
            .rr = estimation::tire_models.rr.computeCombinedFx_N<autodiff::dual>(
                fz_rr, current_slip_angles.rr, low_speed_blend, kappa(3)),
        };

        const wheel_set<autodiff::dual> predicted_fy{
            .fl = estimation::tire_models.fl.computeCombinedFy_N<autodiff::dual>(
                fz_fl, current_slip_angles.fl, low_speed_blend, kappa(0)),
            .fr = estimation::tire_models.fr.computeCombinedFy_N<autodiff::dual>(
                fz_fr, current_slip_angles.fr, low_speed_blend, kappa(1)),
            .rl = estimation::tire_models.rl.computeCombinedFy_N<autodiff::dual>(
                fz_rl, current_slip_angles.rl, low_speed_blend, kappa(2)),
            .rr = estimation::tire_models.rr.computeCombinedFy_N<autodiff::dual>(
                fz_rr, current_slip_angles.rr, low_speed_blend, kappa(3)),
        };
        const autodiff::dual predicted_mz = yawMomentFromTireForces(predicted_fx, predicted_fy, steering_angle_rad);

        DualVec5 residuals;
        residuals(0) = autodiff::dual(sqrt_w_fx) * (predicted_fx.fl - blended_des_f_x.fl);
        residuals(1) = autodiff::dual(sqrt_w_fx) * (predicted_fx.fr - blended_des_f_x.fr);
        residuals(2) = autodiff::dual(sqrt_w_fx) * (predicted_fx.rl - blended_des_f_x.rl);
        residuals(3) = autodiff::dual(sqrt_w_fx) * (predicted_fx.rr - blended_des_f_x.rr);
        residuals(4) = autodiff::dual(sqrt_w_mz) * (predicted_mz - blended_des_m_z);
        return residuals;
    };

    float previous_cost = std::numeric_limits<float>::infinity();

    for (int iter = 0; iter < MAX_ITER; ++iter)
    {
        DualVec4 kappa_dual;
        for (int i = 0; i < 4; ++i)
            kappa_dual(i) = autodiff::dual(opt_slip(i));

        DualVec5 residual_eval_dual;
        Mat54f   jacobian;
        autodiff::jacobian(
            residualVector, autodiff::wrt(kappa_dual), autodiff::at(kappa_dual), residual_eval_dual, jacobian);

        Vec5f residuals;
        for (int i = 0; i < 5; ++i)
            residuals(i) = static_cast<float>(autodiff::val(residual_eval_dual(i)));

        // Gauss-Newton solves:
        //   (J^T J) * delta = -J^T r
        // where r is the residual vector and J is dr/dkappa at the current trial slip.
        //
        // J comes directly from autodiff::jacobian(...), so we do not hand-derive per-wheel slopes
        // or yaw-moment sensitivities. The optimizer stays readable: define residuals first, then
        // let autodiff provide the linearization used by Gauss-Newton.
        Mat44f normal_matrix = jacobian.transpose() * jacobian;
        normal_matrix.diagonal().array() += NORMAL_MATRIX_EPS;

        const Vec4f         rhs = -jacobian.transpose() * residuals;
        Eigen::LDLT<Mat44f> ldlt(normal_matrix);

        if (ldlt.info() != Eigen::Success)
            break;

        const Vec4f delta = ldlt.solve(rhs);
        if (!delta.allFinite())
            break;

        opt_slip += delta;
        for (int i = 0; i < 4; ++i)
            opt_slip(i) = std::clamp(opt_slip(i), -SLIP_CLAMP, SLIP_CLAMP);

        // Least-squares cost:
        //   J = 0.5 * r^T r
        // This is used only for convergence monitoring; the actual update is driven by J^T J and J^T r above.
        const float cost = 0.5f * residuals.squaredNorm();
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
