#pragma once

#include "io_math.hpp"
#include "dual.hpp"
#include <utility>

/**
 * Note:
 *
 * In order to use this you need CORDIC to be enabled on your MCU. If it is disabled
 * This will not work. Currently can only be used in a single task. If using mathDual,
 * You can include just "io_mathDual.hpp" and not have to include "io_math.hpp".
 */
namespace io::math::fast
{
// A Dual or an unevaluated autodiff expression. The expression is evaluated into a Dual,
// then the value and derivative are computed with the fast float functions.
template <typename R>
concept DualExpr = autodiff::detail::isExpr<R>;

template <DualExpr R> [[nodiscard]] inline auto sin(R &&expr)
{
    auto       x   = autodiff::detail::eval(std::forward<R>(expr));
    const auto val = x.val;
    x.val          = fast::sin(val);
    x.grad *= fast::cos(val);
    return x;
}

template <DualExpr R> [[nodiscard]] inline auto cos(R &&expr)
{
    auto       x   = autodiff::detail::eval(std::forward<R>(expr));
    const auto val = x.val;
    x.val          = fast::cos(val);
    x.grad *= -fast::sin(val);
    return x;
}

template <DualExpr R> [[nodiscard]] inline auto atan(R &&expr)
{
    auto       x   = autodiff::detail::eval(std::forward<R>(expr));
    const auto val = x.val;
    x.val          = fast::atan(val);
    x.grad /= (1.0f + (val * val));
    return x;
}

template <DualExpr R> [[nodiscard]] inline auto sinatan(R &&expr)
{
    auto       x     = autodiff::detail::eval(std::forward<R>(expr));
    const auto denom = 1.0f / std::sqrt(1.0f + (x.val * x.val));
    x.val *= denom;
    x.grad *= denom * denom * denom;
    return x;
}

template <DualExpr R> [[nodiscard]] inline auto cosatan(R &&expr)
{
    auto       x     = autodiff::detail::eval(std::forward<R>(expr));
    const auto denom = 1.0f / std::sqrt(1.0f + (x.val * x.val));
    x.grad *= -x.val * (denom * denom * denom);
    x.val = denom;
    return x;
}

} // namespace io::math::fast
