#include "io_imu.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_spis.hpp"
#endif

namespace io::imus
{
extern Imu IMU1;
extern Imu IMU2;
extern Imu IMU3;
void       init();
} // namespace io::imus
