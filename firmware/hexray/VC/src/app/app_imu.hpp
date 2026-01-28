#include "io_imu.hpp"
#include "hw_spi.hpp"

namespace vc::app::imus{
// idk what else this file is used for :(
const AccelData *app_imu_getAccelData(AccelData &data);
const GyroData *app_imu_getGyroData(GyroData &data);
} // namespace vc::app::imus