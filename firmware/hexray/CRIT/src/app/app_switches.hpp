#pragma once

namespace app::switches
{
bool start_get();
bool telem_get();
bool regen_get();
bool torque_vectoring_get();

void broadcast();
} // namespace app::switches