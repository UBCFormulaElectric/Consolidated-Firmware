#pragma once

#include <cmath>

#include "app_canUtils.hpp"
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
struct OptimizerWeights
{
    float sqrt_w_fx;
    float sqrt_w_mz;
    float sqrt_w_r;

    OptimizerWeights(const float w_fx, const float w_mz, const float w_r)
      : sqrt_w_fx(std::sqrt(w_fx)), sqrt_w_mz(std::sqrt(w_mz)), sqrt_w_r(std::sqrt(w_r))
    {
    }
};

[[nodiscard]] const OptimizerWeights &weightsFor(can_utils::DriveMode drive_mode);

/**
 * Run Gauss-Newton iterations to find optimal slip ratios.
 * @param state current vehicle state
 * @param ax_setpoint_mps2 requested longitudinal acceleration in m/s^2
 * @param mz_setpoint_Nm requested yaw moment in Nm (ISO 8855, positive = left), from the yaw rate controller
 * @return Per-wheel optimal slip ratios
 */
[[nodiscard]] wheel_set<float> optimize(
    const VehicleState     &state,
    float                   ax_setpoint_mps2,
    float                   mz_setpoint_Nm,
    const OptimizerWeights &weights);
} // namespace app::controls::allocation
