#pragma once

#include "util_errorCodes.hpp"

namespace io::sd
{
/**
 * Upgrade the SD interface to 4-bit bus width and high-speed mode. Must run after the card has been
 * initialised in 1-bit mode. This touches hardware directly, so it lives behind an io seam to keep
 * app/test code off the hw layer (the host test build links a fake instead of the STM32 HAL).
 * @return ErrorCode if either the bus-width or speed change fails.
 */
result<void> upgrade();
} // namespace io::sd
