#pragma once

#include "dual.hpp"
template <typename T>
concept DecimalOrDual = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, autodiff::dual>;

template <typename T>
concept Decimal = std::same_as<T, float> || std::same_as<T, double>;
