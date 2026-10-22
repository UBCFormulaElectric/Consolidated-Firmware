#pragma once

namespace app::switches
{
bool start_get();
bool telem_get();
bool torque_vectoring_get();
bool regen_get();
void broadcast();
} // namespace app::switches