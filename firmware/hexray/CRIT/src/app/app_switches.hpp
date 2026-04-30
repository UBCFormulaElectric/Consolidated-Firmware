#pragma once

#include "app_signal.hpp"

namespace app::switches
{
bool torque_vectoring_get();
bool launch_control_get();
bool regen_get();
bool start_get();

void broadcast();
} // namespace app::switches