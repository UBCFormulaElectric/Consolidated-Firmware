#pragma once

namespace app::switches
{
bool start_get();
bool telem_get();
bool regen_get();
void broadcast();
bool regen_switch_get();
bool PC8_Switch_get();
} // namespace app::switches