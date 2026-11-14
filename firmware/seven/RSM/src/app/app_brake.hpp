#pragma once

namespace app::brake
{
/*
 * Check if the brake is actuated through digital signal from the comparator on the RSM
 * return True if brake is actuated, false if brake is not
 */
bool isActuated();
/*
 * Broadcast brake info on CAN
 */
void broadcast();
} // namespace app::brake
