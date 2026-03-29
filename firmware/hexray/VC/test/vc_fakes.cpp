#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "io_pumpControl.hpp"
#include "io_efuses.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_vcShdn.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"

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

namespace sbgEllipse
{
    const Attitude getEkfEulerAngles()
    {
        return Attitude{ 0.0f, 0.0f, 0.0f };
    }
    uint32_t getEkfSolutionMode()
    {
        return 0;
    }
    const VelocityData getEkfNavVelocityData()
    {
        return VelocityData{ 0u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    }
    uint32_t getTimestampUs()
    {
        return 0;
    }
    uint8_t getOverflowCount()
    {
        return 0;
    }
    uint32_t getComStatus()
    {
        return 0;
    }
    uint16_t getGeneralStatus()
    {
        return 0;
    }
    void broadcast() {}
} // namespace sbgEllipse

namespace powerMonitoring
{
    float read_power(unsigned char)
    {
        return 0.0f;
    }
    float read_current(unsigned char)
    {
        return 0.0f;
    }
    float read_voltage(unsigned char)
    {
        return 0.0f;
    }
    void refresh() {}
    bool init()
    {
        return true;
    }
} // namespace powerMonitoring
namespace imus
{
} // namespace imus
} // namespace io