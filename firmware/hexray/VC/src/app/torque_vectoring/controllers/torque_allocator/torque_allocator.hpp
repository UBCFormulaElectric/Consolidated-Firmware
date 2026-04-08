#pragma once

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"
#include <dual.hpp>

namespace app::tv::controllers::allocator
{

/**
 * Torque allocator using Gauss-Newton optimization.
 *
 * Minimizes a weighted cost function over per-wheel slip ratios:
 *   J(kappa) = W_FX * sum(Fx_i(kappa_i) - des_fx_i)^2
 *            + W_MZ * (Mz(kappa) - des_Mz)^2
 *
 * Outputs per-wheel optimal slip ratios using the caller-provided per-wheel tire models.
 */
class TorqueAllocator
{
public:
    /**
     * Run Gauss-Newton iterations to find optimal slip ratios.
     *
     * @param tire_models         Per-wheel tire models used to evaluate candidate slip ratios
     * @param des_f_x             Target per-wheel longitudinal force (pedal request + yaw split)
     * @param normal_forces_N     Per-wheel vertical load from dynamics estimator
     * @param current_slip_ratios Warm start from current cycle estimation
     * @param current_slip_angles Fixed per-wheel slip angles for this optimization tick
     * @param low_speed_blend     Precomputed low-speed force-availability blend in [0, 1]
     * @param steering_angle_rad  Steering angle used to rotate front-wheel forces into the body frame
     * @param des_M_z             Target yaw moment from high-level controller (Nm)
     * @return Per-wheel optimal slip ratios
     */
    [[nodiscard]] shared_datatypes::datatypes::wheel_set<float> optimize(
        const shared_datatypes::datatypes::wheel_set<estimation::TireModel>& tire_models,
        const shared_datatypes::datatypes::wheel_set<float>& des_f_x,
        const shared_datatypes::datatypes::wheel_set<float>& normal_forces_N,
        const shared_datatypes::datatypes::wheel_set<float>& current_slip_ratios,
        const shared_datatypes::datatypes::wheel_set<float>& current_slip_angles,
        float low_speed_blend,
        float steering_angle_rad,
        float des_M_z);

};

} // namespace app::tv::controllers::allocator
