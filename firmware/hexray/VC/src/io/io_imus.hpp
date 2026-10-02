#pragma once
#include "io_imu.hpp"

namespace io::imus
{
extern imu IMU1;
extern imu IMU2;
extern imu IMU3;

result<void> init();
} // namespace io::imus