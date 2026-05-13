#pragma once
#include <expected>
#include "util_errorCodes.hpp"

namespace app::batteryMonitoring
{
std::expected<void, ErrorCode> update();

}