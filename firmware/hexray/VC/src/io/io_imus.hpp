#pragma once
#include "io_imu.hpp"

namespace io::imus
{
extern io::imu IMU1;
extern io::imu IMU2;
extern io::imu IMU3;

result<void> init();
} // namespace io::imus