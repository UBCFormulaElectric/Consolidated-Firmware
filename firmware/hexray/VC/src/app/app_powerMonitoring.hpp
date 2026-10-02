#pragma once
#include <expected>
#include <cstdint>
#include "util_errorCodes.hpp"

namespace app::powerMonitoring
{
result<void> update();
}