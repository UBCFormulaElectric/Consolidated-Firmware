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
        if (auto result = IMU1.init(); not result)
        {
            return result;
        }
        if (auto result = IMU2.init(); not result)
        {
            return result;
        }
        if (auto result = IMU3.init(); not result)
        {
            return result;
        }
        return {};
    }
} // namespace imus
} // namespace io