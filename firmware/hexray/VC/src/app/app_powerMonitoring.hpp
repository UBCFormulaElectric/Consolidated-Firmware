#pragma once
#include <expected>
#include <cstdint>
#include "util_errorCodes.hpp"

namespace app::powerMonitoring
{
std::expected<void, ErrorCode> update();
}