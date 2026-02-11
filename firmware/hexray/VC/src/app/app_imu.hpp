//refactor needed
//#include "hw_spi.hpp"
#include "io_imu.hpp"

namespace vc::app::imus
{
// idk what else this file is used for :(
const io::Imu::AccelData *app_imu_getAccelData(AccelData &data);
const io::Imu::GyroData  *app_imu_getGyroData(GyroData &data);
// Add a func for the IMU
} // namespace vc::app::imus