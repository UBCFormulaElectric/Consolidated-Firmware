#pragma once

#include <cstdint>
#include <span>

#include "util_errorCodes.hpp"

namespace io::telemUart
{

#ifdef TARGET_EMBEDDED
[[nodiscard]] result<void> transmit(std::span<const uint8_t> tx_data);
#else
[[nodiscard]] inline result<void> transmit(std::span<const uint8_t> tx_data)
{
    (void)tx_data;
    return {};
}
#endif

} // namespace io::telemUart
