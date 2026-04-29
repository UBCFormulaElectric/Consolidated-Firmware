#pragma once

#include "hw_gpios.hpp"

namespace io::brakeLight
{
/*
 * Brake light control
 * value = value of the brake light
 */
void set(const bool value);
} // namespace io::brakeLight