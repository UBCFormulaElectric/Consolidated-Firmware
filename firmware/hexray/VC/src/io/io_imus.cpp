#include "io_imus.hpp"

namespace io::imus
{
 io::Imu IMU1(hw::spi::imu1);
 io::Imu IMU2(hw::spi::imu2);
 io::Imu IMU3(hw::spi::imu3);


void init()
{
    IMU1.init();
    IMU2.init();
    IMU3.init();
}

} // namespace io::imus
