#include "hw_spis.hpp"
#include "io_imu.hpp"

namespace io::imus{

extern io::Imu IMU1(hw::spi::imu1);
extern io::Imu IMU2(hw::spi::imu2);
extern io::Imu IMU3(hw::spi::imu3);

void app_imu_init();
}
