#include "io_imus.hpp"
#include "hw_spis.hpp"

ImuFilterConfig filter = {};
io::Imu IMU1(hw::spi::imu1, filter);
io::Imu IMU2(hw::spi::imu2, filter);
io::Imu IMU3(hw::spi::imu3, filter);
