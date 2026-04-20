#pragma once
#include <expected>
#include <util_errorCodes.hpp>

namespace io::telemRx
{
std::expected<void, ErrorCode> transmitNTPStartMsg();
std::expected<void, ErrorCode> pollForRadioMessages();
} // namespace io::telemRx
