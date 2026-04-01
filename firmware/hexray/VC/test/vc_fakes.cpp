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

io::Imu            IMU1;
io::Imu            IMU2;
io::Imu            IMU3;
io::Pump           rr_pump;
io::Pump           rl_pump;
io::TI_TPS28_Efuse f_inv_efuse;
io::TI_TPS28_Efuse r_inv_efuse;
io::TI_TPS28_Efuse bms_efuse;
io::TI_TPS28_Efuse rsm_efuse;
// app::Timer         timer{};

io::TI_TPS28_Efuse dam_efuse;
io::TI_TPS28_Efuse front_efuse;
io::TI_TPS28_Efuse l_rad_fan_efuse;
io::TI_TPS28_Efuse r_rad_fan_efuse;
io::TI_TPS25_Efuse rl_pump_efuse;
io::TI_TPS25_Efuse rr_pump_efuse;

const io::shdn::node tsms_node(false, app::can_tx::VC_TSMSOKStatus_set);
const io::shdn::node inertia_stop_node(false, app::can_tx::VC_InertiaSwitch_set);
const io::shdn::node rear_right_motor_interlock_node(false, app::can_tx::VC_RearRightMotorInterlock_set);
const io::shdn::node splitter_box_interlock_node(false, app::can_tx::VC_MSDOrEMeterOKStatus_set);
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