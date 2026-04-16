#include "torque_allocator.hpp"

#include <cmath>
#include <limits>

#include <Eigen/Dense>
#include <dual.hpp>
#include <gradient.hpp>
#include <cstdio>

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"

#include <iostream>
using namespace app::tv::shared_datatypes;
using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::allocator
{
namespace
{
    // ---- Optimizer tuning ----
    constexpr double W_FX = 0.5f;
    constexpr double W_MZ = 0.0f;
    constexpr double W_R  = 15.5f;

    constexpr int                     MAX_ITER          = 8;
    [[maybe_unused]] constexpr double SLIP_CLAMP        = 0.3;
    constexpr float                   NORMAL_MATRIX_EPS = 1e-6f;
    constexpr float                   STEP_TOLERANCE    = 1e-5f;
    constexpr float                   COST_TOLERANCE    = 1e-6f;

    using Vec6f    = Eigen::Matrix<double, 6, 1>;
    using Vec4f    = Eigen::Matrix<double, 4, 1>;
    using Mat64f   = Eigen::Matrix<double, 6, 4>;
    using Mat44f   = Eigen::Matrix<double, 4, 4>;
    using DualVec6 = Eigen::Matrix<autodiff::dual, 6, 1>;
    using DualVec4 = Eigen::Matrix<autodiff::dual, 4, 1>;

    // print the normal_matrix
    const Eigen::IOFormat CleanFmt(3, 0, ", ", "\n", "[", "]");

    void debug(const Mat44f &normal_matrix)
    {
        std::cout << "Normal Matrix:\n" << normal_matrix.format(CleanFmt) << std::endl;
        // eigenvalues
        const Eigen::EigenSolver<Mat44f> es(normal_matrix);
        std::cout << "Eigenvalues:\n" << es.eigenvalues() << std::endl;
        // condition number
        const Eigen::JacobiSVD<Mat44f> svd(normal_matrix);
        const double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size() - 1);
        std::cout << "Condition number: " << cond << std::endl;
    }
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

    std::cout << ax_setpoint << " " << omegadot_setpoint << std::endl;

    static const float SQRT_W_FX = std::sqrt(W_FX);
    static const float SQRT_W_MZ = std::sqrt(W_MZ);
    static const float SQRT_W_R  = std::sqrt(W_R);

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
    const auto residualVector                           = [&](const DualVec4 &kappa) -> DualVec6
    {
        const wheel_set<Pair<autodiff::dual>> predicted_f{
            {
                estimation::tire_model.computeCombinedFx_N<autodiff::dual>(fz_fl, alpha_fl, kappa(0)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_fl, alpha_fl, kappa(0)),
            },
            {
                estimation::tire_model.computeCombinedFx_N<autodiff::dual>(fz_fr, alpha_fr, kappa(1)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_fr, alpha_fr, kappa(1)),
            },
            {
                estimation::tire_model.computeCombinedFx_N<autodiff::dual>(fz_rl, alpha_rl, kappa(2)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_rl, alpha_rl, kappa(2)),
            },
            {
                estimation::tire_model.computeCombinedFx_N<autodiff::dual>(fz_rr, alpha_rr, kappa(3)),
                estimation::tire_model.computeCombinedFy_N<autodiff::dual>(fz_rr, alpha_rr, kappa(3)),
            },
        };
        const autodiff::dual sum_fx       = predicted_f.fl.x + predicted_f.fr.x + predicted_f.rl.x + predicted_f.rr.x;
        const autodiff::dual predicted_mz = state.est_Mz_N(predicted_f);
        // print out all forces and their gradients
        // std::cout << "Predicted forces at kappa:\n"
        //           << "FL: " << predicted_f.fl.x << " N, dFL/dkappa: " << autodiff::derivative(predicted_f.fl.x)
        //           << "\nFR: " << predicted_f.fr.x << " N, dFR/dkappa: " << autodiff::derivative(predicted_f.fr.x)
        //           << "\nRL: " << predicted_f.rl.x << " N, dRL/dkappa: " << autodiff::derivative(predicted_f.rl.x)
        //           << "\nRR: " << predicted_f.rr.x << " N, dRR/dkappa: " << autodiff::derivative(predicted_f.rr.x)
        //           << std::endl;
        // print out sum_fx and its gradient
        // std::cout << "Sum Fx: " << sum_fx << " N, d(Sum Fx)/dkappa: " << autodiff::derivative(sum_fx) << std::endl;
        return DualVec6{ SQRT_W_FX * (sum_fx - CAR_MASS_AT_CG_KG * ax_setpoint),
                         SQRT_W_MZ * (predicted_mz - CAR_YAW_MOMENT_INERTIA_KGM2 * omegadot_setpoint),
                         SQRT_W_R * kappa[0],
                         SQRT_W_R * kappa[1],
                         SQRT_W_R * kappa[2],
                         SQRT_W_R * kappa[3] };
    };

    Vec4f opt_slip{ 0, 0, 0, 0 }; // output variable

    float previous_cost = std::numeric_limits<float>::infinity();

    uint32_t iter;
    for (iter = 0; iter < MAX_ITER; ++iter)
    {
        DualVec4 kappa{
            autodiff::dual(opt_slip(0)),
            autodiff::dual(opt_slip(1)),
            autodiff::dual(opt_slip(2)),
            autodiff::dual(opt_slip(3)),
        };
        // evaluate and calculate jacobian at kappa
        DualVec6 residual_at_kappa;
        Mat64f   jacobian_residual_at_kappa;
        autodiff::jacobian(
            residualVector, autodiff::wrt(kappa), autodiff::at(kappa), residual_at_kappa, jacobian_residual_at_kappa);
        const Vec6f residuals_at_kappa_primal{
            autodiff::val(residual_at_kappa(0)), autodiff::val(residual_at_kappa(1)),
            autodiff::val(residual_at_kappa(2)), autodiff::val(residual_at_kappa(3)),
            autodiff::val(residual_at_kappa(4)), autodiff::val(residual_at_kappa(5)),
        };
        Mat44f normal_matrix = jacobian_residual_at_kappa.transpose() * jacobian_residual_at_kappa;
        normal_matrix.diagonal().array() += NORMAL_MATRIX_EPS; // condition problem lmao
        const Eigen::LDLT<Mat44f> ldlt(normal_matrix);
        if (ldlt.info() != Eigen::Success)
        {
            std::cout << "optimizer :( 1\n" << ldlt.info() << std::endl;
            debug(normal_matrix);
            break;
        }
        const Vec4f rhs   = -jacobian_residual_at_kappa.transpose() * residuals_at_kappa_primal;
        const Vec4f delta = ldlt.solve(rhs);
        if (!delta.allFinite())
        {
            std::printf("optimizer :( 2\n");
            std::fflush(stdout);
            break;
        }

        Vec4f next_opt_slip = opt_slip + delta;
        for (int i = 0; i < 4; ++i)
            next_opt_slip(i) = std::clamp(next_opt_slip(i), -SLIP_CLAMP, SLIP_CLAMP);
        // Least-squares cost: |r|_2^2
        // This is used only for convergence monitoring; the actual update is driven by J^T J and J^T r above.
        const float cost = residuals_at_kappa_primal.squaredNorm();
        std::cout << "Iter " << iter << ": cost = " << cost << ", opt_slip = [" << opt_slip.transpose()
                  << "], delta = [" << delta.transpose() << "]\n";
        if (delta.norm() < STEP_TOLERANCE || std::fabs(previous_cost - cost) < COST_TOLERANCE)
            break;

        previous_cost = cost;
        opt_slip      = next_opt_slip;
    }
    if (iter == MAX_ITER)
    {
        std::cout << "Reached max iterations without convergence.\n" << std::endl;
    }

    return {
        .fl = opt_slip(0),
        .fr = opt_slip(1),
        .rl = opt_slip(2),
        .rr = opt_slip(3),
    };
}

// template wheel_set<float>  optimize(const VehicleState<float> &state, float ax_setpoint, float omegadot_setpoint);
template wheel_set<double> optimize(const VehicleState<double> &state, double ax_setpoint, double omegadot_setpoint);
} // namespace app::tv::controllers::allocator
