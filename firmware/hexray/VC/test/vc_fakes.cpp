#include "fsmMocks.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"

namespace fakes::io // Set all the fake values using functions here
{
}
namespace io // Define the mocked functions here
{
namespace imus
{
    Imu IMU1;
    Imu IMU2;
    Imu IMU3;

    std::expected<void, ErrorCode> init()
    {
        return IMU1.init() | IMU2.init() | IMU3.init();
    }
} // namespace imus
} // namespace io