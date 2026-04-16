#pragma once

#include "dual.hpp"

template <typename T>
concept Decimal = std::same_as<T, float> || std::same_as<T, double>;

template <Decimal T> using DecimalDual = autodiff::HigherOrderDual<1, T>;

template <typename T>
concept DecimalOrDual = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, DecimalDual<float>> ||
                        std::same_as<T, DecimalDual<double>>;
