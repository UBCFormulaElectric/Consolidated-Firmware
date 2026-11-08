#pragma once

#include "controls/datatypes/vehicle_state.hpp"
#include "controls/datatypes/wheel_set.hpp"

/**
 * Control allocation: shares the requested total Fx and yaw moment over the four wheels, using Gauss-Newton
 * optimization.
 *
 * Minimizes a weighted cost function over per-wheel slip ratios:
 *   J(kappa) = W_FX * sum(Fx_i(kappa_i) - des_fx_i)^2
 *            + W_MZ * (Mz(kappa) - des_Mz)^2
 *
 * Outputs per-wheel optimal slip ratios using the caller-provided per-wheel tire models.
 */
namespace app::controls::allocation
{
/**
 * Run Gauss-Newton iterations to find optimal slip ratios.
 * @param state current vehicle state
 * @param ax_setpoint_mps2 requested longitudinal acceleration in m/s^2
 * @param mz_setpoint_Nm requested yaw moment in Nm (ISO 8855, positive = left), from the yaw rate controller
 * @return Per-wheel optimal slip ratios
 */
template <Decimal T>
[[nodiscard]] wheel_set<T> optimize(const VehicleState<T> &state, T ax_setpoint_mps2, T mz_setpoint_Nm);
} // namespace app::controls::allocation
