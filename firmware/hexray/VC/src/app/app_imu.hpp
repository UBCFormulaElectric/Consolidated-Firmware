// refactor needed
#include "io_imus.hpp"
#include "io_imu.hpp"
#ifdef TARGET_EMBEDDED
#include "io_efuse.hpp"
#endif

namespace app::imus
{
void init();
void broadcast();
// TODO: IMU state machine for averaging or taking median of sensors
} // namespace app::imus