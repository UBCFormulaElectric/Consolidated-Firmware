#pragma once

#include <concepts>

#include "dual.hpp"

// Floating-point scalar types that the autodiff-based code supports.
template <typename T>
concept Decimal = std::same_as<T, float> || std::same_as<T, double>;

// First-order forward-mode dual number with value and derivative both of type T.
// autodiff::dual is fixed to double; use this to keep float code in float.
template <Decimal T> using DecimalDual = autodiff::HigherOrderDual<1, T>;
