#pragma once

#include <cstdint>
#include <span>

#include "util_errorCodes.hpp"

namespace io::telemUart
{

#ifdef TARGET_EMBEDDED
[[nodiscard]] std::expected<void, ErrorCode> transmitPoll(std::span<const uint8_t> tx_data);
[[nodiscard]] std::expected<void, ErrorCode> transmitIt(std::span<const uint8_t> tx_data);
[[nodiscard]] std::expected<void, ErrorCode> receivePoll(std::span<uint8_t> rx_data);
[[nodiscard]] std::expected<void, ErrorCode> receiveIt(std::span<uint8_t> rx_data);
#else
[[nodiscard]] inline std::expected<void, ErrorCode> transmitPoll(std::span<const uint8_t> tx_data)
{
    (void)tx_data;
    return {};
}

[[nodiscard]] inline std::expected<void, ErrorCode> transmitIt(std::span<const uint8_t> tx_data)
{
    (void)tx_data;
    return {};
}

[[nodiscard]] inline std::expected<void, ErrorCode> receivePoll(std::span<uint8_t> rx_data)
{
    (void)rx_data;
    return {};
}

[[nodiscard]] inline std::expected<void, ErrorCode> receiveIt(std::span<uint8_t> rx_data)
{
    (void)rx_data;
    return {};
}
#endif

} // namespace io::telemUart
