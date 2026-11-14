#pragma once

#include "io_math.hpp"
#include "dual.hpp"

namespace io::math::fast
{
// First-order dual number with value and derivative of the same type. Named directly (not through an alias template
// such as DecimalDual<T>), because templates cannot deduce T through that kind of alias.
template <Decimal T> using Dual = autodiff::detail::Dual<T, T>;

template <Decimal T> [[nodiscard]] inline Dual<T> sin(const Dual<T> &x)
{
    Dual<T> result;
    result.val  = fast::sin(x.val);
    result.grad = fast::cos(x.val) * x.grad;
    return result;
}

template <Decimal T> [[nodiscard]] inline Dual<T> cos(const Dual<T> &x)
{
    Dual<T> result;
    result.val  = fast::cos(x.val);
    result.grad = -fast::sin(x.val) * x.grad;
    return result;
}

template <Decimal T> [[nodiscard]] inline Dual<T> sinatan(const Dual<T> &x)
{
    Dual<T> result;
    T       denom = T(1.0) / std::sqrt(T(1.0) + (x.val * x.val));
    result.val    = x.val * denom;
    result.grad   = x.grad * (denom * denom * denom);
    return result;
}

template <Decimal T> [[nodiscard]] inline Dual<T> cosatan(const Dual<T> &x)
{
    Dual<T> result;
    T       denom = T(1.0) / std::sqrt(T(1.0) + (x.val * x.val));
    result.val    = cosatan(x.val);
    result.grad   = -x.val * x.grad * (result.val * result.val * result.val);
    return result;
}

} // namespace io::math::fast
