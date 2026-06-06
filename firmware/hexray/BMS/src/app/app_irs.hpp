#pragma once

namespace app::irs
{

/**
 * @brief Checks if the negative contactor has been opened for at least the debounce period.
 *
 * @return true if the negative contactor has been opened for at least the debounce period, false otherwise.
 */
bool negativeOpenedDebounced();

/**
 *  @brief Broadcast IRs status over CAN.
 */
void broadcast();
} // namespace app::irs