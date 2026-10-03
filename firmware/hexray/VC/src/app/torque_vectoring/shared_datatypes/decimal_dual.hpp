#pragma once

#include "dual.hpp"

template <typename T>
concept Decimal = std::same_as<T, float> || std::same_as<T, double>;

template <Decimal T> using DecimalDual = autodiff::HigherOrderDual<1, T>;

// Precision the torque-vectoring code is compiled for. Every template is instantiated for this one type only, so a
// build contains either the float or the double version, never both. Firmware uses float; define TV_DOUBLE_PRECISION
// (e.g. on the Simulink library target) to build the double version instead.
#ifdef TV_DOUBLE_PRECISION
using tv_real = double;
#else
using tv_real = float;
#endif

template <typename T>
concept DecimalOrDual = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, DecimalDual<float>> ||
                        std::same_as<T, DecimalDual<double>>;
