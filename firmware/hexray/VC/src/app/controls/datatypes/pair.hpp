#pragma once
#include "controls/datatypes/decimal_dual.hpp"

namespace app::controls
{
template <DecimalOrDual T> struct Pair
{
    T x;
    T y;
};
} // namespace app::controls

// {
//
// // inline float slipRatioToWheelAngularVelocity(const float slip_ratio, const float v_x_mps)
// // {
// //     // Avoid division by zero at very low speeds
// //     if (std::fabs(v_x_mps) < params::SMALL_EPSILON)
// //         return 0.0f;
// //     return (1.0f + slip_ratio) * (v_x_mps / params::WHEEL_RADIUS_M);
// // }
//

//
// inline float calculateSlipRatio(const float omega, const float vx)
// {
//     return (vx - omega * params::WHEEL_RADIUS_M) / safe_vx(vx);
// }
// } // namespace app::controls
