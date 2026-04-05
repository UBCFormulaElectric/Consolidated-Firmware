#pragma once

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"

namespace app::tv::controllers::allocator
{

/**
 * Torque allocator using Gauss-Newton optimization.
 *
 * Minimizes a weighted cost function over per-wheel slip ratios:
 *   J(kappa) = W_FX * sum(Fx_i(kappa_i) - des_fx_i)^2
 *            + W_MZ * (Mz(kappa) - des_Mz)^2
 *
 * Outputs per-wheel optimal slip ratios. Owns internal tire model
 * instances separate from the estimation models to avoid state contamination.
 */
class TorqueAllocator
{
public:
    explicit TorqueAllocator(
        estimation::TireModel::TirePressure pressure = estimation::TireModel::TirePressure::PSI_12);

    /**
     * Run Gauss-Newton iterations to find optimal slip ratios.
     *
     * @param des_f_x             Target per-wheel longitudinal force (pedal request + yaw moment)
     * @param normal_forces_N     Per-wheel vertical load from dynamics estimator
     * @param current_slip_ratios Warm start from current cycle estimation
     * @param vehicle_state       Vehicle-level state (fixed during optimization)
     * @param acc_M_z             Current yaw moment estimate supplied by the caller (Nm)
     * @param des_M_z             Target yaw moment from high-level controller (Nm)
     * @return Per-wheel optimal slip ratios
     */
    [[nodiscard]] datatypes::datatypes::wheel_set optimize(
        const datatypes::datatypes::wheel_set& des_f_x,
        const datatypes::datatypes::wheel_set& normal_forces_N,
        const datatypes::datatypes::wheel_set& current_slip_ratios,
        const datatypes::datatypes::VehicleState& vehicle_state,
        float acc_M_z,
        float des_M_z);

    /**
     * Estimate yaw moment from longitudinal wheel forces.
     * Mz = (track_width / 2) * ((Fx_fr + Fx_rr) - (Fx_fl + Fx_rl))
     * Temporary fallback for callers until a dedicated yaw-moment estimator is wired in.
     */
    [[nodiscard]] static float estimateYawMoment(const datatypes::datatypes::wheel_set& f_x);

private:
    estimation::TireModel fl_tire_;
    estimation::TireModel fr_tire_;
    estimation::TireModel rl_tire_;
    estimation::TireModel rr_tire_;
};

} // namespace app::tv::controllers::allocator
