#pragma once

#include "util_errorCodes.hpp"
#include "io_imu.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_spis.hpp"
#endif

namespace io::imus
{
extern imu imu_rear;

std::expected<void, ErrorCode> init();
} // namespace io::imus
