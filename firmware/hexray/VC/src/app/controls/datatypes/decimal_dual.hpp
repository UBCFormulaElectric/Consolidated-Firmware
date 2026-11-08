#pragma once

#include "util_decimal_dual.hpp" // Decimal, DecimalDual

// The controls code runs in float (the Simulink wrapper converts its double signals at the boundary). Templates are
// only used where a function also runs with dual numbers for the optimizer / EKF Jacobians.
template <typename T>
concept DecimalOrDual = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, DecimalDual<float>> ||
                        std::same_as<T, DecimalDual<double>>;
