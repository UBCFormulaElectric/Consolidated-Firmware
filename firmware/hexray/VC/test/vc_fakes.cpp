#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"

io::Imu IMU1;
io::Imu IMU2;
io::Imu IMU3;

namespace io // Define the mocked functions here
{
namespace imus
{
    std::expected<void, ErrorCode> initAll()
    {
        return IMU1.init().and_then([]() { return IMU2.init(); }).and_then([]() { return IMU3.init(); });
    }
} // namespace imus
} // namespace io