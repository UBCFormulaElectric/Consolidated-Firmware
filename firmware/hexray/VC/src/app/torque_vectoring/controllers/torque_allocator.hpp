#pragma once

#include "torque_vectoring/shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/shared_datatypes/wheel_set.hpp"

namespace app::tv::controllers
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
namespace allocator
{
    /**
     * Run Gauss-Newton iterations to find optimal slip ratios.
     * @return Per-wheel optimal slip ratios
     */
    template <Decimal T>
    [[nodiscard]] shared_datatypes::wheel_set<T>
        optimize(const shared_datatypes::VehicleState<T> &state, T ax_setpoint, T omegadot_setpoint);
}; // namespace allocator
} // namespace app::tv::controllers
