#pragma once

#include "controls/datatypes/decimal_dual.hpp"

namespace app::controls::reference::accel_request
{

template <Decimal T> struct AccelRequest
{
    T normalized;          // -1..1, fraction of MAX_AX_MPS2 (negative = regen)
    T regen_derate_factor; // 0..1 with speed: 0 below MIN_SPEED_REGEN_KMH, 1 from twice that speed
    T brake_derate_factor;
    T low_pedal_derate_factor;
};

/**
 * @brief Compute an acceleration request based on pedals and if regen is enabled.
 *
 * @note v_x_mps is only used to derate regen request as rules require no regen below 5 kmh.
 *
 * @param apps accelerator pedal, 0..1
 * @param brake brake pedal, 0..1
 * @param v_x_mps longitudinal vehicle body speed in m/s
 * @param regen_enabled
 *
 * @return Acceleration request, -1..1 of MAX_AX_MPS2, and the derate factors
 */
template <Decimal T> AccelRequest<T> compute(T apps, T brake, T v_x_mps, bool regen_enabled);
} // namespace app::controls::reference::accel_request