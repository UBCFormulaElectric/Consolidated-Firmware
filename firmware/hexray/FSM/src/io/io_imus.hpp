#pragma once

#include "util_errorCodes.hpp"
#include "io_imu.hpp"

namespace io::imus
{
extern imu imu_front;

result<void> init();
} // namespace io::imus
