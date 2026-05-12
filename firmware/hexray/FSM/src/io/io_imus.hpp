#pragma once

#include "util_errorCodes.hpp"
#include "io_imu.hpp"

namespace io::imus
{
extern imu imu_front;

std::expected<void, ErrorCode> init();
} // namespace io::imus
