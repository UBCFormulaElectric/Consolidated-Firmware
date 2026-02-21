#pragma once

#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

namespace io::suspension
{
float getRLTravel();
float getRRTravel();
bool  RR_OCSC();
bool  RL_OCSC();
} // namespace io::suspension