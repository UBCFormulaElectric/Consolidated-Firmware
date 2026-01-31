#pragma once

#include "io_canMsg.hpp"
#include <expected>
#include "util_errorCodes.hpp"

namespace io::telemMessageQueue
{

void init();

[[nodiscard]] std::expected<void, ErrorCode> push(const io::CanMsg& msg);

[[nodiscard]] std::expected<io::CanMsg, ErrorCode> pop();

} // namespace io::telemMessageQueue
