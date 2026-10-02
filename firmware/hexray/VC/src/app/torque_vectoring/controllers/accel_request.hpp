#pragma once

#include "../shared_datatypes/decimal_dual.hpp"

namespace app::tv::controllers::accel_request
{
/**
 * @brief Compute an acceleration request based on pedals and if regen is enabled.
 *
 * @note v_x_mps is only used to derate regen request as rules require no regen below 5 kmh.
 *
 * @param apps pedal percentage
 * @param brakes pedal percentage
 * @param v_x_mps longitudinal vehicle body speed
 * @param regen_enabled
 *
 * @return Acceleration request in mps^2
 */
template <Decimal T> T compute_accel_request(T apps, T brakes, T v_x_mps, bool regen_enabled);
} // namespace accel_request