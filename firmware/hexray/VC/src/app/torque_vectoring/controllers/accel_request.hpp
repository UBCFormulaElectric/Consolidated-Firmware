#pragma once

#include "../shared_datatypes/decimal_dual.hpp"

namespace app::tv::controllers::accel_request
{

template <Decimal T> struct AccelRequest
{
    T percentage;
    T regen_derate_factor;
    T brake_derate_factor;
    T low_pedal_derate_factor;
};

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
template <Decimal T> AccelRequest<T> compute(T apps, T brakes, T v_x_mps, bool regen_enabled);
} // namespace app::tv::controllers::accel_request