#inlcude "io_imus.hpp"

namespace io::imus{

void init() {
    IMU1.init();
    IMU2.init();
    IMU3.init();
}

} // namespace io::imus
