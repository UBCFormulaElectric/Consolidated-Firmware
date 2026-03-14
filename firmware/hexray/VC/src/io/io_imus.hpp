#include "io_imu.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_spis.hpp"
#endif

namespace io::imus
{
void init();
} // namespace io::imus
