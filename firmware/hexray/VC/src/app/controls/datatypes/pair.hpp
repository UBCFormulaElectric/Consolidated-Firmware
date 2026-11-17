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
// // inline float slipRatioToWheelAngularVelocity(const float slip_ratio, const float vx_mps)
// // {
// //     // Avoid division by zero at very low speeds
// //     if (std::fabs(vx_mps) < params::SMALL_EPSILON)
// //         return 0.0f;
// //     return (1.0f + slip_ratio) * (vx_mps / params::WHEEL_RADIUS_m);
// // }
//

//
// inline float calculateSlipRatio(const float omega, const float vx)
// {
//     return (vx - omega * params::WHEEL_RADIUS_m) / safeVx(vx);
// }
// } // namespace app::controls
