#pragma once

#include "torque_vectoring/shared_datatypes/datatypes.hpp"

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
namespace TorqueAllocator
{
    /**
     * Run Gauss-Newton iterations to find optimal slip ratios.
     * @return Per-wheel optimal slip ratios
     */
    [[nodiscard]] shared_datatypes::wheel_set<float>
        optimize(const shared_datatypes::VehicleState &state, float ax_setpoint, float omegadot_setpoint);

}; // namespace TorqueAllocator
} // namespace app::tv::controllers
