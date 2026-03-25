#include "io_imus.hpp"
#include "hw_spis.hpp"

io::Imu IMU1(hw::spi::imu1);
io::Imu IMU2(hw::spi::imu2);
io::Imu IMU3(hw::spi::imu3);
